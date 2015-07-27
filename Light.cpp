#include "Arduino.h"
#include "Light.h"

Light::Light(int pin) {
  _pin = pin;
  pinMode(_pin, OUTPUT);
}

void Light::on() {
  digitalWrite(_pin, HIGH);
}

void Light::off() {
  digitalWrite(_pin, LOW);
}