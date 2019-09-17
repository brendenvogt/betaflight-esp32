
#include "arduino.h"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include <sdkconfig.h>

void pinMode(int pin, int mode)
{
    switch (mode)
    {
    case 0:
        gpio_set_direction((gpio_num_t)pin, GPIO_MODE_OUTPUT);
        break;
    case 1:
        gpio_set_direction((gpio_num_t)pin, GPIO_MODE_INPUT);
        break;
    default:
        gpio_set_direction((gpio_num_t)pin, GPIO_MODE_DISABLE);
        break;
    }
}

int digitalRead(int pin)
{
    return gpio_get_level((gpio_num_t)pin);
}

void digitalWrite(int pin, int val)
{
    gpio_set_level((gpio_num_t)pin, val);
}

void delay(int delayMs)
{
    vTaskDelay(delayMs / portTICK_PERIOD_MS);
}

void blink(int pin, int blinkCount)
{
    int delayMs = 50;
    int lastState = digitalRead(pin);
    digitalWrite(pin, LOW);
    delay(delayMs);
    for (int i = 0; i < blinkCount; i++)
    {
        digitalWrite(pin, HIGH);
        delay(delayMs);
        digitalWrite(pin, LOW);
        delay(delayMs);
    }
    digitalWrite(pin, lastState);
}
