// Arduino RBD Light Library v1.0.2 - Control many lights.
// https://github.com/alextaujenis/RBD_Light
// Copyright 2015 Alex Taujenis
// MIT License

#include <Arduino.h>
#include <RBD_Timer.h> // https://github.com/alextaujenis/RBD_Timer
#include <RBD_Light.h> // https://github.com/alextaujenis/RBD_Light

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
    return getPwm() == 0;
  }

  void Light::update() {
    if(_blinking) {
      _blink();
    }
    if(_fading) {
      _fade();
    }
  }

  void Light::setPwm(int value) {
    if(value > -1 && value < 256){
      analogWrite(_pin, value);
      _pwm_value = value;
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

  void Light::blink(unsigned long on_time, unsigned long off_time, int times) {
    _forever = false;
    _on_timer.setTimeout(on_time);
    _off_timer.setTimeout(off_time);
    _times = times;
    _stopEverything();
    _startBlinking();
  }

  // unlimited times
  void Light::blink(unsigned long on_time, unsigned long off_time) {
    _forever = true;
    blink(on_time, off_time, 0);
  }

  void Light::fade(unsigned long up_time, unsigned long on_time, unsigned long down_time, unsigned long off_time, int times) {
    _forever = false;
    _up_timer.setTimeout(up_time);
    _on_timer.setTimeout(on_time);
    _down_timer.setTimeout(down_time);
    _off_timer.setTimeout(off_time);
    _times = times;
    _stopEverything();
    _startFading();
  }

  // unlimited times
  void Light::fade(unsigned long up_time, unsigned long on_time, unsigned long down_time, unsigned long off_time) {
    _forever = true;
    fade(up_time, on_time, down_time, off_time, 0);
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
    if(!_forever) {
      _times--;
      if(_times == 0) {_stopBlinking();}
    }
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

  void Light::_fade() {
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
      if(!isOff()) {
        off();
      }
    }
    else {
      if(!_forever) {_times--;}

      if(_times == 0 && !_forever) {
        _stopFading();
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

  void Light::_startFading() {
    _up_timer.restart();
    _status  = RISING;
    _fading = true;
  }

  void Light::_stopFading() {
    _fading = false;
  }

  void Light::_startBlinking() {
    _blinking = true;
  }

  void Light::_stopBlinking() {
    _blinking = false;
  }

  void Light::_stopEverything() {
    _stopBlinking();
    _stopFading();
  }
}