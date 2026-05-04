//Nicole Eames, Georgia Guillebeau, Trinity Hayes


#include "myFunctions.h"
#include<LiquidCrystal.h>
#include <RTClib.h>

RTC_DS3231 rtc;

#define OFF_BUTTON_PIN 2
#define ON_BUTTON_PIN 3
#define RESET_BUTTON_PIN 18

#define OFF_STATE_LED_PIN 4
#define IDLE_STATE_LED_PIN 5
#define ACTIVE_STATE_LED_PIN 6
#define ERROR_STATE_LED_PIN 7

#define LCD_D8 8
#define LCD_D9 9
#define LCD_D10 10
#define LCD_D11 11
#define LCD_EN 12
#define LCD_RS 13
LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D11, LCD_D10, LCD_D9, LCD_D8);


#define TRAFFIC_BUTTON_PIN 14
#define TRAFFIC_LED_RED 22
#define TRAFFIC_LED_YELLOW 23
#define TRAFFIC_LED_GREEN 24

#define BUZZER_PIN 31
#define WATER_SENSOR_POWER_PIN 32
#define WATER_SENSOR_ANALOG_PIN 0


volatile int current_state = 0;
volatile int previous_state = -1;
#define OFF_STATE 0
#define IDLE_STATE 1
#define ACTIVE_STATE 2
#define ERROR_STATE 3

unsigned int water_threshold = 90;
unsigned int error_threshold = 140;
int water_level = 0;

unsigned long previousMillis = 0;
const long time_interval = 750; // Interval for traffic LED blink (in milliseconds)
bool traffic_cycle_finished = true;
bool traffic_cycle_started = false;
int program_index = 0;


void offButtonPressed();
void onButtonPressed();
void resetButtonPressed();
void errorStateSetup();
void activeStateSetup();
void idleStateSetup();
void offStateSetup();
void RCTprintTimeSerialMonitor(DateTime now);
void updateTrafficLEDs();


volatile unsigned long lastInterruptTime_ON = 0;
volatile unsigned long lastInterruptTime_OFF = 0;
volatile unsigned long lastInterruptTime_RESET = 0;

void offButtonPressed()
{
  unsigned long now = millis();
  if (now - lastInterruptTime_OFF > 50) {
    current_state = OFF_STATE;
  }
  lastInterruptTime_OFF = now;
}

void onButtonPressed()
{
  if (current_state == OFF_STATE)
  {
    unsigned long now = millis();
    if (now - lastInterruptTime_ON > 50) {
      current_state = IDLE_STATE;
    }
    lastInterruptTime_ON = now;
  }
}

void resetButtonPressed()
{
  if (current_state == ERROR_STATE || current_state == ACTIVE_STATE)
  {
    unsigned long now = millis();
    if (now - lastInterruptTime_RESET > 50) {
      current_state = IDLE_STATE;
    }
    lastInterruptTime_RESET = now;
  }
}

void errorStateSetup()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.write("ERROR: Sensor");
  lcd.setCursor(0, 1);
  lcd.write("Fault");

  DateTime now = rtc.now();
  RCTprintTimeSerialMonitor(now);
  mySerial_println(" - ERROR state entered");
  

  //fix state LEDs
  myDigitalWrite(OFF_STATE_LED_PIN, myLOW);
  myDigitalWrite(IDLE_STATE_LED_PIN, myLOW);
  myDigitalWrite(ACTIVE_STATE_LED_PIN, myLOW);
  myDigitalWrite(ERROR_STATE_LED_PIN, myHIGH);

  //turn off buzzer
  noTone(BUZZER_PIN);
  now = rtc.now();
  RCTprintTimeSerialMonitor(now);
  mySerial_println(" - Buzzer OFF");

  //turn off water sensor
  myDigitalWrite(WATER_SENSOR_POWER_PIN, myLOW);

  //turn off traffic LEDs
  myDigitalWrite(TRAFFIC_LED_RED, myLOW);
  myDigitalWrite(TRAFFIC_LED_YELLOW, myLOW);
  myDigitalWrite(TRAFFIC_LED_GREEN, myLOW);
}

