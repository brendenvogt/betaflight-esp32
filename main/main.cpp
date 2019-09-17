/*
 * 1. Open up the project properties
 * 2. Visit C/C++ General > Preprocessor Include Paths, Macros, etc
 * 3. Select the Providers tab
 * 4. Check the box for "CDT GCC Built-in Compiler Settings"
 * 5. Set the compiler spec command to "xtensa-esp32-elf-gcc ${FLAGS} -E -P -v -dD "${INPUTS}""
 * 6. Rebuild the index
 */

#include <esp_log.h>
#include <string>
#include <sstream>

#include "sdkconfig.h"
#include "Arduino.h"

static char tag[] = "cpp_main";

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

BLEServer *pServer = NULL;
BLECharacteristic * pTxCharacteristic;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint8_t txValue = 0;

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" // UART service UUID
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

extern "C"
{
void app_main(void);
}

class MyServerCallbacks: public BLEServerCallbacks
{
	void onConnect(BLEServer* pServer)
	{
		ESP_LOGI(tag, "Device Connected");
		deviceConnected = true;
	};

	void onDisconnect(BLEServer* pServer)
	{
		ESP_LOGI(tag, "Device Disconnected");
		deviceConnected = false;
	};
};

class MyCallbacks: public BLECharacteristicCallbacks
{
	void onWrite(BLECharacteristic *pCharacteristic)
	{
		std::string rxValue = pCharacteristic->getValue();

		if (rxValue.length() > 0)
		{

			ESP_LOGI(tag, "*********");
			ESP_LOGI(tag, "Received Value: ");
			std::stringstream ss;
			for (int i = 0; i < rxValue.length(); i++)
			{
				ss << rxValue[i];
			}
			ESP_LOGI(tag, "%s", ss.str().c_str());
			ESP_LOGI(tag, "*********");
		}
	}
};

void setup()
{
	ESP_LOGI(tag, "initBLE");

	// Create the BLE Device
	BLEDevice::init("UART Service");

	// Create the BLE Server
	pServer = BLEDevice::createServer();
	pServer->setCallbacks(new MyServerCallbacks());

	// Create the BLE Service
	BLEService *pService = pServer->createService(SERVICE_UUID);

	// Create a BLE Characteristic
	pTxCharacteristic = pService->createCharacteristic(
	CHARACTERISTIC_UUID_TX, BLECharacteristic::PROPERTY_NOTIFY);

	pTxCharacteristic->addDescriptor(new BLE2902());

	BLECharacteristic * pRxCharacteristic = pService->createCharacteristic(
	CHARACTERISTIC_UUID_RX, BLECharacteristic::PROPERTY_WRITE);

	pRxCharacteristic->setCallbacks(new MyCallbacks());

	// Start the service
	pService->start();

	// Start advertising
	pServer->getAdvertising()->addServiceUUID(pService->getUUID());
	pServer->getAdvertising()->start();
//  Serial.println("Waiting a client connection to notify...");
}

void loop()
{

	if (deviceConnected)
	{
		pTxCharacteristic->setValue(&txValue, 1);
		pTxCharacteristic->notify();
		txValue++;
		delay(10); // bluetooth stack will go into congestion, if too many packets are sent
	}

	// disconnecting
	if (!deviceConnected && oldDeviceConnected)
	{
		delay(500); // give the bluetooth stack the chance to get things ready
		pServer->startAdvertising(); // restart advertising
//        Serial.println("start advertising");
		oldDeviceConnected = deviceConnected;
	}
	// connecting
	if (deviceConnected && !oldDeviceConnected)
	{
		// do stuff here on connecting
		oldDeviceConnected = deviceConnected;
	}
}

void app_main(void)
{
	setup();
	while (true)
	{
		loop();
	}

}

