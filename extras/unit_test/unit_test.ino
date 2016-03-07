// Arduino RBD Light Library v2.1.7 - Unit test coverage.
// https://github.com/alextaujenis/RBD_Light
// Copyright 2016 Alex Taujenis
// MIT License

// Test Setup: Connect Arduino pin 3 directly to pin 2 with a small jumper wire.
// The light library is outputting PWM signals on pin 3, while the test picks up
// signal changes on pin 2. Keep the jumper wire short to ensure test accuracy.
// Also connect Arduino digital pin 23 directly to digital pin 22 if using a Mega 2560.

#include <ArduinoUnit.h> // https://github.com/mmurdoch/arduinounit
#include <RBD_Timer.h>   // https://github.com/alextaujenis/RBD_Timer
#include <RBD_Light.h>   // https://github.com/alextaujenis/RBD_Light

// pwm test light
RBD::Light light(3);       // pwm test send pin
const int receive_pin = 2; // test receive pin

// digital test light
RBD::Light digital_light(23);       // digital test send pin
const int digital_receive_pin = 22; // test receive pin

// pwm test helpers
unsigned long getPulseTime() { return pulseIn(receive_pin, HIGH); }
bool isOn()  { return digitalRead(receive_pin); }
bool isOff() { return !isOn(); }

// digital test helpers
bool digitalIsOn()  { return digitalRead(digital_receive_pin); }
bool digitalIsOff() { return !digitalIsOn(); }

// on
  test(on_should_turn_on_the_light) {
    light.on();
    assertTrue(isOn());
    testCleanup();
  }

  test(on_should_stop_the_light_from_blinking) {
    light.blink(10,10);
    light.update();
    light.on();
    delay(15);
    light.update();
    assertTrue(isOn());
    testCleanup();
  }

  test(on_should_stop_the_light_from_fading) {
    light.fade(10,10,10,10);
    light.update();
    light.on();
    delay(5);
    light.update();
    assertTrue(isOn());
    testCleanup();
  }

// digital on
  test(on_should_turn_on_the_digital_light) {
    digital_light.on();
    assertTrue(digitalIsOn());
    testCleanup();
  }

// off
  test(off_should_turn_off_the_light) {
    light.on();
    light.off();
    assertTrue(isOff());
    testCleanup();
  }

  test(off_should_stop_the_light_from_blinking) {
    light.blink(10,10);
    light.update();
    light.off();
    delay(5);
    light.update();
    assertTrue(isOff());
    testCleanup();
  }

  test(off_should_stop_the_light_from_fading) {
    light.fade(10,10,10,10);
    light.update();
    light.off();
    delay(15);
    light.update();
    assertTrue(isOff());
    testCleanup();
  }

// digital off
  test(off_should_turn_off_the_digital_light) {
    digital_light.on();
    digital_light.off();
    assertTrue(digitalIsOff());
    testCleanup();
  }

// isOn
  test(isOn_should_return_true_when_on) {
    light.on();
    assertTrue(light.isOn());
    testCleanup();
  }

  test(isOn_should_return_false_when_off) {
    light.off();
    assertFalse(light.isOn());
    testCleanup();
  }

  test(isOn_should_return_true_when_setBrightness_is_255) {
    light.setBrightness(255);
    assertTrue(light.isOn());
    testCleanup();
  }

  test(isOn_should_return_false_when_setBrightness_is_254) {
    light.setBrightness(254);
    assertFalse(light.isOn());
    testCleanup();
  }

  test(isOn_should_return_false_when_setBrightness_is_0) {
    light.setBrightness(0);
    assertFalse(light.isOn());
    testCleanup();
  }

  test(isOn_should_return_true_when_setBrightnessPercent_is_100) {
    light.setBrightnessPercent(100);
    assertTrue(light.isOn());
    testCleanup();
  }

  test(isOn_should_return_false_when_setBrightnessPercent_is_99) {
    light.setBrightnessPercent(99);
    assertFalse(light.isOn());
    testCleanup();
  }

  test(isOn_should_return_false_when_setBrightnessPercent_is_0) {
    light.setBrightnessPercent(0);
    assertFalse(light.isOn());
    testCleanup();
  }

// digital isOn
  test(isOn_should_return_true_when_digital_light_is_on) {
    digital_light.on();
    assertTrue(digital_light.isOn());
    testCleanup();
  }

