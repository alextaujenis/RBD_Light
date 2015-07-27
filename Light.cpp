#include "Arduino.h"
#include "Light.h"

Light::Light(int pin) {
  // capture the injected pin number to a local variable
  _pin = pin;

  // prepare the physical port for control
  pinMode(_pin, OUTPUT);

  // set the internal state according to reality
  _on = digitalRead(_pin);
}

void Light::on() {
  // turn the light on
  digitalWrite(_pin, HIGH);

  // set the internal state
  _on = true;
}

void Light::off() {
  // turn the light off
  digitalWrite(_pin, LOW);

  // set the internal state
  _on = false;
}

bool Light::isOn() {
  // accessor method
  return _on;
}

bool Light::isOff() {
  // accessor method
  return !_on;
}