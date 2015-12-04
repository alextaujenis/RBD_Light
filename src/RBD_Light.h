// Arduino RBD Light Library v2.1.1 - Control many lights.
// https://github.com/alextaujenis/RBD_Light
// Copyright 2015 Alex Taujenis
// MIT License

#ifndef RBD_LIGHT_H
#define RBD_LIGHT_H

#include <Arduino.h>
#include <RBD_Timer.h>

namespace RBD {
  class Light {
    static const int _RISING  = 0;
    static const int _MAX     = 1;
    static const int _FALLING = 2;
    static const int _MIN     = 3;

    public:
      Light(int pin);
      void on(bool stop_everything = true);
      void off(bool stop_everything = true);
      bool isOn();
      bool isOff();
      void update();
      void setBrightness(int value, bool stop_everything = true);        // 0 - 255
      void setBrightnessPercent(int value, bool stop_everything = true); // 0 - 100
      int getBrightness();        // 0 - 255
      int getBrightnessPercent(); // 0 - 100
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
      Timer _up_timer;
      Timer _on_timer;
      Timer _down_timer;
      Timer _off_timer;
  };
}
#endif