// isOff
  test(isOff_should_return_true_when_off) {
    light.off();
    assertTrue(light.isOff());
    testCleanup();
  }

  test(isOff_should_return_false_when_on) {
    light.on();
    assertFalse(light.isOff());
    testCleanup();
  }

  test(isOff_should_return_true_when_setBrightness_is_0) {
    light.setBrightness(0);
    assertTrue(light.isOff());
    testCleanup();
  }

  test(isOff_should_return_false_when_setBrightness_is_1) {
    light.setBrightness(1);
    assertFalse(light.isOff());
    testCleanup();
  }

  test(isOff_should_return_false_when_setBrightness_is_255) {
    light.setBrightness(255);
    assertFalse(light.isOff());
    testCleanup();
  }

  test(isOff_should_return_true_when_setBrightnessPercent_is_0) {
    light.setBrightnessPercent(0);
    assertTrue(light.isOff());
    testCleanup();
  }

  test(isOff_should_return_false_when_setBrightnessPercent_is_1) {
    light.setBrightnessPercent(1);
    assertFalse(light.isOff());
    testCleanup();
  }

  test(isOff_should_return_false_when_setBrightnessPercent_is_100) {
    light.setBrightnessPercent(100);
    assertFalse(light.isOff());
    testCleanup();
  }

// digital isOff
  test(isOff_should_return_true_when_digital_light_is_off) {
    digital_light.on();
    digital_light.off();
    assertTrue(light.isOff());
    testCleanup();
  }

// update: covered in blink and fade tests

// setBrightness
  unsigned long pulse1 = 0;
  unsigned long pulse2 = 0;

  void assertWithinTolerance(unsigned long value1, unsigned long value2, int tolerance_percent) {
    // +/- tolerance percent
    assertLessOrEqual(value1, (unsigned long)(value2 * float(1 + tolerance_percent / 100.0)));
    assertMoreOrEqual(value1, (unsigned long)(value2 * float(1 - tolerance_percent / 100.0)));
  }

  test(setBrightness_should_increase_the_brightness) {
    light.setBrightness(1);
    pulse1 = getPulseTime();
    light.setBrightness(254);
    pulse2 = getPulseTime();
    assertMore(pulse2, pulse1);
    testCleanup();
  }

  test(setBrightness_should_decrease_the_brightness) {
    light.setBrightness(254);
    pulse1 = getPulseTime();
    light.setBrightness(1);
    pulse2 = getPulseTime();
    assertMore(pulse1, pulse2);
    testCleanup();
  }

  test(setBrightness_should_set_the_brightness) {
    light.setBrightness(25);
    pulse1 = getPulseTime();
    light.setBrightness(50);
    pulse2 = getPulseTime();
    assertWithinTolerance(pulse1 * 2, pulse2, 10);
    testCleanup();
  }

  test(setBrightness_of_0_should_turn_off_the_light) {
    light.on();
    light.setBrightness(0);
    assertTrue(isOff());
    testCleanup();
  }

  test(setBrightness_should_constrain_to_zero) {
    light.on();
    light.setBrightness(-1);
    assertEqual(light.getBrightness(), 0)
    assertTrue(isOff());
    testCleanup();
  }

  test(setBrightness_should_constrain_to_255) {
    light.off();
    light.setBrightness(256);
    assertEqual(light.getBrightness(), 255)
    assertTrue(isOn());
    testCleanup();
  }

  test(setBrightness_of_255_should_turn_on_the_light) {
    light.off();
    light.setBrightness(255);
    assertTrue(isOn());
    testCleanup();
  }

  test(setBrightness_should_change_getBrightness) {
    light.setBrightness(123);
    assertEqual(light.getBrightness(), 123);
    testCleanup();
  }

  test(setBrightness_should_change_getBrightnessPercent) {
    light.setBrightness(51);
    assertEqual(light.getBrightnessPercent(), 20);
    testCleanup();
  }

  test(setBrightness_should_stop_the_light_from_blinking) {
    // calibrate
    light.setBrightness(123);
    pulse1 = getPulseTime();
    light.off();
    // start test
    light.blink(10,10);
    light.update();
    light.setBrightness(123);
    delay(15);
    light.update();
    assertWithinTolerance(getPulseTime(), pulse1, 15);
    testCleanup();
  }

  test(setBrightness_should_stop_the_light_from_fading) {
    // calibrate
    light.setBrightness(123);
    pulse1 = getPulseTime();
    light.off();
    // start test
    light.fade(10,10,10,10);
    light.update();
    light.setBrightness(123);
    delay(35);
    light.update();
    assertWithinTolerance(getPulseTime(), pulse1, 15);
    testCleanup();
  }

