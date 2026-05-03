//For ATMega 2560 ♡

#include "helpers.h"

void setup() 
{
  mySerial_begin(9600);
  current_state = 0;
  lcd.begin(16, 2);
  rtc.begin();
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); //set RCT time to computer time

  //interrupt button pullup, so active high, so the interupt triggers on falling edge.
  myPinMode(OFF_BUTTON_PIN, myINPUT_PULLUP);
  myPinMode(ON_BUTTON_PIN, myINPUT_PULLUP);
  myPinMode(RESET_BUTTON_PIN, myINPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(OFF_BUTTON_PIN), offButtonPressed, FALLING);
  attachInterrupt(digitalPinToInterrupt(ON_BUTTON_PIN), onButtonPressed, FALLING);
  attachInterrupt(digitalPinToInterrupt(RESET_BUTTON_PIN), resetButtonPressed, FALLING);

  //State LEDs
  myPinMode(OFF_STATE_LED_PIN, myOUTPUT);
  myPinMode(IDLE_STATE_LED_PIN, myOUTPUT);
  myPinMode(ACTIVE_STATE_LED_PIN, myOUTPUT);
  myPinMode(ERROR_STATE_LED_PIN, myOUTPUT);

  //traffic button
  myPinMode(TRAFFIC_BUTTON_PIN, myINPUT_PULLUP);

  //traffic LEDs
  myPinMode(TRAFFIC_LED_RED, myOUTPUT);
  myPinMode(TRAFFIC_LED_YELLOW, myOUTPUT);
  myPinMode(TRAFFIC_LED_GREEN, myOUTPUT);

  myPinMode(BUZZER_PIN, myOUTPUT);
  myPinMode(WATER_SENSOR_POWER_PIN, myOUTPUT);
  mySetUpAnalog();
}

void loop() 
{
  //display LCD message
  switch (current_state)
  {
    case OFF_STATE: 
      offStateActive();
      break;
    case IDLE_STATE:
      idleStateActive();
      break;
    case ACTIVE_STATE:
      activeStateActive();
      break;
    case ERROR_STATE:
      errorStateActive();
      break;
    default: break;
  }
  program_index++;
}

void offStateActive()
{
  if (!(previous_state == current_state))
  {
    offStateSetup();
  }
  current_state = OFF_STATE;
  previous_state = current_state;
}

void idleStateActive()
{
  if (!(previous_state == current_state))
  {
    idleStateSetup();
  }
  current_state = IDLE_STATE;
  previous_state = current_state;

  //traffic button is active low
  if ((myDigitalRead(TRAFFIC_BUTTON_PIN) == myLOW) && (traffic_cycle_finished == true))
  {
    previousMillis = millis();
    myDigitalWrite(TRAFFIC_LED_RED, myHIGH);
    myDigitalWrite(TRAFFIC_LED_YELLOW, myLOW);
    myDigitalWrite(TRAFFIC_LED_GREEN, myLOW);
    traffic_cycle_started = true;
  }
  
  if (traffic_cycle_started == true)
  {
    updateTrafficLEDs();
  }

  water_level = myAnalogRead(WATER_SENSOR_ANALOG_PIN);

  if (program_index % 1000 == 0)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("State: IDLE");
    lcd.setCursor(0, 1);
    lcd.print("Water Level: ");
    lcd.print(water_level);
  }

  if (water_level >= error_threshold) //maybe also < 0
  {
    current_state = ERROR_STATE;
  }
  else if (water_level >= water_threshold)
  {
    current_state = ACTIVE_STATE;
  }
}

void activeStateActive()
{
  if (!(previous_state == current_state))
  {
    activeStateSetup();
  }
  current_state = ACTIVE_STATE;
  previous_state = current_state;

  //for toggling the red traffic LED so it blinks
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= time_interval) {
    previousMillis = currentMillis; // Save the last time the LED blinked
    myDigitalWrite(TRAFFIC_LED_RED, !myDigitalRead(TRAFFIC_LED_RED)); // Toggle the LED
  }

  tone(BUZZER_PIN, 440, 20); //pin, frequency, duration

  water_level = myAnalogRead(WATER_SENSOR_ANALOG_PIN);

  if (program_index % 1000 == 0)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("State: ACTIVE");
    lcd.setCursor(0, 1);
    lcd.print("Water Level: ");
    lcd.print(water_level);
  }

  if (water_level >= error_threshold || water_level < 0)
  {
    current_state = ERROR_STATE;
  }
  else if (water_level >= water_threshold)
  {
    current_state = ACTIVE_STATE;
  }
  else
  {
    current_state = IDLE_STATE;
  }
}

void errorStateActive()
{
  if (!(previous_state == current_state))
  {
    errorStateSetup();
  }
  current_state = ERROR_STATE;
  previous_state = current_state;
}






