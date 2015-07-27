#include "Arduino.h"
#include "Light.h"

Light::Light(int pin) {
  _pin = pin;
}

void Light::on() {
  analogWrite(_pin, 255);
  _on = true;
}

void Light::off() {
  analogWrite(_pin, 0);
  _on = false;
}

bool Light::isOn() {
  return _on;
}

bool Light::isOff() {
  return !_on;
}

void Light::update() {
  if(_blinking) {
    _blink();
  }
}

void Light::blink(int on_time, int off_time, int times) {
  _blink_on_time  = on_time;
  _blink_off_time = off_time;
  _blink_times    = times;
  _blinking       = true;
}


// Private

void Light::_blink() {
  if(isOn() && _shouldBlinkOff()) {
    // turn it off
    off();
    // reset the off timer
    _blink_off_timer = millis();
    // decrement the number of blinks left
    _blink_times--;
    // if this is the last blink
    if(_blink_times == 0) {
      // stop blinking
      _blinking = false;
    }
  }
  else if(isOff() && _shouldBlinkOn()) {
    // turn it on
    on();
    // reset the on timer
    _blink_on_timer = millis();
  }
}

bool Light::_shouldBlinkOff() {
  return (millis() - _blink_on_timer > _blink_on_time);
}

bool Light::_shouldBlinkOn() {
  return (millis() - _blink_off_timer > _blink_off_time);
}