// digital setBrightness
// Apparently: analogWrite(digital_pin, 127) is the same as digitalWrite(digital_pin, LOW)
// and analogWrite(digital_pin, 128) is the same as digitalWrite(digital_pin, HIGH)
// as documented through tests run on an Arduino Mega 2560, and verified in wiring_analog.c
  test(setBrightness_should_turn_on_the_digital_light_at_255) {
    digital_light.setBrightness(255);
    assertTrue(digitalIsOn());
    testCleanup();
  }

  test(setBrightness_should_turn_on_the_digital_light_at_228) {
    digital_light.setBrightness(228);
    assertTrue(digitalIsOn());
    testCleanup();
  }

  test(setBrightness_should_turn_on_the_digital_light_at_128) {
    digital_light.setBrightness(128);
    assertTrue(digitalIsOn());
    testCleanup();
  }

  test(setBrightness_should_turn_off_the_digital_light_at_127) {
    digital_light.setBrightness(127);
    assertTrue(digitalIsOff());
    testCleanup();
  }

  test(setBrightness_should_turn_off_the_digital_light_at_27) {
    digital_light.setBrightness(27);
    assertTrue(digitalIsOff());
    testCleanup();
  }

  test(setBrightness_should_turn_off_the_digital_light_at_0) {
    digital_light.setBrightness(0);
    assertTrue(digitalIsOff());
    testCleanup();
  }

// setBrightnessPercent
  test(setBrightnessPercent_should_increase_the_brightness) {
    light.setBrightnessPercent(1);
    pulse1 = getPulseTime();
    light.setBrightnessPercent(99);
    pulse2 = getPulseTime();
    assertMore(pulse2, pulse1);
    testCleanup();
  }

  test(setBrightnessPercent_should_decrease_the_brightness) {
    light.setBrightnessPercent(99);
    pulse1 = getPulseTime();
    light.setBrightnessPercent(1);
    pulse2 = getPulseTime();
    assertMore(pulse1, pulse2);
    testCleanup();
  }

  test(setBrightnessPercent_should_set_the_brightness) {
    light.setBrightnessPercent(25);
    pulse1 = getPulseTime();
    light.setBrightnessPercent(50);
    pulse2 = getPulseTime();
    assertWithinTolerance(pulse1 * 2, pulse2, 5);
    testCleanup();
  }

  test(setBrightnessPercent_of_0_should_turn_off_the_light) {
    light.on();
    light.setBrightnessPercent(0);
    assertTrue(isOff());
    testCleanup();
  }

  test(setBrightnessPercent_of_100_should_turn_on_the_light) {
    light.off();
    light.setBrightnessPercent(100);
    assertTrue(isOn());
    testCleanup();
  }

  test(setBrightnessPercent_should_change_getBrightness) {
    light.setBrightnessPercent(20);
    assertEqual(light.getBrightness(), 51);
    testCleanup();
  }

  test(setBrightnessPercent_should_change_getBrightnessPercent) {
    light.setBrightnessPercent(20);
    assertEqual(light.getBrightnessPercent(), 20);
    testCleanup();
  }

  test(setBrightnessPercent_should_stop_the_light_from_blinking) {
    // calibrate
    light.setBrightnessPercent(35);
    pulse1 = getPulseTime();
    light.off();
    // start test
    light.blink(10,10);
    light.update();
    light.setBrightnessPercent(35);
    delay(15);
    light.update();
    assertWithinTolerance(getPulseTime(), pulse1, 15);
    testCleanup();
  }

  test(setBrightnessPercent_should_stop_the_light_from_fading) {
    // calibrate
    light.setBrightnessPercent(35);
    pulse1 = getPulseTime();
    light.off();
    // start test
    light.fade(10,10,10,10);
    light.update();
    light.setBrightnessPercent(35);
    delay(35);
    light.update();
    assertWithinTolerance(getPulseTime(), pulse1, 15);
    testCleanup();
  }

// getBrightness
  test(getBrightness_returns_the_brightness) {
    light.setBrightness(123);
    assertEqual(light.getBrightness(), 123);
    testCleanup();
  }

  test(getBrightness_returns_a_changed_brightness) {
    light.setBrightness(123);
    light.setBrightness(124);
    assertEqual(light.getBrightness(), 124);
    testCleanup();
  }

// getBrightnessPercent
  test(getBrightnessPercent_returns_the_brightness_percent) {
    light.setBrightness(51);
    assertEqual(light.getBrightnessPercent(), 20);
    testCleanup();
  }

  test(getBrightnessPercent_returns_a_changed_brightness_percent) {
    light.setBrightness(41);
    light.setBrightness(51);
    assertEqual(light.getBrightnessPercent(), 20);
    testCleanup();
  }

