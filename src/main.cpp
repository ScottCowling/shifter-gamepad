#include <Arduino.h>
#include <Adafruit_DotStar.h>
#include <HID-Project.h>
#include <HID-Settings.h>



///////////
//OPTIONS//
///////////
#define TARGET_REPORT_RATE     125                    //Set the rate the device will report back to the USB host.

#define DEBUG_MODE                                    //Enable debug logging via Serial.
#define WAIT_FOR_DEBUG                                //Wait for a Serial connection before initializing.

#define DOTSTAR_STATUS_LED_ENABLE                     //Enable RGB DotStar as an led indicator for button presses.
#define DOTSTAR_STATUS_LED_BRIGHTNESS 255             //DotStar brightness (0-255).
#define DOTSTAR_STATUS_LED_DATA       7               //DotStar data pin.
#define DOTSTAR_STATUS_LED_CLK        8               //DotStar clock pin.
#define DOTSTAR_STATUS_LED_COUNT      1               //DotStar single led on Trinket M0.
#define DOTSTAR_STATUS_LED_FORMAT     DOTSTAR_BRG     //DotStar colour format e.g. RGB, BRG, BGR.



///////////
//DOTSTAR//
///////////
#ifdef DOTSTAR_STATUS_LED_ENABLE
int buttonColours[5] = { 0xFF0000, 0x00FF00, 0x0000FF, 0xFFFF00, 0x00FFFF };
Adafruit_DotStar led(DOTSTAR_STATUS_LED_COUNT, DOTSTAR_STATUS_LED_DATA, DOTSTAR_STATUS_LED_CLK, DOTSTAR_STATUS_LED_FORMAT);
#endif

void dotstar_init() {
#ifdef DOTSTAR_STATUS_LED_ENABLE
  led.begin();
  led.setBrightness(DOTSTAR_STATUS_LED_BRIGHTNESS);
  led.show();
#endif
}



//////////
//STATUS//
//////////
void status_init() {
  pinMode(LED_BUILTIN, OUTPUT);
}
void status_on() {
  digitalWrite(LED_BUILTIN, HIGH);
}
void status_off() {
  digitalWrite(LED_BUILTIN, LOW);
}
void status_wait_blink() {
  status_off();
  delay(250);
  status_on();
}



/////////
//DEBUG//
/////////
#ifdef DEBUG_MODE
#ifdef WAIT_FOR_DEBUG
#pragma message "WARNING: BUILDING WITH DEBUG TOOLING ENABLED!"
#endif
#endif

void debug_init() {
#ifdef DEBUG_MODE
  Serial.begin(115200);
#ifdef WAIT_FOR_DEBUG
  while (!Serial) { status_wait_blink(); }
#endif
#endif
}



///////////
//GAMEPAD//
///////////
int buttonCount = 3;
byte buttonPins[5] = { 0, 1, 2, 3, 4 };
bool buttonLastStates[5] = { false, false, false, false, false };

void gamepad_init() {
  for (int i = 0; i < buttonCount; i++) {
    pinMode(i, INPUT_PULLUP);
  }
  Gamepad.begin();
}


/////////////
//EXECUTION//
/////////////
unsigned long currentReportTime = 0;
unsigned long targetReportTime = 0;
unsigned long reportCounter = 0;

void setup() {
  status_init();
  status_on();

  debug_init();

  gamepad_init();

  dotstar_init();

  targetReportTime = 1000 / TARGET_REPORT_RATE;
  
  status_off(); //Signal boot completed by turning off light.
  delay(250); //Delay to make finish visible before gamepad takes over and a switch may set LED solid.
}

void loop() {
  unsigned long reportTimeDifference = millis() - currentReportTime;
  int reportDelay = targetReportTime - reportTimeDifference;
  if (reportDelay > 0) {
    delay(reportDelay);
  } else {
    Serial.print("REPORTED ");
    Serial.print(reportTimeDifference);
    Serial.println("ms LATE!");
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
#ifdef DOTSTAR_STATUS_LED_ENABLE
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
#ifdef DOTSTAR_STATUS_LED_ENABLE
      led.clear();
      led.show();
#endif
      Gamepad.release(i + 1);
      buttonLastStates[i] = false;
    }
  }

  Gamepad.write(); //Write gamepad state.
}