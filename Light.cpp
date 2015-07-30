#include <Arduino.h>
#include <Light.h>
#include <Timer.h>

Timer _up_timer;
Timer _on_timer;
Timer _down_timer;
Timer _off_timer;

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
  _on_timer.setTimeout(on_time);
  _off_timer.setTimeout(off_time);
  _times = times;
  _stopEverything();
  _startBlinking();
}

void Light::pulse(int up_time, int on_time, int down_time, int off_time, int times) {
  _up_timer.setTimeout(up_time);
  _on_timer.setTimeout(on_time);
  _down_timer.setTimeout(down_time);
  _off_timer.setTimeout(off_time);
  _times = times;
  _stopEverything();
  _startPulsing();
}


// Private

void Light::_blink() {
  if(isOn() && _shouldBlinkOff()) {
    // turn it off
    off();
    // restart the off timer
    _off_timer.restart();
    // decrement the blinks
    _times--;
    // stop if this is the last blink
    if(_times == 0) {_stopBlinking();}
  }
  else if(isOff() && _shouldBlinkOn()) {
    // turn it on
    on();
    // restart the on timer
    _on_timer.restart();
  }
}

bool Light::_shouldBlinkOff() {
  return _on_timer.isInactive();
}

bool Light::_shouldBlinkOn() {
  return _off_timer.isInactive();
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
    _on_timer.restart();
    _status = MAX;
  }
}

bool Light::_shouldBeRising() {
  return _up_timer.isActive();
}

int Light::_risingValue() {
  return int(_up_timer.getPercentValue() * 255);
}

void Light::_max() {
  if(_shouldBeMax()) {
    on();
  }
  else {
    _down_timer.restart();
    _status = FALLING;
  }
}

bool Light::_shouldBeMax() {
  return _on_timer.isActive();
}

void Light::_falling() {
  if(_shouldBeFalling()) {
    pwm(_fallingValue());
  }
  else {
    _off_timer.restart();
    _status = MIN;
  }
}

bool Light::_shouldBeFalling() {
  return _down_timer.isActive();
}

int Light::_fallingValue() {
  return int(_down_timer.getInversePercentValue() * 255);
}

void Light::_min() {
  if(_shouldBeMin()) {
    off();
  }
  else {
    _times--;

    if(_times == 0) {
      _stopPulsing();
    }
    else {
      _up_timer.restart();
      _status = RISING;
    }
  }
}

bool Light::_shouldBeMin() {
  return _off_timer.isActive();
}

void Light::_startPulsing() {
  _up_timer.restart();
  _status  = RISING;
  _pulsing = true;
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