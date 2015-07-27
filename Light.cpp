#include "Arduino.h"
#include "Light.h"

Light::Light(int pin) {
  _pin = pin;
  pinMode(_pin, OUTPUT);
}

void Light::on() {
  digitalWrite(_pin, HIGH);
  _on = true;
}

void Light::off() {
  digitalWrite(_pin, LOW);
  _on = false;
}

bool Light::isOn() {
  return _on;
}

bool Light::isOff() {
  return !_on;
}