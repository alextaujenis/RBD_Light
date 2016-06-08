// Arduino RBD Light Library v2.1.7 - Control many lights.
// https://github.com/alextaujenis/RBD_Light
// Copyright 2016 Alex Taujenis
// MIT License

#ifndef RBD_LIGHT_H
#define RBD_LIGHT_H

#include <Arduino.h>
#include <RBD_Timer.h>
#include <avr/pgmspace.h>
namespace RBD {
	

// The lookup table below is a 8bit->8bit inverse function
// of the CIE 1931 XYZ -> Lab conversion, in order
// to make the perceived brightness more linear

// L* = 116(Y/Yn)^1/3 - 16 , Y/Yn > 0.008856
// L* = 903.3(Y/Yn), Y/Yn <= 0.008856

const uint8_t pwm_table[] PROGMEM = {
     0,   0,   0,   0,   0,   1,   1,   1,   1,   1,
     1,   1,   1,   1,   2,   2,   2,   2,   2,   2,
     2,   2,   2,   3,   3,   3,   3,   3,   3,   3,
     3,   4,   4,   4,   4,   4,   4,   5,   5,   5,
     5,   5,   6,   6,   6,   6,   6,   7,   7,   7,
     7,   8,   8,   8,   8,   9,   9,   9,  10,  10,
    10,  10,  11,  11,  11,  12,  12,  12,  13,  13,
    13,  14,  14,  15,  15,  15,  16,  16,  17,  17,
    17,  18,  18,  19,  19,  20,  20,  21,  21,  22,
    22,  23,  23,  24,  24,  25,  25,  26,  26,  27,
    28,  28,  29,  29,  30,  31,  31,  32,  32,  33,
    34,  34,  35,  36,  37,  37,  38,  39,  39,  40,
    41,  42,  43,  43,  44,  45,  46,  47,  47,  48,
    49,  50,  51,  52,  53,  54,  54,  55,  56,  57,
    58,  59,  60,  61,  62,  63,  64,  65,  66,  67,
    68,  70,  71,  72,  73,  74,  75,  76,  77,  79,
    80,  81,  82,  83,  85,  86,  87,  88,  90,  91,
    92,  94,  95,  96,  98,  99, 100, 102, 103, 105,
   106, 108, 109, 110, 112, 113, 115, 116, 118, 120,
   121, 123, 124, 126, 128, 129, 131, 132, 134, 136,
   138, 139, 141, 143, 145, 146, 148, 150, 152, 154,
   155, 157, 159, 161, 163, 165, 167, 169, 171, 173,
   175, 177, 179, 181, 183, 185, 187, 189, 191, 193,
   196, 198, 200, 202, 204, 207, 209, 211, 214, 216,
   218, 220, 223, 225, 228, 230, 232, 235, 237, 240,
   242, 245, 247, 250, 252, 255,
};



#define CIELPWM(a) (pgm_read_word_near(pwm_table + a)) // CIE Lightness loopup table function
 
  class Light {
    public:
      Light(int pin, bool use_perceived_lightness_correction=true);
      void on(bool _stop_everything = true);  // turn on the light, stop everything is for internal use only
      void off(bool _stop_everything = true); // turn off the light, stop everything is for internal use only
      bool isOn();                            // returns true when the light is at pwmMax% brightness or above
      bool isOff();                           // returns true when the ligh is at pwmMin% brightness or below
      void update();                          // process real-time methods
      void setBrightness(uint8_t value, bool _stop_everything = true);        // 0 - 255
      void setBrightnessPercent(int value, bool _stop_everything = true);     // 0 - 100
      int getBrightness();        // 0 - 255
      int getBrightnessPercent(); // 0 - 100
      void blink(unsigned long on_time, unsigned long off_time, uint8_t min_brightness, uint8_t max_brightness, int times);
      void fade(unsigned long up_time, unsigned long on_time, unsigned long down_time, unsigned long off_time, uint8_t min_brightness , uint8_t max_brightness, int times);
      // overloaded blink & fade for unlimited times
      void blink(unsigned long on_time, unsigned long off_time, uint8_t min_brightness = 0, uint8_t max_brightness = 255);
      void fade(unsigned long up_time, unsigned long on_time, unsigned long down_time, unsigned long off_time, uint8_t min_brightness = 0 , uint8_t max_brightness = 255);
    private:
      // global
      int _cie= true;
	  uint8_t _minPmw = 0;
	  uint8_t _maxPmw = 255;
	  int _pin;
      int _times;
      int _pwm_value = 0;
      bool _forever  = false;
      inline void _stopEverything() __attribute__((always_inline));
      // blinking
      bool _blinking;
      inline void _blink() __attribute__((always_inline));
      inline bool _shouldBlinkOff() __attribute__((always_inline));
      inline bool _shouldBlinkOn() __attribute__((always_inline));
      inline void _startBlinking() __attribute__((always_inline));
      inline void _stopBlinking() __attribute__((always_inline));
      inline void _blinkOff(); __attribute__((always_inline));
      inline void _blinkOn(); __attribute__((always_inline));
      // fading
      bool _fading;
      enum {_RISING, _MAX, _FALLING, _MIN} _state, _oldstate;  // internal state
      inline void _fade() __attribute__((always_inline));
      inline void _rising();  __attribute__((always_inline));
      inline void _max();  __attribute__((always_inline));
      inline void _falling();  __attribute__((always_inline));
      inline void _min();  __attribute__((always_inline));
      inline void _startFading(); __attribute__((always_inline));
      inline void _stopFading();  __attribute__((always_inline));
      inline bool _shouldBeRising() __attribute__((always_inline));
      inline bool _shouldBeMax() __attribute__((always_inline));
      inline bool _shouldBeFalling() __attribute__((always_inline));
      inline bool _shouldBeMin() __attribute__((always_inline));
      inline int _risingValue() __attribute__((always_inline));
      inline int _fallingValue() __attribute__((always_inline));
      Timer _up_timer;
      Timer _on_timer;
      Timer _down_timer;
      Timer _off_timer;
	  float _slopeUp;
	  float _slopeDown;
	  float _offsetUp;
	  float _offsetDown;
	  void _preCalculate(unsigned long up_time, unsigned long down_time, uint8_t min_brightness , uint8_t max_brightness);
	  void setBrightnessInner(uint8_t value);        // 0 - 255
  };
}
#endif