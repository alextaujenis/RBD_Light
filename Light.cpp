#include "Arduino.h"
#include "Light.h"

Light::Light(int pin) {
  // capture the injected pin number to a local variable
  _pin = pin;

  // prepare the physical port for control
  pinMode(_pin, OUTPUT);

  // set the internal light state according to reality
  _on = digitalRead(_pin);

  // initialize the local blink variables
  _blinking        = false;
  _blink_on_time   = 0;
  _blink_off_time  = 0;
  _blink_on_timer  = 0;
  _blink_off_timer = 0;
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

// public real time method to keep this class blinking
void Light::update() {
  if (_blinking){
    _blink();
  }
}

// public method to start the blinking
void Light::blink(int on_time, int off_time, int times) {
  // save the values locally
  _blink_on_time  = on_time;
  _blink_off_time = off_time;
  _blink_times    = times;

  // reset the timers to start immediately
  _blink_on_timer  = 0;
  _blink_off_timer = 0;

  // start blinking
  _blinking = true;
}

// private real-time method to blink the lights
void Light::_blink() {
  // if the light is on, the blink on timer has run out, and we have blinks left
  if(isOn() && millis() - _blink_on_timer > _blink_on_time && _blink_times > 0) {
    // turn the light off
    off();

    // restart the blink off timer
    _blink_off_timer = millis();

    // decrement the blinks
    _blink_times--;

    // if this was the last blink
    if(_blink_times == 0) {
      // stop blinking
      _blinking = false;
    }
  }
  // if the light is off, the blink off timer has run out, and we have blinks left
  if(isOff() && millis() - _blink_off_timer > _blink_off_time && _blink_times > 0) {
    // turn it on
    on();

    // restart the blink on timer
    _blink_on_timer = millis();
  }
}