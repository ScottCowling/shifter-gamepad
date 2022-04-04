#include <Arduino.h>
#include <Adafruit_DotStar.h>
#include <HID-Project.h>
#include <HID-Settings.h>

//Set the rate the device will report back to the USB host. 
#define TARGET_REPORT_RATE 125

//Enable debug logging via Serial.
#define DEBUG_MODE

//Wait for a Serial connection before initializing.
#define WAIT_FOR_DEBUG

//Enables RGB led indicator for button presses.
#define LED_ENABLE

#ifdef LED_ENABLE
#define LED_BRIGHTNESS  255
#define LED_NUMPIXELS   1
#define LED_DATA_PIN    7
#define LED_CLK_PIN     8
int buttonColours[5] = { 0xFF0000, 0x00FF00, 0x0000FF, 0xFFFF00, 0x00FFFF };
Adafruit_DotStar led(LED_NUMPIXELS, LED_DATA_PIN, LED_CLK_PIN, DOTSTAR_BRG);
#endif

#define SYS_LED_PIN LED_BUILTIN




int buttonCount = 5;
int buttonPins[5] = { 0, 1, 2, 3, 4 };
bool buttonLastStates[5] = { false, false, false, false, false };

unsigned long currentReportTime = 0;
unsigned long targetReportTime = 0;
unsigned long reportCounter = 0;

void setup() {
  pinMode(SYS_LED_PIN, OUTPUT); 
  digitalWrite(SYS_LED_PIN, HIGH); //Signal start of boot process on LED_BUILTIN

#ifdef DEBUG_MODE
  #pragma message "BUILDING WITH DEBUG ENABLED"
  Serial.begin(115200);
#ifdef WAIT_FOR_DEBUG
  #pragma message "BUILD WILL WAIT FOR SERIAL"
  while (!Serial) { }
#endif
#endif

  Gamepad.begin();
  for (int i = 0; i < buttonCount; i++) {
    pinMode(i, INPUT_PULLUP);
  }

#ifdef LED_ENABLE
 #pragma message "BUILD WITH LED DIAGNOSTIC"
  led.begin();
  led.setBrightness(LED_BRIGHTNESS);
  led.show();
#endif

  targetReportTime = 1000 / TARGET_REPORT_RATE;
  
  digitalWrite(SYS_LED_PIN, LOW); //Signal end of boot process on LED_BUILTIN
}

void loop() {
  unsigned long reportTimeDifference = millis() - currentReportTime;
  int reportDelay = targetReportTime - reportTimeDifference;
  if (reportDelay > 0) {
    delay(reportDelay);
  } else {
    //Serial.print("REPORT LAG: "); 
    //Serial.print(reportDelay * -1); 
    //Serial.println("ms longer than allocated!"); 
    Serial.print("SLOW REPORTING. ");
    Serial.print(1000 / reportTimeDifference);
    Serial.println("hz");
  }
  currentReportTime = millis();
  reportCounter++;

  if (reportCounter % TARGET_REPORT_RATE == 0) {
    Serial.print("RC ");
    Serial.println(reportCounter / TARGET_REPORT_RATE);
  }

  for (int i = 0; i < buttonCount; i++) {
    if (!digitalRead(i)) {
#ifdef DEBUG_MODE
      if (!buttonLastStates[i]) {
        Serial.print("+");
        Serial.println(i);
      }
      buttonLastStates[i] = true;
#endif

      Gamepad.press(i + 1);
#ifdef LED_ENABLE
      led.setPixelColor(0, buttonColours[i]);
      led.show();
#endif
    } else {
#ifdef DEBUG_MODE
      if (buttonLastStates[i]) {
        Serial.print("-");
        Serial.println(i);
      }
#endif
#ifdef LED_ENABLE
      led.clear();
      led.show();
#endif
      Gamepad.release(i + 1);
      buttonLastStates[i] = false;
    }
  }
  Gamepad.write(); //Write gamepad state.
}