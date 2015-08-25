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
    void setPwm(int value); // 0 - 255
    void setPwmPercent(int value); // 0 - 100
    bool isOn();
    bool isOff();
    void update();
    void blink(int on_time, int off_time, int times);
    void pulse(int up_time, int on_time, int down_time, int off_time, int times);
  private:
    // global
    int _pin;
    int _times;
    int _pwm_value;
    void _stopEverything();

    // blinking
    bool _on;
    bool _blinking;
    void _blink();
    bool _shouldBlinkOff();
    bool _shouldBlinkOn();
    void _startBlinking();
    void _stopBlinking();
    void _blinkOff();
    void _blinkOn();

    // pulsing
    int _status;
    bool _pulsing;
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
};

#endif