#include "Arduino.h"
#include "FastLED.h"

#define LED_PER_STRIP 7
#define LED_STRIPS_NUM 4
#define LED_NUM (LED_PER_STRIP * LED_STRIPS_NUM)
#define DPIN 8
#define CPIN 9

#define AUDIO_IN A5
#define MAX_ANALOG_VAL 1023

CRGB g_led[LED_NUM];

void setup() { FastLED.addLeds<WS2801, DPIN, CPIN, RGB>(g_led, LED_NUM); }

void loop() {
  int volume = analogRead(AUDIO_IN);

  volume -= abs(MAX_ANALOG_VAL / 2);
  volume = map(volume, 0, MAX_ANALOG_VAL / 2, 0, LED_PER_STRIP);

  for (int i = 0; i < LED_PER_STRIP; ++i) {
    for (int j = 0; j < LED_STRIPS_NUM; ++j) {
      g_led[i + j * LED_PER_STRIP] = i < volume ? CRGB::Red : CRGB::Black;
    }
  }
  FastLED.show();
  delay(5);
}
