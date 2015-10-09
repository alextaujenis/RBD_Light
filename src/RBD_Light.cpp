// Arduino RBD Light Library - A simple real-time library for controlling many lights
// Copyright 2015 Alex Taujenis
// MIT License

#include <Arduino.h>
#include <RBD_Light.h>
#include <RBD_Timer.h>

namespace RBD {

  Timer _up_timer;
  Timer _on_timer;
  Timer _down_timer;
  Timer _off_timer;

  Light::Light(int pin) {
    _pin = pin;
  }

  void Light::on() {
    setPwm(255);
  }

  void Light::off() {
    setPwm(0);
  }

  bool Light::isOn() {
    return getPwm() == 255;
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

  void Light::setPwm(int value) {
    if(value > -1 && value < 256){
      analogWrite(_pin, value);
      _pwm_value = value;
      _on = bool(value);
    }
  }

  void Light::setPwmPercent(int value) {
    setPwm(int(value / 100.0 * 255));
  }

  int Light::getPwm() {
    return _pwm_value;
  }

  int Light::getPwmPercent() {
    return int(getPwm() / 255.0 * 100);
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
      _blinkOff();
    }
    else if(isOff() && _shouldBlinkOn()) {
      _blinkOn();
    }
  }

  void Light::_blinkOff() {
    off();
    _off_timer.restart();
    _times--;
    if(_times == 0) {_stopBlinking();}
  }

  void Light::_blinkOn() {
    on();
    _on_timer.restart();
  }

  bool Light::_shouldBlinkOff() {
    return _on_timer.isExpired();
  }

  bool Light::_shouldBlinkOn() {
    return _off_timer.isExpired();
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
      setPwm(_risingValue());
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
    return int(_up_timer.getPercentValue() / 100.0 * 255);
  }

  void Light::_max() {
    if(_shouldBeMax()) {
      if(!isOn()) {
        on();
      }
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
      setPwm(_fallingValue());
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
    return int(_down_timer.getInversePercentValue() / 100.0 * 255);
  }

  void Light::_min() {
    if(_shouldBeMin()) {
      if(getPwm() > 0) {
        off();
      }
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
}