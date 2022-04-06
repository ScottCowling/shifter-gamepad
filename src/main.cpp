#include <Arduino.h>
#include <Adafruit_DotStar.h>
#include <HID-Project.h>
#include <HID-Settings.h>
#include <hid.h>

PluggableUSBModule


#define FIRMWARE_NAME "shifter-gamepad"
#define FIRMWARE_VERSION "v0.1.0 alpha"



#define TARGET_REPORT_RATE            125             //Set the rate the device will report back to the USB host. Remove definition to disable (uncapped).

#define DEBUG_MODE                                    //Enable debug logging via Serial.
#define WAIT_FOR_SERIAL                               //Wait for a Serial connection before initializing.
#define SERIAL_BAUD_RATE              115200          //Serial baud rate for debugging.

#define DOTSTAR_STATUS_LED                            //Enable RGB DotStar as an led indicator for button presses. Remove definition to disable.
#define DOTSTAR_STATUS_LED_BRIGHTNESS 125             //DotStar brightness (0-255). Lower brightness is better for long durations to increase time-to-failure.
#define DOTSTAR_STATUS_LED_DATA       7               //DotStar data pin.
#define DOTSTAR_STATUS_LED_CLK        8               //DotStar clock pin.
#define DOTSTAR_STATUS_LED_COUNT      1               //DotStar single led on Trinket M0.
#define DOTSTAR_STATUS_LED_FORMAT     DOTSTAR_BRG     //DotStar colour format e.g. RGB, BRG, BGR.



byte buttonCount = 5;
byte buttonPins[5] = { 0, 1, 2, 3, 4 };
bool buttonLastStates[5] = { false, false, false, false, false };

#ifdef DOTSTAR_STATUS_LED
  Adafruit_DotStar star(DOTSTAR_STATUS_LED_COUNT, DOTSTAR_STATUS_LED_DATA, DOTSTAR_STATUS_LED_CLK, DOTSTAR_STATUS_LED_FORMAT);
#endif



///////////
//PROGRAM//
///////////
#ifdef DEBUG_MODE
  #ifdef WAIT_FOR_SERIAL
    #pragma message "WARNING: BUILDING WITH DEBUG WAIT_FOR_SERIAL ENABLED!"
  #endif
#endif

void dotstar_init() {
  #ifdef DOTSTAR_STATUS_LED
  star.begin();
  star.setBrightness(DOTSTAR_STATUS_LED_BRIGHTNESS);
  star.show();
  #endif
}

void dotstar_update() {
  #ifdef DOTSTAR_STATUS_LED
  star.clear();
  star.show();
  #endif
}



//////////
//TIMING//
//////////
unsigned long currentReportTime = 0;
unsigned long targetReportTime = 0;
unsigned long reportCounter = 0;

void timing_init() {
  targetReportTime = 1000 / TARGET_REPORT_RATE;
}

void timing_calculate() {
  #ifdef TARGET_REPORT_RATE
  unsigned long reportTimeDifference = millis() - currentReportTime;
  int reportDelay = targetReportTime - reportTimeDifference;
  if (reportDelay > 0) {
    delay(reportDelay);
  } else {
    Serial.print("LATE ");
    Serial.print(reportTimeDifference);
    Serial.println("ms");
  }
  currentReportTime = millis();
  reportCounter++;

  if (reportCounter % TARGET_REPORT_RATE == 0) {
    Serial.print("REPORTS ");
    Serial.println(reportCounter);
  }
  #endif
}



void setup() {
  pinMode(LED_BUILTIN, OUTPUT);               //Set LED_BUILTIN to OUTPUT.
  digitalWrite(LED_BUILTIN, HIGH);            //Signal start of boot process by turning on LED_BUILTIN.

  #ifdef DEBUG_MODE
    Serial.begin(SERIAL_BAUD_RATE);           //Initialize Serial because DEBUG_MODE is enabled.
  #ifdef WAIT_FOR_SERIAL
    while (!Serial) {                         //Wait for Serial because WAIT_FOR_SERIAL is enabled. Blink LED_BUILTIN to show waiting for Serial.
      digitalWrite(LED_BUILTIN, LOW);         
      delay(350);
      digitalWrite(LED_BUILTIN, HIGH);
      delay(150);
    }
  #endif
    Serial.println("--DEBUG INFO--");         //Print debug information block at the start of Serial initialization.
    Serial.println(FIRMWARE_NAME);            //Firmware name.
    Serial.println(FIRMWARE_VERSION);         //Firmware version.
    Serial.print("target report rate: ");     //Firmware target report rate.       
    #ifdef TARGET_REPORT_RATE
      Serial.print(TARGET_REPORT_RATE);
      Serial.println("hz");
    #else
      Serial.println("uncapped");
    #endif
    #ifdef DOTSTAR_STATUS_LED
      Serial.println("dotstar: enabled");
    #else
      Serial.println("dotstar: disabled");
    #endif
    //TODO: print button config to serial in this startup block. e.g pin, btn number, inverted?, mode/type.
  #endif

  timing_init();                              
  
  dotstar_init();                             //Initialize DotStar for indicating state.

  for (int i = 0; i < buttonCount; i++) {     //Initialize gamepad button pins to INPUT_PULLUP.
    pinMode(i, INPUT_PULLUP);
  }
  Gamepad.begin();                            //Initialize gamepad HID.
  
  digitalWrite(LED_BUILTIN, LOW);             //Signal end of boot process by turning off LED_BUILTIN.
}

bool updateButton(byte pin) {
  if (!digitalRead(pin)) {  
#ifdef DEBUG_MODE
      if (!buttonLastStates[pin]) {
        Serial.print("+");
        Serial.println(pin);
      }
      buttonLastStates[pin] = true;
#endif
      Gamepad.press(pin + 1);
#ifdef DOTSTAR_STATUS_LED
      star.setPixelColor(0, star.ColorHSV((32565 / buttonCount) * pin));
      star.show();
#endif
      return true;
    } else {
#ifdef DEBUG_MODE
      if (buttonLastStates[pin]) {
        Serial.print("-");
        Serial.println(pin);
      }
#endif
      buttonLastStates[pin] = false;
      Gamepad.release(pin + 1);
      return false;
    }
}

void loop() {
  timing_calculate();
  bool anyButtonPressed = false;
  //Iterate pins and update button states.
  for (int i = 0; i < buttonCount; i++) {
    if (updateButton(buttonPins[i])) {
      anyButtonPressed = true;
    }
  }

  //Set DotStar colour if any button is triggered.
  dotstar_update();
  
  //Set LED_BUILTIN if any button is pressed.
  digitalWrite(LED_BUILTIN, (anyButtonPressed) ? HIGH : LOW);

  //Write gamepad state.
  Gamepad.write();                                
}