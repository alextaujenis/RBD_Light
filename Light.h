// light.h - A high-level library for controlling LEDs

#ifndef LIGHT_H
#define LIGHT_H

#include "Arduino.h"

#define RISING  0
#define MAX     1
#define FALLING 2
#define MIN     3

class Light {
  public:
    Light(int pin);
    void on();
    void off();
    void pwm(int value); // 0 - 255
    bool isOn();
    bool isOff();
    void update();
    void blink(int on_time, int off_time, int times);
    void pulse(int up_time, int on_time, int down_time, int off_time, int times);
  private:
    // global
    int _pin;

    // blinking
    bool _on;
    bool _blinking;
    int _blink_on_time;
    int _blink_off_time;
    int _blink_times;
    unsigned long _blink_on_timer;
    unsigned long _blink_off_timer;
    void _blink();
    bool _shouldBlinkOff();
    bool _shouldBlinkOn();
    void _startBlinking();
    void _stopBlinking();

    // pulsing
    bool _pulsing;
    int _pulse_up_time;
    int _pulse_on_time;
    int _pulse_down_time;
    int _pulse_off_time;
    int _pulse_times;
    int _status;
    unsigned long _pulse_up_timer;
    unsigned long _pulse_on_timer;
    unsigned long _pulse_down_timer;
    unsigned long _pulse_off_timer;
    void _pulse();
    void _rising();
    void _max();
    void _falling();
    void _min();
    void _startPulsing();
    void _stopPulsing();
    bool _shouldBeRising();
    bool _shouldBeMax();
    bool _shouldBeFalling();
    bool _shouldBeMin();
    int _risingValue();
    int _fallingValue();
    void _stopEverything();
};

#endif