// blink
  test(blink_should_turn_the_light_on_and_off) {
    light.blink(10,10,1);
    // blink 1
    light.update();
    assertTrue(isOn());
    delay(11);
    light.update();
    assertFalse(isOn());
    delay(11);
    // validate finished
    light.update();
    assertFalse(isOn());
    // cleanup
    testCleanup();
  }

  test(blink_should_turn_the_light_on_and_off_multiple_times) {
    light.blink(10,10,2);
    // blink 1
    light.update();
    assertTrue(isOn());
    delay(11);
    light.update();
    assertFalse(isOn());
    delay(11);
    // blink 2
    light.update();
    assertTrue(isOn());
    delay(11);
    light.update();
    assertFalse(isOn());
    // validate finished
    delay(11);
    light.update();
    assertFalse(isOn());
    // cleanup
    testCleanup();
  }

  test(blink_zero_times_should_not_turn_the_light_on) {
    light.blink(10,10,0);
    light.update();
    assertTrue(isOff());
    // cleanup
    testCleanup();
  }

// blink: overloaded constructor
  test(overloaded_blink_should_turn_the_light_on_and_off_forever) {
    light.blink(10,10);
    // blink 1
    light.update();
    assertTrue(isOn());
    delay(11);
    light.update();
    assertFalse(isOn());
    delay(11);
    // blink 2
    light.update();
    assertTrue(isOn());
    delay(11);
    light.update();
    assertFalse(isOn());
    delay(11);
    // blink 3
    light.update();
    assertTrue(isOn());
    delay(11);
    light.update();
    assertFalse(isOn());
    // cleanup
    testCleanup();
  }

// digital blink
  test(blink_should_turn_the_digital_light_on_and_off_multiple_times) {
    digital_light.blink(10,10,2);
    // blink 1
    digital_light.update();
    assertTrue(digitalIsOn());
    delay(11);
    digital_light.update();
    assertFalse(digitalIsOn());
    delay(11);
    // blink 2
    digital_light.update();
    assertTrue(digitalIsOn());
    delay(11);
    digital_light.update();
    assertFalse(digitalIsOn());
    // validate finished
    delay(11);
    digital_light.update();
    assertFalse(digitalIsOn());
    // cleanup
    testCleanup();
  }

// GitHub issue #4 bug fix
// https://github.com/alextaujenis/RBD_Light/issues/4
  test(blink_should_turn_on_the_light_after_calling_setBrightness) {
    light.setBrightness(128);
    light.blink(100,100);
    light.update();
    assertTrue(isOn());
    testCleanup();
  }

// fade
  test(fade_should_ramp_the_light_up_and_down) {
    // calibrate pulse time
    light.setBrightnessPercent(50);
    pulse1 = getPulseTime();
    // fade the light
    light.fade(100,100,100,100,1);
    light.update();
    delay(50);
    light.update();
    assertWithinTolerance(getPulseTime(),pulse1,15);
    delay(65);
    light.update();
    assertTrue(isOn());
    delay(135);
    light.update();
    assertWithinTolerance(getPulseTime(),pulse1,15);
    delay(51);
    light.update();
    assertTrue(isOff());
    testCleanup();
  }

  test(fade_zero_times_should_not_ramp_the_light) {
    // fade the light
    light.fade(100,100,100,100,0);
    light.update();
    delay(50);
    light.update();
    delay(65);
    light.update();
    assertTrue(isOff());
    delay(135);
    light.update();
    delay(51);
    light.update();
    assertTrue(isOff());
    testCleanup();
  }

// fade: overloaded constructor
  test(overloaded_fade_should_ramp_the_light_up_and_down_forever) {
    // calibrate pulse time
    light.setBrightnessPercent(50);
    pulse1 = getPulseTime();
    // fade the light
    light.fade(100,100,100,100);
    light.update();
    delay(50);
    light.update();
    assertWithinTolerance(getPulseTime(),pulse1,15);
    delay(65);
    light.update();
    assertTrue(isOn());
    delay(135);
    light.update();
    assertWithinTolerance(getPulseTime(),pulse1,15);
    delay(51);
    light.update();
    assertTrue(isOff());
    delay(149);
    light.update();
    assertWithinTolerance(getPulseTime(),pulse1,15);
    delay(200);
    light.update();
    assertWithinTolerance(getPulseTime(),pulse1,15);
    testCleanup();
  }

// setup and run tests
void setup() {
  testSetup();
  Serial.begin(115200);
  while(!Serial);
}

void loop() {
  Test::run();
}

void testSetup() {
  pinMode(receive_pin, INPUT);
  pinMode(digital_receive_pin, INPUT);
}

void testCleanup() {
  light.off();
  digital_light.off();
}