// light.h - A high-level library for controlling LEDs

#ifndef LIGHT_H
#define LIGHT_H

#include "Arduino.h"

class Light {
  public:
    Light(int pin);
    void on();
    void off();
    bool isOn();
    bool isOff();
    void blink(int on_time, int off_time, int times);
    void update();
  private:
    int _pin;
    bool _on;
    void _blink();
    bool _blinking;
    int _blink_on_time;
    int _blink_off_time;
    int _blink_times;
    unsigned long _blink_on_timer;
    unsigned long _blink_off_timer;
};

#endif