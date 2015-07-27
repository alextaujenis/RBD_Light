// light.h - A high-level library for controlling LEDs

#ifndef LIGHT_H
#define LIGHT_H

#include "Arduino.h"

class Light {
  public:
    Light(int pin);
    void on();
    void off();
  private:
    int _pin;
};

#endif