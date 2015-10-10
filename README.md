#Arduino Light Library
Control many lights in real-time and blink without delay using commands such as [on()](#on), [off()](#off), [blink()](#blinkon_time-off_time-times1), and [fade()](#fadeup_time-on_time-down_time-off_time-times1). Lights must be connected to PWM enabled Arduino pins to work with this library.

##Real-Time Library

This real-time library requires that you manage the flow of your program without delay() or interrupts. Check out this [Arduino Timer Library](https://github.com/alextaujenis/RBD_Timer) if you are managing millis() by hand.

##Example Setup

0. Install this Light Library and the [RBD::Timer Library](https://github.com/alextaujenis/RBD_Timer) dependency
0. Load the example sketch on to an Arduino
0. Watch the onboad Arduino LED 13 blink without delay

**Example Sketch**:

    #include <RBD_Timer.h> // https://github.com/alextaujenis/RBD_Timer
    #include <RBD_Light.h> // https://github.com/alextaujenis/RBD_Light

    RBD::Light light(13);

    void setup() {
      light.blink(250,250);
    }

    void loop() {
      light.update();
    }

#Documentation
##Public Methods
* [constructor(pin)](#constructorpin)
<hr />
* [on()](#on)
* [off()](#off)
* [setPwm(value)](#setpwmvalue)
* [setPwmPercent(value)](#setpwmpercentvalue)
<hr />
* [isOn()](#ison)
* [isOff()](#isoff)
* [getPwm()](#getpwm)
* [getPwmPercent()](#getpwmpercent)
<hr />
* [update()](#update)
* [blink(on\_time, off\_time, times)](#blinkon_time-off_time-times1)
* [fade(up\_time, on\_time, down\_time, off\_time, times)](#fadeup_time-on_time-down_time-off_time-times1)

##constructor(pin)
Create a new light and pass in the Arduino pin number.

    RBD::Light light(13);

    void setup() {
      ...
    }

##on()
Turn on the light.

    void loop() {
      light.on();
    }

##off()
Turn off the light.

    void loop() {
      light.off();
    }

##setPwm(value)
Pass in an integer between 0 - 255 to set the brightness of the light.

    void loop() {
      light.setPwm(200);
    }

##setPwmPercent(value)
Pass in an integer between 0 - 100 to set the brightness of the light.

    void loop() {
      light.setPwmPercent(75);
    }

##isOn()
Returns true if the current light brightness is at 100%.

    void loop() {
      if(light.isOn()) {
        ...
      }
    }

##isOff()
Returns true if the current light brightness is at 0%.

    void loop() {
      if(light.isOff()) {
        ...
      }
    }

##getPwm()
Returns an integer of the current light brightness from 0 - 255.

    void loop() {
      light.getPwm();
    }

##getPwmPercent()
Returns an integer of the current light percentage brightness from 0 - 100.

    void loop() {
      light.getPwmPercent();
    }

##update()
Keep the light moving in real-time. This must be called continuously from within loop() in order to [blink()](#blinkon_time-off_time-times1) or [fade()](#fadeup_time-on_time-down_time-off_time-times1) the light.

    void loop() {
      light.update();
    }

##blink(on\_time, off\_time, times<sup>1</sup>)
Pass in unsigned longs for the time values in milliseconds. Pass in an integer for the last parameter for the number of times the light should blink. The last parameter is optional<sup>1</sup> and can be left out to make the light repeat the blink cycle forever.

* the light will turn on for the on\_time
* then turn off for the off\_time
* and will repeat this cycle for the number of times you provided<sup>1</sup>

Example:

    void setup() {
      light.blink(1000,500,25);
    }

    void loop() {
      light.update();
    }

##fade(up\_time, on\_time, down\_time, off\_time, times<sup>1</sup>)
Pass in unsigned longs for the time values in milliseconds. Pass in an integer for the last parameter for the number of times the light should fade. The last parameter is optional<sup>1</sup> and can be left out to make the light repeat the fade cycle forever.

* the light will fade up according to the up\_time
* stay on for the on\_time
* fade down for the down\_time
* then stay off for the off\_time
* and will repeat this cycle for the number of times your provided<sup>1</sup>

Example:

    void setup() {
      light.fade(2500,250,2000,750,1000);
    }

    void loop() {
      light.update();
    }

#License
This code is available under the [MIT License](http://opensource.org/licenses/mit-license.php).