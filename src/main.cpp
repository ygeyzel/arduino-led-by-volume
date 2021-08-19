#include "Arduino.h"
#include "FastLED.h"
#include "arduino-timer.h"

#define LED_PER_STRIP 7
#define LED_STRIPS_NUM 4
#define LED_NUM (LED_PER_STRIP * LED_STRIPS_NUM)
#define DPIN 8
#define CPIN 9

#define AUDIO_AIN A5
#define MAX_ANALOG_VAL 1023
#define ANALOG_MAX_DELTA 150

#define HUE_FACTOR 20
#define HUE_TIMER 200

CRGB g_led[LED_NUM];
int g_hue = 0;
auto g_timer = timer_create_default();

// Changing the strips colors
bool hue_rotate(void *) {
  g_hue = (g_hue + HUE_FACTOR) % 256;
  return true;
}

void setup() {
  FastLED.addLeds<WS2801, DPIN, CPIN, RGB>(g_led, LED_NUM);
  g_timer.every(HUE_TIMER, hue_rotate);
}

void loop() {
  g_timer.tick();
  int volume = analogRead(AUDIO_AIN);

  volume = abs(volume - MAX_ANALOG_VAL / 2);
  volume =
      map(volume, 0, MAX_ANALOG_VAL / 2 - ANALOG_MAX_DELTA, 0, LED_PER_STRIP);

  for (int i = 0; i < LED_PER_STRIP; ++i) {
    for (int j = 0; j < LED_STRIPS_NUM; ++j) {
      int hue = (g_hue + HUE_FACTOR * j) % 256;
      g_led[i + j * LED_PER_STRIP] =
          i < volume ? CHSV(hue, 255, 125) : CHSV(0, 0, 0);
    }
  }
  FastLED.show();
  delay(5);
}
