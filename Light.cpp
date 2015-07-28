#include "Arduino.h"
#include "Light.h"

Light::Light(int pin) {
  _pin = pin;
}

void Light::on() {
  pwm(255);
}

void Light::off() {
  pwm(0);
}

void Light::pwm(int value) {
  if(value > 0) {
    _on = true;
  }
  else {
    _on = false;
  }
  analogWrite(_pin, value);
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
  if(_pulsing) {
    _pulse();
  }
}

void Light::blink(int on_time, int off_time, int times) {
  _blink_on_time  = on_time;
  _blink_off_time = off_time;
  _blink_times    = times;
  _stopEverything();
  _startBlinking();
}

void Light::pulse(int up_time, int on_time, int down_time, int off_time, int times) {
  _pulse_up_time   = up_time;
  _pulse_on_time   = on_time;
  _pulse_down_time = down_time;
  _pulse_off_time  = off_time;
  _pulse_times     = times;
  _stopEverything();
  _startPulsing();
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
      _stopBlinking();
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

void Light::_pulse() {
  switch(_status) {
    case RISING:
      _rising();
      break;
    case MAX:
      _max();
      break;
    case FALLING:
      _falling();
      break;
    case MIN:
      _min();
      break;
  }
}

void Light::_rising() {
  if(_shouldBeRising()) {
    pwm(_risingValue());
  }
  else {
    _pulse_on_timer = millis();
    _status = MAX;
  }
}

bool Light::_shouldBeRising() {
  return (millis() - _pulse_up_timer < _pulse_up_time);
}

int Light::_risingValue() {
  return int(((millis() - _pulse_up_timer) / float(_pulse_up_time)) * 255);
}

void Light::_max() {
  if(_shouldBeMax()) {
    on();
  }
  else {
    _pulse_down_timer = millis();
    _status = FALLING;
  }
}

bool Light::_shouldBeMax() {
  return (millis() - _pulse_on_timer < _pulse_on_time);
}

void Light::_falling() {
  if(_shouldBeFalling()) {
    pwm(_fallingValue());
  }
  else {
    _pulse_off_timer = millis();
    _status = MIN;
  }
}

bool Light::_shouldBeFalling() {
  return (millis() - _pulse_down_timer < _pulse_down_time);
}

int Light::_fallingValue() {
  return int(abs(1 - ((millis() - _pulse_down_timer) / float(_pulse_down_time))) * 255);
}

void Light::_min() {
  if(_shouldBeMin()) {
    off();
  }
  else {
    _pulse_times--;

    if(_pulse_times == 0) {
      _stopPulsing();
    }
    else {
      _pulse_up_timer = millis();
      _status = RISING;
    }
  }
}

bool Light::_shouldBeMin() {
  return (millis() - _pulse_off_timer < _pulse_off_time);
}

void Light::_startPulsing() {
  _status         = RISING;
  _pulse_up_timer = millis();
  _pulsing        = true;
}

void Light::_stopPulsing() {
  _pulsing = false;
}

void Light::_startBlinking() {
  _blinking = true;
}

void Light::_stopBlinking() {
  _blinking = false;
}

void Light::_stopEverything() {
  _stopBlinking();
  _stopPulsing();
}