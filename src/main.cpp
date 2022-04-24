#include <Arduino.h>
#include <Adafruit_DotStar.h>
#include <HID-Project.h>
#include <HID-Settings.h>
//#include <hid.h>
#include <HID.h>

#define FIRMWARE_NAME "shifter-gamepad"
#define FIRMWARE_VERSION "v0.1.0 alpha"

//TODO: Make DotStar an optional function via a preprocessor #def.
//TODO: Track time to produce target report rate and if late when in debug mode.

#define TARGET_REPORT_RATE 1000          //Set the rate the device will report back to the USB host per second. Remove definition to disable (uncapped).

#define BUTTON_COUNT       5            //How many buttons do you have?

#define DEBUG_ENABLE                    //Compile with debug code enabled (Serial printing of button presses and timings). This will likely reduce performance of reporting significantly.
//#define WAIT_FOR_SERIAL                 //Wait for Serial communication to begin before initializing the gamepad.
#define SERIAL_BAUD        115200       //Serial baud rate for debug serial.

#define DOTSTAR_BRIGHTNESS 64           //DotStar brightness (0-255). Lower brightness is better for long durations to increase time-to-failure.
#define DOTSTAR_DATA       7            //DotStar data pin.
#define DOTSTAR_CLK        8            //DotStar clock pin.
#define DOTSTAR_COUNT      1            //DotStar single led on Trinket M0.
#define DOTSTAR_FORMAT     DOTSTAR_BRG  //DotStar colour format e.g. RGB, BRG, BGR.

int pressedButtons = 0;
uint32_t buttonStates = 0;
byte gamepadPinMap[BUTTON_COUNT] = { 0, 1, 2, 3, 4  };                                          //Specify the digital pins your buttons will operate from.
byte gamepadButtonMap[BUTTON_COUNT] = { 1, 2, 3, 4, 5 };                                        //Specify the gamepad "buttons" that will be triggered for each input pin e.g. pin 0 presses gamepad button 1.
uint32_t gamepadColourMap[BUTTON_COUNT] = { 0xFF0000, 0x00FF00, 0x0000FF, 0xFFFF00, 0x00FFFF }; //Specify DotStar colours for each button press.

Adafruit_DotStar star(DOTSTAR_COUNT, DOTSTAR_DATA, DOTSTAR_CLK, DOTSTAR_FORMAT);

#ifdef DEBUG_ENABLE 
    #ifdef WAIT_FOR_SERIAL
        #pragma message "COMPILING WITH 'WAIT_FOR_SERIAL' ENABLED. SERIAL MUST BE ESTABLISHED BEFORE THE GAMEPAD CONTROLLER WILL WORK."
    #endif

    bool isInDebugMode = false;
    #define BEGIN_DEBUG_MODE debug_mode();
    #define DEBUG_PRESSED(btnIdx) Serial.print("PRESSED BUTTON '"); Serial.print(gamepadPinMap[i]); Serial.print("' ON PIN '"); Serial.print(gamepadPinMap[i]); Serial.println("'"); //Serial.println(buttonStates, BIN);
    #define DEBUG_RELEASED(btnIdx) Serial.print("RELEASED BUTTON '"); Serial.print(gamepadPinMap[i]); Serial.print("' ON PIN '"); Serial.print(gamepadPinMap[i]); Serial.println("'"); //Serial.println(buttonStates, BIN);

    void debug_mode() {
        isInDebugMode = true;
        Serial.begin(115200);
        #ifdef WAIT_FOR_SERIAL 
        while (!Serial) {
            digitalWrite(LED_BUILTIN, HIGH);
            delay(1); 
            digitalWrite(LED_BUILTIN, LOW);
            delay(50); 
        }
        #endif
        Serial.println("--DEBUG--");                
        Serial.println(FIRMWARE_NAME);              
        Serial.println(FIRMWARE_VERSION);           
        Serial.print("target report rate: ");       
        Serial.print(TARGET_REPORT_RATE);
        Serial.println("hz");
        Serial.print("buttons: ");
        Serial.println(BUTTON_COUNT);
        for (size_t i = 0; i < BUTTON_COUNT; i++)
        {
            Serial.print("INDEX ");
            Serial.print(i);
            Serial.print(" AT PIN ");
            Serial.print(gamepadPinMap[i]);
            Serial.print(" IS BUTTON ");
            Serial.print(gamepadButtonMap[i]);
            Serial.print(" WITH DOTSTAR COLOR 0x");
            uint32_t color = gamepadColourMap[i];
            if (color < 16711680) {
                Serial.print("00");
            }
            if (color < 65280) {
                Serial.print("00");
            }
            Serial.print(color, HEX);
            Serial.println(",");
        }
        Serial.println("----");
    }
#else
    #define BEGIN_DEBUG_MODE
    #define DEBUG_PRESSED(btnIdx)
    #define DEBUG_RELEASED(btnIdx)
#endif

byte pressedButton(int zone) {
    int current = 0;
    for (int i = 0; i < BUTTON_COUNT; i++) {
        if (bitRead(buttonStates, i)) {
            current++;
            if (current == zone) {
                return i;
            }
        }
    }
    return 0;
}

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);

    star.begin();
    star.setBrightness(DOTSTAR_BRIGHTNESS);
    star.show();

    for (size_t i = 0; i < BUTTON_COUNT; i++)
    {
        pinMode(gamepadPinMap[i], INPUT_PULLUP);
    }
    
    Gamepad.begin();

    BEGIN_DEBUG_MODE
}

unsigned long lastReport = 0;
unsigned long nextReport = 0;
unsigned long targetReportTime = (1000 / TARGET_REPORT_RATE) * 1000;
unsigned long reportCounter = 0;

void loop() {
#ifdef TARGET_REPORT_RATE
    unsigned long time = micros();
    if (time > nextReport) {
        Gamepad.write();
        lastReport = time;
        nextReport = lastReport + targetReportTime;
        reportCounter++;
        if (reportCounter % TARGET_REPORT_RATE == 0) {
            reportCounter = 0;
            digitalWrite(LED_BUILTIN, HIGH);
        } else {
            digitalWrite(LED_BUILTIN, LOW);
        }
    }
#else
    Gamepad.write();
#endif

    for (size_t i = 0; i < BUTTON_COUNT; i++)
    {
        int pin = gamepadPinMap[i];
        bool mstate = bitRead(buttonStates, i);
        bool state = !digitalRead(pin);
        if (state && !mstate) {
            Gamepad.press(gamepadButtonMap[i]);
            bitSet(buttonStates, i);
            pressedButtons++;
            DEBUG_PRESSED(i);
        } else if (!state && mstate) {
            Gamepad.release(gamepadButtonMap[i]);
            bitClear(buttonStates, i);
            pressedButtons--;
            DEBUG_RELEASED(i);  
        }
    }
    if (pressedButtons == 0) {
        star.clear();
    } else {
        unsigned long duration = 1000 / pressedButtons; 
        unsigned long zone = (millis() % 1000);
        zone = (zone - (zone % duration)) / duration;
        star.fill(gamepadColourMap[pressedButton(zone + 1)]);
    }
    star.show();
}