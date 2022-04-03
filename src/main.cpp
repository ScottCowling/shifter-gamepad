#include <Arduino.h>
#include <Adafruit_DotStar.h>
#include <HID-Project.h>
#include <HID-Settings.h>

//#define DEBUG_MODE
#define LED_ENABLE

#ifdef LED_ENABLE
#define LED_BRIGHTNESS 255
#define LED_NUMPIXELS 1
#define LED_DATA_PIN    7
#define LED_CLK_PIN   8
int buttonColours[5] = { 0xFF0000, 0x00FF00, 0x0000FF, 0xFFFF00, 0x00FFFF };
Adafruit_DotStar led(LED_NUMPIXELS, LED_DATA_PIN, LED_CLK_PIN, DOTSTAR_BRG);
#endif

/*
  #define SYS_LED_PIN = LED_BUILTIN;
  pinMode(SYS_LED_PIN, OUTPUT);
  digitalWrite(SYS_LED_PIN, HIGH);
  digitalWrite(SYS_LED_PIN, LOW);
*/

int buttonCount = 5;
int buttonPins[5] = { 0, 1, 2, 3, 4 } ;
bool buttonLastStates[5] = { false, false, false, false, false };

void setup() {
#ifdef LED_ENABLE
  led.begin();
  led.setBrightness(LED_BRIGHTNESS);
  led.show();
#endif

  Gamepad.begin();
  for (int i = 0; i < buttonCount; i++) {
    pinMode(i, INPUT_PULLUP);
  }
  
#ifdef DEBUG_MODE
  Serial.begin(9600);
  while (!Serial) { }
#endif
}

void loop() {
  led.clear();
  for (int i = 0; i < buttonCount; i++) {
    if (!digitalRead(i)) {
#ifdef DEUBG_MODE
      if (!buttonLastStates[i]) {
        Serial.print("PRESSED ");
        Serial.println(i);
      }
#endif
      Gamepad.press(i + 1);
      buttonLastStates[i] = true;
#ifdef LED_ENABLE
      led.setPixelColor(0, buttonColours[i]);
#endif
    } else {
#ifdef DEUBG_MODE
      if (buttonLastStates[i]) {
        Serial.print("RELEASED ");
        Serial.println(i);
      }
     - 
#endif
      Gamepad.release(i + 1);
      buttonLastStates[i] = false;
    }
  }
  Gamepad.write();
  led.show();
}