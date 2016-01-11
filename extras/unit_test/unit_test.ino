// Arduino RBD Light Library v2.1.3 - Unit test coverage.
// https://github.com/alextaujenis/RBD_Light
// Copyright 2016 Alex Taujenis
// MIT License

// Test Setup: Connect Arduino pin 3 directly to pin 5 with a small jumper wire.
// The light library is outputting PWM signals on pin 3, while the test picks up
// signal changes on pin 5. Keep the jumper wire short to ensure test accuracy.

#include <ArduinoUnit.h> // https://github.com/mmurdoch/arduinounit
#include <RBD_Timer.h>   // https://github.com/alextaujenis/RBD_Timer
#include <RBD_Light.h>   // https://github.com/alextaujenis/RBD_Light

RBD::Light light(3);       // pin 3 (test send pin)
const int receive_pin = 5; // pin 5 (test receive pin)

// test helpers
unsigned long getPulseTime() {
  return pulseIn(receive_pin, HIGH);
}

bool isOn() {
  return digitalRead(receive_pin);
}

bool isOff() {
  return !isOn();
}

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
  pinMode(receive_pin, INPUT); // receive signals on pin 5
}

void testCleanup() {
  light.off();
  light.update();
}