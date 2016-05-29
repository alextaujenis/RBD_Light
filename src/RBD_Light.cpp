// Arduino RBD Light Library v2.1.7 - Control many lights.
// https://github.com/alextaujenis/RBD_Light
// Copyright 2016 Alex Taujenis
// MIT License

#include <Arduino.h>
#include <RBD_Timer.h> // https://github.com/alextaujenis/RBD_Timer
#include <RBD_Light.h> // https://github.com/alextaujenis/RBD_Light

namespace RBD {
  Light::Light(int pin, bool use_perceived_lightness_correction)
  : _up_timer(), _on_timer(), _down_timer(), _off_timer() {
    _pin = pin;
	_cie = use_perceived_lightness_correction;
    pinMode(_pin, OUTPUT);
  }

  void Light::on(bool _stop_everything) { // default: true
    setBrightness(255, _stop_everything);
  }

  void Light::off(bool _stop_everything) { // default: true
    setBrightness(0, _stop_everything);
  }

  bool Light::isOn() {
    return getBrightness() >= _maxPmw;
  }

  bool Light::isOff() {
    return getBrightness() <= _minPmw;
  }

  void Light::update() {
    if(_blinking) {
      _blink();
    }
    if(_fading) {
      _fade();
    }
  }

  void Light::setBrightness(uint8_t value, bool _stop_everything) {
    if(_stop_everything) {
      _stopEverything();
    }
    setBrightnessInner(value);
  }

  inline void Light::setBrightnessInner(const uint8_t value) {
    if(_pwm_value == value) return;
    _pwm_value = constrain(value, _minPmw, _maxPmw);
    analogWrite(_pin, _cie?CIELPWM(_pwm_value):_pwm_value);
  }

  void Light::setBrightnessPercent(int value, bool _stop_everything) {
    setBrightness(int(value * 255.0), _stop_everything);
  }

  int Light::getBrightness() {
    return _pwm_value;
  }

  int Light::getBrightnessPercent() {
    return int(getBrightness() * 0.392156862745);
  }

  void Light::blink(unsigned long on_time, unsigned long off_time, uint8_t min_brightness , uint8_t max_brightness, int times) {
    
	_forever = false;
    _times   = times;
    _minPmw = min_brightness;
	_maxPmw = max_brightness;
	_on_timer.setTimeout(on_time);
    _off_timer.setTimeout(off_time);
    _stopEverything();
    _startBlinking();
  }

  // unlimited times
  void Light::blink(unsigned long on_time, unsigned long off_time,  uint8_t min_brightness , uint8_t max_brightness) {
    _forever = true;
    _times   = 0;
    _minPmw = min_brightness;
	_maxPmw = max_brightness;
    _on_timer.setTimeout(on_time);
    _off_timer.setTimeout(off_time);
    _stopEverything();
    _startBlinking();
  }

  void Light::fade(unsigned long up_time, unsigned long on_time, unsigned long down_time, unsigned long off_time, uint8_t min_brightness , uint8_t max_brightness, int times) {
    _forever = false;
    _times   = times;

	_preCalculate(up_time,down_time,min_brightness,max_brightness);
    _up_timer.setTimeout(up_time);
    _on_timer.setTimeout(on_time);
    _down_timer.setTimeout(down_time);
    _off_timer.setTimeout(off_time);
    _stopEverything();
    _startFading();
  }

  // unlimited times
  void Light::fade(unsigned long up_time, unsigned long on_time, unsigned long down_time, unsigned long off_time, uint8_t min_brightness , uint8_t max_brightness) {
    _forever = true;
    _times   = 0;

    _preCalculate(up_time,down_time,min_brightness,max_brightness);	
    _up_timer.setTimeout(up_time);
    _on_timer.setTimeout(on_time);
    _down_timer.setTimeout(down_time);
    _off_timer.setTimeout(off_time);
    _stopEverything();
    _startFading();
  }

  // Private

  void Light::_preCalculate(unsigned long up_time, unsigned long down_time, uint8_t min_brightness , uint8_t max_brightness) {
    _minPmw = min_brightness;
	_maxPmw = max_brightness;	
	_slopeUp = (float)(_maxPmw-_minPmw)/ (float)up_time;
	_offsetUp = _minPmw;
	_slopeDown = (float)(_maxPmw-_minPmw)/ (float)down_time;
	_offsetDown = _minPmw;
  }
  
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
    if(_forever) return;
    if(_times > 0) {_times--;}
    else if(_times == 0) {_stopBlinking();}    
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
      setBrightnessInner(_risingValue()); // don't stop everything
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
	return int(_offsetDown + _up_timer.getValue() * _slopeUp);
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
      setBrightnessInner(_fallingValue()); // don't stop everything
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
	return int(_offsetDown + _down_timer.getInverseValue() * _slopeDown);
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