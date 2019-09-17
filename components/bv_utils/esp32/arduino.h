#ifndef COMPONENTS_BV_ARDUINO_HPP_
#define COMPONENTS_BV_ARDUINO_HPP_

#define HIGH 1
#define LOW 0

#define INPUT 1
#define OUTPUT 0

void pinMode(int pin, int mode);
int digitalRead(int pin);
void digitalWrite(int pin, int val);
void delay(int delayMs);
void blink(int pin, int blinkCount = 2);

#endif /* COMPONENTS_BV_ARDUINO_HPP_ */