void activeStateSetup()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.write("State: ACTIVE");

  DateTime now = rtc.now();
  RCTprintTimeSerialMonitor(now);
  mySerial_println(" - ACTIVE state entered");

  //fix state LEDs
  myDigitalWrite(OFF_STATE_LED_PIN, myLOW);
  myDigitalWrite(IDLE_STATE_LED_PIN, myLOW);
  myDigitalWrite(ACTIVE_STATE_LED_PIN, myHIGH);
  myDigitalWrite(ERROR_STATE_LED_PIN, myLOW);

  //turn off yellow and green traffic LEDs
  myDigitalWrite(TRAFFIC_LED_YELLOW, myLOW);
  myDigitalWrite(TRAFFIC_LED_GREEN, myLOW);

  //turn on red traffic LED
  myDigitalWrite(TRAFFIC_LED_RED, myHIGH);
  previousMillis = millis();

  now = rtc.now();
  RCTprintTimeSerialMonitor(now);
  mySerial_println(" - Buzzer ON");
}

void idleStateSetup()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.write("State: IDLE");

  DateTime now = rtc.now();
  RCTprintTimeSerialMonitor(now);
  mySerial_println(" - IDLE state entered");

  //fix state LEDs
  myDigitalWrite(OFF_STATE_LED_PIN, myLOW);
  myDigitalWrite(IDLE_STATE_LED_PIN, myHIGH);
  myDigitalWrite(ACTIVE_STATE_LED_PIN, myLOW);
  myDigitalWrite(ERROR_STATE_LED_PIN, myLOW);

  //turn on water sensor
  myDigitalWrite(WATER_SENSOR_POWER_PIN, myHIGH);

  //turn off buzzer
  noTone(BUZZER_PIN);
  now = rtc.now();
  RCTprintTimeSerialMonitor(now);
  mySerial_println(" - Buzzer OFF");

  //turn off yellow and green traffic LEDs
  myDigitalWrite(TRAFFIC_LED_YELLOW, myLOW);
  myDigitalWrite(TRAFFIC_LED_GREEN, myLOW);

  //turn on red traffic LED
  myDigitalWrite(TRAFFIC_LED_RED, myHIGH);

  traffic_cycle_finished = true;
}

void offStateSetup()
{
  //display LCD message
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.write("System OFF");

  DateTime now = rtc.now();
  RCTprintTimeSerialMonitor(now);
  mySerial_println(" - OFF state entered");

  //fix state LEDs
  myDigitalWrite(OFF_STATE_LED_PIN, myHIGH);
  myDigitalWrite(IDLE_STATE_LED_PIN, myLOW);
  myDigitalWrite(ACTIVE_STATE_LED_PIN, myLOW);
  myDigitalWrite(ERROR_STATE_LED_PIN, myLOW);

  //turn off water sensor
  myDigitalWrite(WATER_SENSOR_POWER_PIN, myLOW);

  //turn off buzzer
  noTone(BUZZER_PIN);
  now = rtc.now();
  RCTprintTimeSerialMonitor(now);
  mySerial_println(" - Buzzer OFF");

  //turn off traffic LEDs
  myDigitalWrite(TRAFFIC_LED_RED, myLOW);
  myDigitalWrite(TRAFFIC_LED_YELLOW, myLOW);
  myDigitalWrite(TRAFFIC_LED_GREEN, myLOW);
}

void RCTprintTimeSerialMonitor(DateTime now)
{
  mySerial_print(now.hour());
  mySerial_print(':');
  mySerial_print(now.minute());
  mySerial_print(':');
  mySerial_print(now.second());
}

void updateTrafficLEDs()
{
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= 3 * time_interval) {
    myDigitalWrite(TRAFFIC_LED_RED, myHIGH);
    myDigitalWrite(TRAFFIC_LED_YELLOW, myLOW);
    myDigitalWrite(TRAFFIC_LED_GREEN, myLOW);
    traffic_cycle_finished = true;
    traffic_cycle_started = false;
  }
  else if (currentMillis - previousMillis >= 2 * time_interval) {
    myDigitalWrite(TRAFFIC_LED_RED, myLOW);
    myDigitalWrite(TRAFFIC_LED_YELLOW, myHIGH);
    myDigitalWrite(TRAFFIC_LED_GREEN, myLOW);
    traffic_cycle_finished = false;
  }
  else if (currentMillis - previousMillis >= time_interval) {
    myDigitalWrite(TRAFFIC_LED_RED, myLOW);
    myDigitalWrite(TRAFFIC_LED_YELLOW, myLOW);
    myDigitalWrite(TRAFFIC_LED_GREEN, myHIGH);
    traffic_cycle_finished = false;
  }
}

