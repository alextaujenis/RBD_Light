// Arduino RBD Light Library v1.0.4 - Control many lights.
// https://github.com/alextaujenis/RBD_Light
// Copyright 2015 Alex Taujenis
// MIT License

#ifndef RBD_LIGHT_H
#define RBD_LIGHT_H

#include <Arduino.h>

#define RISING  0
#define MAX     1
#define FALLING 2
#define MIN     3

namespace RBD {
  class Light {
    public:
      Light(int pin);
      void on();
      void off();
      bool isOn();
      bool isOff();
      void update();
      void setPwm(int value);        // 0 - 255
      void setPwmPercent(int value); // 0 - 100
      int getPwm();                  // 0 - 255
      int getPwmPercent();           // 0 - 100
      // blink & fade
      void blink(unsigned long on_time, unsigned long off_time, int times);
      void fade(unsigned long up_time, unsigned long on_time, unsigned long down_time, unsigned long off_time, int times);
      // overloaded blink & fade for optional last parameter
      void blink(unsigned long on_time, unsigned long off_time);
      void fade(unsigned long up_time, unsigned long on_time, unsigned long down_time, unsigned long off_time);
    private:
      // global
      int _pin;
      int _times;
      int _pwm_value;
      bool _forever = false;
      void _stopEverything();
      // blinking
      bool _blinking;
      void _blink();
      bool _shouldBlinkOff();
      bool _shouldBlinkOn();
      void _startBlinking();
      void _stopBlinking();
      void _blinkOff();
      void _blinkOn();
      // fading
      int _status;
      bool _fading;
      void _fade();
      void _rising();
      void _max();
      void _falling();
      void _min();
      void _startFading();
      void _stopFading();
      bool _shouldBeRising();
      bool _shouldBeMax();
      bool _shouldBeFalling();
      bool _shouldBeMin();
      int _risingValue();
      int _fallingValue();
  };
}
#endif