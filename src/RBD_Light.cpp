// Arduino RBD Light Library v2.1.8 - Control many lights.
// https://github.com/alextaujenis/RBD_Light
// Copyright (c) 2015 Alex Taujenis - MIT License

#include <Arduino.h>
#include <RBD_Timer.h> // https://github.com/alextaujenis/RBD_Timer
#include <RBD_Light.h> // https://github.com/alextaujenis/RBD_Light

namespace RBD {
  SimpleLight::SimpleLight(int pin, bool isSink) {
    _pin = pin;
    _isSink = isSink;

    if (isSink) {
      analogWrite(_pin, 255);
    }
    pinMode(_pin, OUTPUT);
  }

  void SimpleLight::on(bool _stop_everything) { // default: true
    setBrightness(255, _stop_everything);
  }
  
  void SimpleLight::setState(bool state, bool _stop_everything) { // default: true
    if (state) {
      on(_stop_everything);
    } else {
      off(_stop_everything);
    }
  }
  
  void SimpleLight::toggle() {
    setBrightness(255 - getBrightness());
  }

  void SimpleLight::off(bool _stop_everything) { // default: true
    setBrightness(0, _stop_everything);
  }

  bool SimpleLight::isOn() {
    return getBrightness() == 255;
  }

  bool SimpleLight::isOff() {
    return getBrightness() == 0;
  }

  void SimpleLight::setBrightness(int value, bool _stop_everything) {
    if(_stop_everything) {
      _stopEverything();
    }
    if(_pwm_value != value) {
      _pwm_value = constrain(value, 0, 255);
      if (_isSink) {
        analogWrite(_pin, 255 - _pwm_value);
      } else {
        analogWrite(_pin, _pwm_value);  
      }
    }
  }

  void SimpleLight::setBrightnessPercent(int value, bool _stop_everything) {
    setBrightness(int(value / 100.0 * 255), _stop_everything);
  }

  int SimpleLight::getBrightness() {
    return _pwm_value;
  }

  int SimpleLight::getBrightnessPercent() {
    return int(getBrightness() / 255.0 * 100);
  }
  
  void SimpleLight::_stopEverything() {
    // do nothing
  }





  Light::Light(int pin, bool isSink)
  : SimpleLight(pin, isSink), _up_timer(), _on_timer(), _down_timer(), _off_timer() {

  }

  void Light::update() {
    if(_blinking) {
      _blink();
    }
    if(_fading) {
      _fade();
    }
  }

  void Light::blink(unsigned long on_time, unsigned long off_time, int times) {
    _forever = false;
    _times   = times;

    _on_timer.setTimeout(on_time);
    _off_timer.setTimeout(off_time);
    _stopEverything();
    _startBlinking();
  }

  // unlimited times
  void Light::blink(unsigned long on_time, unsigned long off_time) {
    _forever = true;
    _times   = 0;

    _on_timer.setTimeout(on_time);
    _off_timer.setTimeout(off_time);
    _stopEverything();
    _startBlinking();
  }

  void Light::fade(unsigned long up_time, unsigned long on_time, unsigned long down_time, unsigned long off_time, int times) {
    _forever = false;
    _times   = times;

    _up_timer.setTimeout(up_time);
    _on_timer.setTimeout(on_time);
    _down_timer.setTimeout(down_time);
    _off_timer.setTimeout(off_time);
    _stopEverything();
    _startFading();
  }

  // unlimited times
  void Light::fade(unsigned long up_time, unsigned long on_time, unsigned long down_time, unsigned long off_time) {
    _forever = true;
    _times   = 0;

    _up_timer.setTimeout(up_time);
    _on_timer.setTimeout(on_time);
    _down_timer.setTimeout(down_time);
    _off_timer.setTimeout(off_time);
    _stopEverything();
    _startFading();
  }


  // Private

  void Light::_blink() {
    if(isOn() && _shouldBlinkOff()) {
      _blinkOff();
    }
    else if(isOff() && _shouldBlinkOn()) {
      _blinkOn();
    }
    else if(!isOn() && !isOff()) {
      _blinkOn();
    }
  }

  void Light::_blinkOff() {
    off(false); // don't stop everything
    _off_timer.restart();
    if(!_forever) {
      if(_times > 0) {_times--;}
      if(_times == 0) {_stopBlinking();}
    }
  }

  void Light::_blinkOn() {
    on(false); // don't stop everything
    _on_timer.restart();
  }

  bool Light::_shouldBlinkOff() {
    return _on_timer.isExpired();
  }

  bool Light::_shouldBlinkOn() {
    return _off_timer.isExpired();
  }

  void Light::_fade() {
    switch(_state) {
      case _RISING:
        _rising();
        break;
      case _MAX:
        _max();
        break;
      case _FALLING:
        _falling();
        break;
      case _MIN:
        _min();
        break;
    }
  }

  void Light::_rising() {
    if(_shouldBeRising()) {
      setBrightness(_risingValue(), false); // don't stop everything
    }
    else {
      _on_timer.restart();
      _state = _MAX;
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
        on(false); // don't stop everything
      }
    }
    else {
      _down_timer.restart();
      _state = _FALLING;
    }
  }

  bool Light::_shouldBeMax() {
    return _on_timer.isActive();
  }

  void Light::_falling() {
    if(_shouldBeFalling()) {
      setBrightness(_fallingValue(), false); // don't stop everything
    }
    else {
      _off_timer.restart();
      _state = _MIN;
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
        off(false); // don't stop everything
      }
    }
    else {
      if(!_forever && _times > 0) {_times--;}
      if(!_forever && _times == 0) {
        _stopFading();
      }
      else {
        _up_timer.restart();
        _state = _RISING;
      }
    }
  }

  bool Light::_shouldBeMin() {
    return _off_timer.isActive();
  }

  void Light::_startFading() {
    if(_times > 0 || _forever) {
      _up_timer.restart();
      _state  = _RISING;
      _fading = true;
    }
  }

  void Light::_stopFading() {
    _fading = false;
  }

  void Light::_startBlinking() {
    if(_times > 0 || _forever) {
      _blinking = true;
    }
  }

  void Light::_stopBlinking() {
    _blinking = false;
  }

  void Light::_stopEverything() {
    _stopBlinking();
    _stopFading();
  }
}