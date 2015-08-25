#Arduino Light Library
A simple real-time library for controlling many lights. It provides basic control along with timed blinking and pulsing.

#Installation
Download and install this Arduino Light Library along with the Arduino Timer library below:

[Arduino Timer Library (required)](https://github.com/alextaujenis/Timer) <- dependency

The Light Library requires the Timer Library, but you are not required to use the Timer functions in your own sketchs. **Beware:** you need to keep the loop() moving for the Light timed functions to work, and you **can't use delay()**. If you are managing millis() by hand; you should look into using the Timer [library functions](https://github.com/alextaujenis/Timer#arduino-timer-library) in your own sketches to simplify things.


##Example Setup

0. Install this Library and the Timer Library above
0. Load the example sketch on to an Arduino
0. Watch the onboad Arduino LED 13 slowly pulse on and off

##[example.ino](https://github.com/alextaujenis/Light/blob/master/example/example.ino)

    #include <Timer.h>
    #include <Light.h>

    Light light(13);

    void setup() {
      light.pulse(2500,250,2000,750,1000);
    }

    void loop() {
      light.update();
    }

#Documentation
##Public Methods
* [constructor(pin)](#constructorpin)
* [on()](#on)
* [off()](#off)
* [isOn()](#ison)
* [isOff()](#isoff)
* [update()](#update)
* [setPwm(value)](#setpwmvalue)
* [setPwmPercent(value)](#setpwmpercentvalue)
* [getPwm()](#getpwm)
* [getPwmPercent()](#getpwmpercent)
* [blink(on\_time, off\_time, times)](#blinkon_time-off_time-times)
* [pulse(up\_time, on\_time, down\_time, off\_time, times)](#pulseup_time-on_time-down_time-off_time-times)

##constructor(pin)
Pass in an integer for the Arduino pin to create an instance of this class.

    Light light(13);

##on()
Turn on the light.

    light.on();

##off()
Turn off the light.

    light.off();


##isOn()
Returns true if the light is 100% on.

    light.isOn();

##isOff()
Returns true if the light is 0% on.

    light.isOff();

##update()
Keep the light moving in real-time. This must be called continuously from within loop() in order to [blink()](#blinkon_time-off_time-times) or [pulse()](#pulseup_time-on_time-down_time-off_time-times) the light.

    light.update();

##setPwm(value)
Pass in an integer between 0 - 255 to set the brightness of the light.

    light.setPwm(200);

##setPwmPercent(value)
Pass in an integer between 0 - 100 to set the brightness of the light.

    light.setPwmPercent(75);

##getPwm()
Returns an integer of the current light brightness from 0 - 255.

    light.getPwm();

##getPwmPercent()
Returns an integer of the current light percentage brightness from 0 - 100.

    light.getPwmPercent();

##blink(on\_time, off\_time, times)
Pass in integers for the values in milliseconds.

* the light will turn on for the on\_time
* then turn off for the off\_time
* and will repeat this cycle for the number of times you provided

Example:

    // blink on for 1 second
    // off for 1/2 second
    // do this 25 times

    light.blink(1000,500,25);

##pulse(up\_time, on\_time, down\_time, off\_time, times)
Pass in integers for the values in milliseconds.

* the light will fade up according to the up\_time
* stay on for the on\_time
* fade down for the down\_time
* then stay off for the off\_time
* and will repeat this cycle for the number of times your provided

Example:

    // fade up for 2.5 seconds
    // stay on for 1/4 second
    // fade down for 2 seconds
    // stay off for 3/4 second
    // do this 1000 times

    light.pulse(2500,250,2000,750,1000)

#License
This code is available under the [MIT License](http://opensource.org/licenses/mit-license.php).