//For ATMega 2560 ♡

#pragma once

//for pinMode, digitalWrite, and digitalRead
volatile unsigned char* port_a = (unsigned char*) 0x22;
volatile unsigned char* ddr_a = (unsigned char*) 0x21;
volatile unsigned char* pin_a = (unsigned char*) 0x20;

volatile unsigned char* port_b = (unsigned char*) 0x25;
volatile unsigned char* ddr_b = (unsigned char*) 0x24;
volatile unsigned char* pin_b = (unsigned char*) 0x23;

volatile unsigned char* port_c = (unsigned char*) 0x28;
volatile unsigned char* ddr_c = (unsigned char*) 0x27;
volatile unsigned char* pin_c = (unsigned char*) 0x26;

volatile unsigned char* port_d = (unsigned char*) 0x2B;
volatile unsigned char* ddr_d = (unsigned char*) 0x2A;
volatile unsigned char* pin_d = (unsigned char*) 0x29;

volatile unsigned char* port_e = (unsigned char*) 0x2E;
volatile unsigned char* ddr_e = (unsigned char*) 0x2D;
volatile unsigned char* pin_e = (unsigned char*) 0x2C;

volatile unsigned char* port_g = (unsigned char*) 0x34;
volatile unsigned char* ddr_g = (unsigned char*) 0x33;
volatile unsigned char* pin_g = (unsigned char*) 0x32;

volatile unsigned char* port_h = (unsigned char*) 0x102;
volatile unsigned char* ddr_h = (unsigned char*) 0x101;
volatile unsigned char* pin_h = (unsigned char*) 0x100;

volatile unsigned char* port_j = (unsigned char*) 0x105;
volatile unsigned char* ddr_j = (unsigned char*) 0x104;
volatile unsigned char* pin_j = (unsigned char*) 0x103;

volatile unsigned char* port_l = (unsigned char*) 0x10B;
volatile unsigned char* ddr_l = (unsigned char*) 0x10A;
volatile unsigned char* pin_l = (unsigned char*) 0x109;

//for delay
volatile unsigned char *myTCCR1A = (unsigned char *) 0x80;
volatile unsigned char *myTCCR1B = (unsigned char *) 0x81;
volatile unsigned char *myTCCR1C = (unsigned char *) 0x82;
volatile unsigned char *myTIMSK1 = (unsigned char *) 0x6F;
volatile unsigned int  *myTCNT1  = (unsigned  int *) 0x84;
volatile unsigned char *myTIFR1 =  (unsigned char *) 0x36;

//for analog read
volatile unsigned char *myUCSR0A = (unsigned char *)0x00C0;
volatile unsigned char *myUCSR0B = (unsigned char *)0x00C1;
volatile unsigned char *myUCSR0C = (unsigned char *)0x00C2;
volatile unsigned int  *myUBRR0  = (unsigned int *) 0x00C4;
volatile unsigned char *myUDR0   = (unsigned char *)0x00C6;
 
volatile unsigned char* my_ADMUX = (unsigned char*) 0x7C;
volatile unsigned char* my_ADCSRB = (unsigned char*) 0x7B;
volatile unsigned char* my_ADCSRA = (unsigned char*) 0x7A;
volatile unsigned int* my_ADC_DATA = (unsigned int*) 0x78;

//for Serial.begin, Serial.available, Serial.print, Serial.println, Serial.read
#define RDA 0x80
#define TBE 0x20  


const int myOUTPUT = 0;
const int myINPUT = 1;
const int myINPUT_PULLUP = 2;

const bool myHIGH = true;
const bool myLOW = false;

//foward declarations
void myPinMode(int pin, int type);
void myDigitalWrite(int pin, bool voltage);
bool myDigitalRead(int pin);

void myDelay(unsigned int msDelayed);
void waveform(unsigned int ticks);

void mySerial_begin(unsigned long U0baud);
int mySerial_available();
unsigned char mySerial_read();
void writeChar(unsigned char U0pdata);
void mySerial_print(const char* toPrint);
void mySerial_print(const char toPrint);
void mySerial_print(const int toPrint);
void mySerial_print(const long toPrint);
void mySerial_print(const bool toPrint);
void mySerial_print(const double toPrint);

void mySerial_println(const char* toPrint);
void mySerial_println(const char toPrint);
void mySerial_println(const int toPrint);
void mySerial_println(const long toPrint);
void mySerial_println(const bool toPrint);
void mySerial_println(const double toPrint);
void mySerial_write(const char toPrint);
void mySerial_write(const char* toPrint);

void mySetUpAnalog();
unsigned int myAnalogRead(unsigned char adc_channel_num);


unsigned int myAnalogRead(unsigned char adc_channel_num)
{
  //mySetUpAnalog();
  // clear the channel selection bits (MUX 4:0)
 *my_ADMUX &= 0b11100000; 

  // clear the channel selection bits (MUX 5) hint: it's not in the ADMUX register
  *my_ADCSRB &= 0b11110111;
 
  // set the channel selection bits for channel 0
  *my_ADMUX |= (adc_channel_num & 0x1F); // CHECK WORKS

  // set bit 6 of ADCSRA to 1 to start a conversion
  *my_ADCSRA |= 0b01000000;

  // wait for the conversion to complete
  while((*my_ADCSRA & 0x40) != 0);
  // return the result in the ADC data register and format the data based on right justification (check the lecture slide)
  
  unsigned int val = (*my_ADC_DATA & 0x03FF); 
  return val;
}

void mySetUpAnalog()
{
  // setup the A register
  // set bit 7 to 1 to enable the ADC 
  *my_ADCSRA |= 0b10000000;
  // clear bit 5 to 0 to disable the ADC trigger mode
  *my_ADCSRA &= 0b11011111;
  // clear bit 3 to 0 to disable the ADC interrupt 
  *my_ADCSRA &= 0b11110111;
  // clear bit 0-2 to 0 to set prescaler selection to slow reading
  *my_ADCSRA &= 0b11111000;  // clear
  *my_ADCSRA |= 0b00000111;  // prescaler = 128

  // setup the B register
  // clear bit 3 to 0 to reset the channel and gain bits
  *my_ADCSRB &= 0b11110111;
  // clear bit 2-0 to 0 to set free running mode
  *my_ADCSRB &= 0b11111000;

  // setup the MUX Register
  // clear bit 7 to 0 for AVCC analog reference
  *my_ADMUX &= 0b01111111;
  // set bit 6 to 1 for AVCC analog reference
  *my_ADMUX |= 0b01000000;
  // clear bit 5 to 0 for right adjust result
  *my_ADMUX &= 0b11011111;
  // clear bit 4-0 to 0 to reset the channel and gain bits
  *my_ADMUX &= 0b11100000;
}


void myPinMode(int pin, int type)
{
  if (type == myOUTPUT) 
  {
    switch (pin) {
      case 0:  //PE0
        *ddr_e |= (0x01 << 0);
        break;
      case 1:  //PE1
        *ddr_e |= (0x01 << 1);
        break;
      case 2:  //PE4
        *ddr_e |= (0x01 << 4);
        break;
      case 3:  //PE5
        *ddr_e |= (0x01 << 5);
        break;
      case 4:  //PG5
        *ddr_g |= (0x01 << 5);
        break;
      case 5:  //PE3
        *ddr_e |= (0x01 << 3);
        break;
      case 6:  //PH3
        *ddr_h |= (0x01 << 3);
        break;
      case 7:  //PH4
        *ddr_h |= (0x01 << 4);
        break;
      case 8:  //PH5
        *ddr_h |= (0x01 << 5);
        break;
      case 9:  //PH6
        *ddr_h |= (0x01 << 6);
        break;
      case 10: //PB4
        *ddr_b |= (0x01 << 4);
        break;
      case 11: //PB5
        *ddr_b |= (0x01 << 5);
        break;
      case 12: //PB6
        *ddr_b |= (0x01 << 6);
        break;
      case 13: //PB7
        *ddr_b |= (0x01 << 7);
        break;
      case 14: //PJ1
        *ddr_j |= (0x01 << 1);
        break;
      case 15: //PJ0
        *ddr_j |= (0x01 << 0);
        break;
      case 16: //PH1
        *ddr_h |= (0x01 << 1);
        break;
      case 17: //PH0
        *ddr_h |= (0x01 << 0);
        break;
      case 18: //PD3
        *ddr_d |= (0x01 << 3);
        break;
      case 19: //PD2
        *ddr_d |= (0x01 << 2);
        break;
      case 20: //PD1
        *ddr_d |= (0x01 << 1);
        break;
      case 21: //PD0
        *ddr_d |= (0x01 << 0);
        break;
      case 22: //PA0
        *ddr_a |= (0x01 << 0);
        break;
      case 23: //PA1
        *ddr_a |= (0x01 << 1);
        break;
      case 24: //PA2
        *ddr_a |= (0x01 << 2);
        break;
      case 25: //PA3
        *ddr_a |= (0x01 << 3);
        break;
      case 26: //PA4
        *ddr_a |= (0x01 << 4);
        break;
      case 27: //PA5
        *ddr_a |= (0x01 << 5);
        break;
      case 28: //PA6
        *ddr_a |= (0x01 << 6);
        break;
      case 29: //PA7
        *ddr_a |= (0x01 << 7);
        break;
      case 30: //PC7
        *ddr_c |= (0x01 << 7);
        break;
      case 31: //PC6
        *ddr_c |= (0x01 << 6);
        break;
      case 32: //PC5
        *ddr_c |= (0x01 << 5);
        break;
      case 33: //PC4
        *ddr_c |= (0x01 << 4);
        break;
      case 34: //PC3
        *ddr_c |= (0x01 << 3);
        break;
      case 35: //PC2
        *ddr_c |= (0x01 << 2);
        break;
      case 36: //PC1
        *ddr_c |= (0x01 << 1);
        break;
      case 37: //PC0
        *ddr_c |= (0x01 << 0);
        break;
      case 38: //PD7
        *ddr_d |= (0x01 << 7);
        break;
      case 39: //PG2
        *ddr_g |= (0x01 << 2);
        break;
      case 40: //PG1
        *ddr_g |= (0x01 << 1);
        break;
      case 41: //PG0
        *ddr_g |= (0x01 << 0);
        break;
      case 42: //PL7
        *ddr_l |= (0x01 << 7);
        break;
      case 43: //PL6
        *ddr_l |= (0x01 << 6);
        break;
      case 44: //PL5
        *ddr_l |= (0x01 << 5);
        break;
      case 45: //PL4
        *ddr_l |= (0x01 << 4);
        break;
      case 46: //PL3
        *ddr_l |= (0x01 << 3);
        break;
      case 47: //PL2
        *ddr_l |= (0x01 << 2);
        break;
      case 48: //PL1
        *ddr_l |= (0x01 << 1);
        break;
      case 49: //PL0
        *ddr_l |= (0x01 << 0);
        break;
      case 50: //PB3
        *ddr_b |= (0x01 << 3);
        break;
      case 51: //PB2
        *ddr_b |= (0x01 << 2);
        break;
      case 52: //PB1
        *ddr_b |= (0x01 << 1);
        break;
      case 53: //PB0
        *ddr_b |= (0x01 << 0);
        break;
      default: 
        mySerial_println("Error in myPinMode: invalid pin");
        break;
    }
  }
  else if (type == myINPUT)
  {
    switch (pin) {
      case 0:  //PE0
        *ddr_e &= ~(0x01 << 0);
        *port_e &= ~(0x01 << 0);
        break;
      case 1:  //PE1
        *ddr_e &= ~(0x01 << 1);
        *port_e &= ~(0x01 << 1);
        break;
      case 2:  //PE4
        *ddr_e &= ~(0x01 << 4);
        *port_e &= ~(0x01 << 4);
        break;
      case 3:  //PE5
        *ddr_e &= ~(0x01 << 5);
        *port_e &= ~(0x01 << 5);
        break;
      case 4:  //PG5
        *ddr_g &= ~(0x01 << 5);
        *port_g &= ~(0x01 << 5);
        break;
      case 5:  //PE3
        *ddr_e &= ~(0x01 << 3);
        *port_e &= ~(0x01 << 3);
        break;
      case 6:  //PH3
        *ddr_h &= ~(0x01 << 3);
        *port_h &= ~(0x01 << 3);
        break;
      case 7:  //PH4
        *ddr_h &= ~(0x01 << 4);
        *port_h &= ~(0x01 << 4);
        break;
      case 8:  //PH5
        *ddr_h &= ~(0x01 << 5);
        *port_h &= ~(0x01 << 5);
        break;
      case 9:  //PH6
        *ddr_h &= ~(0x01 << 6);
        *port_h &= ~(0x01 << 6);
        break;
      case 10: //PB4
        *ddr_b &= ~(0x01 << 4);
        *port_b &= ~(0x01 << 4);
        break;
      case 11: //PB5
        *ddr_b &= ~(0x01 << 5);
        *port_b &= ~(0x01 << 5);
        break;
      case 12: //PB6
        *ddr_b &= ~(0x01 << 6);
        *port_b &= ~(0x01 << 6);
        break;
      case 13: //PB7
        *ddr_b &= ~(0x01 << 7);
        *port_b &= ~(0x01 << 7);
        break;
      case 14: //PJ1
        *ddr_j &= ~(0x01 << 1);
        *port_j &= ~(0x01 << 1);
        break;
      case 15: //PJ0
        *ddr_j &= ~(0x01 << 0);
        *port_j &= ~(0x01 << 0);
        break;
      case 16: //PH1
        *ddr_h &= ~(0x01 << 1);
        *port_h &= ~(0x01 << 1);
        break;
      case 17: //PH0
        *ddr_h &= ~(0x01 << 0);
        *port_h &= ~(0x01 << 0);
        break;
      case 18: //PD3
        *ddr_d &= ~(0x01 << 3);
        *port_d &= ~(0x01 << 3);
        break;
      case 19: //PD2
        *ddr_d &= ~(0x01 << 2);
        *port_d &= ~(0x01 << 2);
        break;
      case 20: //PD1
        *ddr_d &= ~(0x01 << 1);
        *port_d &= ~(0x01 << 1);
        break;
      case 21: //PD0
        *ddr_d &= ~(0x01 << 0);
        *port_d &= ~(0x01 << 0);
        break;
      case 22: //PA0
        *ddr_a &= ~(0x01 << 0);
        *port_a &= ~(0x01 << 0);
        break;
      case 23: //PA1
        *ddr_a &= ~(0x01 << 1);
        *port_a &= ~(0x01 << 1);
        break;
      case 24: //PA2
        *ddr_a &= ~(0x01 << 2);
        *port_a &= ~(0x01 << 2);
        break;
      case 25: //PA3
        *ddr_a &= ~(0x01 << 3);
        *port_a &= ~(0x01 << 3);
        break;
      case 26: //PA4
        *ddr_a &= ~(0x01 << 4);
        *port_a &= ~(0x01 << 4);
        break;
      case 27: //PA5
        *ddr_a &= ~(0x01 << 5);
        *port_a &= ~(0x01 << 5);
        break;
      case 28: //PA6
        *ddr_a &= ~(0x01 << 6);
        *port_a &= ~(0x01 << 6);
        break;
      case 29: //PA7
        *ddr_a &= ~(0x01 << 7);
        *port_a &= ~(0x01 << 7);
        break;
      case 30: //PC7
        *ddr_c &= ~(0x01 << 7);
        *port_c &= ~(0x01 << 7);
        break;
      case 31: //PC6
        *ddr_c &= ~(0x01 << 6);
        *port_c &= ~(0x01 << 6);
        break;
      case 32: //PC5
        *ddr_c &= ~(0x01 << 5);
        *port_c &= ~(0x01 << 5);
        break;
      case 33: //PC4
        *ddr_c &= ~(0x01 << 4);
        *port_c &= ~(0x01 << 4);
        break;
      case 34: //PC3
        *ddr_c &= ~(0x01 << 3);
        *port_c &= ~(0x01 << 3);
        break;
      case 35: //PC2
        *ddr_c &= ~(0x01 << 2);
        *port_c &= ~(0x01 << 2);
        break;
      case 36: //PC1
        *ddr_c &= ~(0x01 << 1);
        *port_c &= ~(0x01 << 1);
        break;
      case 37: //PC0
        *ddr_c &= ~(0x01 << 0);
        *port_c &= ~(0x01 << 0);
        break;
      case 38: //PD7
        *ddr_d &= ~(0x01 << 7);
        *port_d &= ~(0x01 << 7);
        break;
      case 39: //PG2
        *ddr_g &= ~(0x01 << 2);
        *port_g &= ~(0x01 << 2);
        break;
      case 40: //PG1
        *ddr_g &= ~(0x01 << 1);
        *port_g &= ~(0x01 << 1);
        break;
      case 41: //PG0
        *ddr_g &= ~(0x01 << 0);
        *port_g &= ~(0x01 << 0);
        break;
      case 42: //PL7
        *ddr_l &= ~(0x01 << 7);
        *port_l &= ~(0x01 << 7);
        break;
      case 43: //PL6
        *ddr_l &= ~(0x01 << 6);
        *port_l &= ~(0x01 << 6);
        break;
      case 44: //PL5
        *ddr_l &= ~(0x01 << 5);
        *port_l &= ~(0x01 << 5);
        break;
      case 45: //PL4
        *ddr_l &= ~(0x01 << 4);
        *port_l &= ~(0x01 << 4);
        break;
      case 46: //PL3
        *ddr_l &= ~(0x01 << 3);
        *port_l &= ~(0x01 << 3);
        break;
      case 47: //PL2
        *ddr_l &= ~(0x01 << 2);
        *port_l &= ~(0x01 << 2);
        break;
      case 48: //PL1
        *ddr_l &= ~(0x01 << 1);
        *port_l &= ~(0x01 << 1);
        break;
      case 49: //PL0
        *ddr_l &= ~(0x01 << 0);
        *port_l &= ~(0x01 << 0);
        break;
      case 50: //PB3
        *ddr_b &= ~(0x01 << 3);
        *port_b &= ~(0x01 << 3);
        break;
      case 51: //PB2
        *ddr_b &= ~(0x01 << 2);
        *port_b &= ~(0x01 << 2);
        break;
      case 52: //PB1
        *ddr_b &= ~(0x01 << 1);
        *port_b &= ~(0x01 << 1);
        break;
      case 53: //PB0
        *ddr_b &= ~(0x01 << 0);
        *port_b &= ~(0x01 << 0);
        break;
      default: 
        mySerial_println("Error in myPinMode: invalid pin");
        break;
    }
  }
  else if (type == myINPUT_PULLUP)
  {
    switch (pin) {
      case 0:  //PE0
        *ddr_e &= ~(0x01 << 0);
        *port_e |= (0x01 << 0);
        break;
      case 1:  //PE1
        *ddr_e &= ~(0x01 << 1);
        *port_e |= (0x01 << 1);
        break;
      case 2:  //PE4
        *ddr_e &= ~(0x01 << 4);
        *port_e |= (0x01 << 4);
        break;
      case 3:  //PE5
        *ddr_e &= ~(0x01 << 5);
        *port_e |= (0x01 << 5);
        break;
      case 4:  //PG5
        *ddr_g &= ~(0x01 << 5);
        *port_g |= (0x01 << 5);
        break;
      case 5:  //PE3
        *ddr_e &= ~(0x01 << 3);
        *port_e |= (0x01 << 3);
        break;
      case 6:  //PH3
        *ddr_h &= ~(0x01 << 3);
        *port_h |= (0x01 << 3);
        break;
      case 7:  //PH4
        *ddr_h &= ~(0x01 << 4);
        *port_h |= (0x01 << 4);
        break;
      case 8:  //PH5
        *ddr_h &= ~(0x01 << 5);
        *port_h |= (0x01 << 5);
        break;
      case 9:  //PH6
        *ddr_h &= ~(0x01 << 6);
        *port_h |= (0x01 << 6);
        break;
      case 10: //PB4
        *ddr_b &= ~(0x01 << 4);
        *port_b |= (0x01 << 4);
        break;
      case 11: //PB5
        *ddr_b &= ~(0x01 << 5);
        *port_b |= (0x01 << 5);
        break;
      case 12: //PB6
        *ddr_b &= ~(0x01 << 6);
        *port_b |= (0x01 << 6);
        break;
      case 13: //PB7
        *ddr_b &= ~(0x01 << 7);
        *port_b |= (0x01 << 7);
        break;
      case 14: //PJ1
        *ddr_j &= ~(0x01 << 1);
        *port_j |= (0x01 << 1);
        break;
      case 15: //PJ0
        *ddr_j &= ~(0x01 << 0);
        *port_j |= (0x01 << 0);
        break;
      case 16: //PH1
        *ddr_h &= ~(0x01 << 1);
        *port_h |= (0x01 << 1);
        break;
      case 17: //PH0
        *ddr_h &= ~(0x01 << 0);
        *port_h |= (0x01 << 0);
        break;
      case 18: //PD3
        *ddr_d &= ~(0x01 << 3);
        *port_d |= (0x01 << 3);
        break;
      case 19: //PD2
        *ddr_d &= ~(0x01 << 2);
        *port_d |= (0x01 << 2);
        break;
      case 20: //PD1
        *ddr_d &= ~(0x01 << 1);
        *port_d |= (0x01 << 1);
        break;
      case 21: //PD0
        *ddr_d &= ~(0x01 << 0);
        *port_d |= (0x01 << 0);
        break;
      case 22: //PA0
        *ddr_a &= ~(0x01 << 0);
        *port_a |= (0x01 << 0);
        break;
      case 23: //PA1
        *ddr_a &= ~(0x01 << 1);
        *port_a |= (0x01 << 1);
        break;
      case 24: //PA2
        *ddr_a &= ~(0x01 << 2);
        *port_a |= (0x01 << 2);
        break;
      case 25: //PA3
        *ddr_a &= ~(0x01 << 3);
        *port_a |= (0x01 << 3);
        break;
      case 26: //PA4
        *ddr_a &= ~(0x01 << 4);
        *port_a |= (0x01 << 4);
        break;
      case 27: //PA5
        *ddr_a &= ~(0x01 << 5);
        *port_a |= (0x01 << 5);
        break;
      case 28: //PA6
        *ddr_a &= ~(0x01 << 6);
        *port_a |= (0x01 << 6);
        break;
      case 29: //PA7
        *ddr_a &= ~(0x01 << 7);
        *port_a |= (0x01 << 7);
        break;
      case 30: //PC7
        *ddr_c &= ~(0x01 << 7);
        *port_c |= (0x01 << 7);
        break;
      case 31: //PC6
        *ddr_c &= ~(0x01 << 6);
        *port_c |= (0x01 << 6);
        break;
      case 32: //PC5
        *ddr_c &= ~(0x01 << 5);
        *port_c |= (0x01 << 5);
        break;
      case 33: //PC4
        *ddr_c &= ~(0x01 << 4);
        *port_c |= (0x01 << 4);
        break;
      case 34: //PC3
        *ddr_c &= ~(0x01 << 3);
        *port_c |= (0x01 << 3);
        break;
      case 35: //PC2
        *ddr_c &= ~(0x01 << 2);
        *port_c |= (0x01 << 2);
        break;
      case 36: //PC1
        *ddr_c &= ~(0x01 << 1);
        *port_c |= (0x01 << 1);
        break;
      case 37: //PC0
        *ddr_c &= ~(0x01 << 0);
        *port_c |= (0x01 << 0);
        break;
      case 38: //PD7
        *ddr_d &= ~(0x01 << 7);
        *port_d |= (0x01 << 7);
        break;
      case 39: //PG2
        *ddr_g &= ~(0x01 << 2);
        *port_g |= (0x01 << 2);
        break;
      case 40: //PG1
        *ddr_g &= ~(0x01 << 1);
        *port_g |= (0x01 << 1);
        break;
      case 41: //PG0
        *ddr_g &= ~(0x01 << 0);
        *port_g |= (0x01 << 0);
        break;
      case 42: //PL7
        *ddr_l &= ~(0x01 << 7);
        *port_l |= (0x01 << 7);
        break;
      case 43: //PL6
        *ddr_l &= ~(0x01 << 6);
        *port_l |= (0x01 << 6);
        break;
      case 44: //PL5
        *ddr_l &= ~(0x01 << 5);
        *port_l |= (0x01 << 5);
        break;
      case 45: //PL4
        *ddr_l &= ~(0x01 << 4);
        *port_l |= (0x01 << 4);
        break;
      case 46: //PL3
        *ddr_l &= ~(0x01 << 3);
        *port_l |= (0x01 << 3);
        break;
      case 47: //PL2
        *ddr_l &= ~(0x01 << 2);
        *port_l |= (0x01 << 2);
        break;
      case 48: //PL1
        *ddr_l &= ~(0x01 << 1);
        *port_l |= (0x01 << 1);
        break;
      case 49: //PL0
        *ddr_l &= ~(0x01 << 0);
        *port_l |= (0x01 << 0);
        break;
      case 50: //PB3
        *ddr_b &= ~(0x01 << 3);
        *port_b |= (0x01 << 3);
        break;
      case 51: //PB2
        *ddr_b &= ~(0x01 << 2);
        *port_b |= (0x01 << 2);
        break;
      case 52: //PB1
        *ddr_b &= ~(0x01 << 1);
        *port_b |= (0x01 << 1);
        break;
      case 53: //PB0
        *ddr_b &= ~(0x01 << 0);
        *port_b |= (0x01 << 0);
        break;
      default: 
        mySerial_println("Error in myPinMode: invalid pin");
        break;
    }
  }
  else 
  {
    mySerial_println("Error in myPinMode: invalid type");
  }
}

void myDigitalWrite(int pin, bool voltage)
{
  if (voltage == myHIGH)
  {
    switch (pin) {
      case 0:  //PE0
        *port_e |= (0x01 << 0);
        break;
      case 1:  //PE1
        *port_e |= (0x01 << 1);
        break;
      case 2:  //PE4
        *port_e |= (0x01 << 4);
        break;
      case 3:  //PE5
        *port_e |= (0x01 << 5);
        break;
      case 4:  //PG5
        *port_g |= (0x01 << 5);
        break;
      case 5:  //PE3
        *port_e |= (0x01 << 3);
        break;
      case 6:  //PH3
        *port_h |= (0x01 << 3);
        break;
      case 7:  //PH4
        *port_h |= (0x01 << 4);
        break;
      case 8:  //PH5
        *port_h |= (0x01 << 5);
        break;
      case 9:  //PH6
        *port_h |= (0x01 << 6);
        break;
      case 10: //PB4
        *port_b |= (0x01 << 4);
        break;
      case 11: //PB5
        *port_b |= (0x01 << 5);
        break;
      case 12: //PB6
        *port_b |= (0x01 << 6);
        break;
      case 13: //PB7
        *port_b |= (0x01 << 7);
        break;
      case 14: //PJ1
        *port_j |= (0x01 << 1);
        break;
      case 15: //PJ0
        *port_j |= (0x01 << 0);
        break;
      case 16: //PH1
        *port_h |= (0x01 << 1);
        break;
      case 17: //PH0
        *port_h |= (0x01 << 0);
        break;
      case 18: //PD3
        *port_d |= (0x01 << 3);
        break;
      case 19: //PD2
        *port_d |= (0x01 << 2);
        break;
      case 20: //PD1
        *port_d |= (0x01 << 1);
        break;
      case 21: //PD0
        *port_d |= (0x01 << 0);
        break;
      case 22: //PA0
        *port_a |= (0x01 << 0);
        break;
      case 23: //PA1
        *port_a |= (0x01 << 1);
        break;
      case 24: //PA2
        *port_a |= (0x01 << 2);
        break;
      case 25: //PA3
        *port_a |= (0x01 << 3);
        break;
      case 26: //PA4
        *port_a |= (0x01 << 4);
        break;
      case 27: //PA5
        *port_a |= (0x01 << 5);
        break;
      case 28: //PA6
        *port_a |= (0x01 << 6);
        break;
      case 29: //PA7
        *port_a |= (0x01 << 7);
        break;
      case 30: //PC7
        *port_c |= (0x01 << 7);
        break;
      case 31: //PC6
        *port_c |= (0x01 << 6);
        break;
      case 32: //PC5
        *port_c |= (0x01 << 5);
        break;
      case 33: //PC4
        *port_c |= (0x01 << 4);
        break;
      case 34: //PC3
        *port_c |= (0x01 << 3);
        break;
      case 35: //PC2
        *port_c |= (0x01 << 2);
        break;
      case 36: //PC1
        *port_c |= (0x01 << 1);
        break;
      case 37: //PC0
        *port_c |= (0x01 << 0);
        break;
      case 38: //PD7
        *port_d |= (0x01 << 7);
        break;
      case 39: //PG2
        *port_g |= (0x01 << 2);
        break;
      case 40: //PG1
        *port_g |= (0x01 << 1);
        break;
      case 41: //PG0
        *port_g |= (0x01 << 0);
        break;
      case 42: //PL7
        *port_l |= (0x01 << 7);
        break;
      case 43: //PL6
        *port_l |= (0x01 << 6);
        break;
      case 44: //PL5
        *port_l |= (0x01 << 5);
        break;
      case 45: //PL4
        *port_l |= (0x01 << 4);
        break;
      case 46: //PL3
        *port_l |= (0x01 << 3);
        break;
      case 47: //PL2
        *port_l |= (0x01 << 2);
        break;
      case 48: //PL1
        *port_l |= (0x01 << 1);
        break;
      case 49: //PL0
        *port_l |= (0x01 << 0);
        break;
      case 50: //PB3
        *port_b |= (0x01 << 3);
        break;
      case 51: //PB2
        *port_b |= (0x01 << 2);
        break;
      case 52: //PB1
        *port_b |= (0x01 << 1);
        break;
      case 53: //PB0
        *port_b |= (0x01 << 0);
        break;
      default: 
        mySerial_println("Error in myDigitalWrite: invalid pin");
        break;
    }
  }
  else if (voltage == myLOW)
  {
    switch (pin) {
      case 0:  //PE0
        *port_e &= ~(0x01 << 0);
        break;
      case 1:  //PE1
        *port_e &= ~(0x01 << 1);
        break;
      case 2:  //PE4
        *port_e &= ~(0x01 << 4);
        break;
      case 3:  //PE5
        *port_e &= ~(0x01 << 5);
        break;
      case 4:  //PG5
        *port_g &= ~(0x01 << 5);
        break;
      case 5:  //PE3
        *port_e &= ~(0x01 << 3);
        break;
      case 6:  //PH3
        *port_h &= ~(0x01 << 3);
        break;
      case 7:  //PH4
        *port_h &= ~(0x01 << 4);
        break;
      case 8:  //PH5
        *port_h &= ~(0x01 << 5);
        break;
      case 9:  //PH6
        *port_h &= ~(0x01 << 6);
        break;
      case 10: //PB4
        *port_b &= ~(0x01 << 4);
        break;
      case 11: //PB5
        *port_b &= ~(0x01 << 5);
        break;
      case 12: //PB6
        *port_b &= ~(0x01 << 6);
        break;
      case 13: //PB7
        *port_b &= ~(0x01 << 7);
        break;
      case 14: //PJ1
        *port_j &= ~(0x01 << 1);
        break;
      case 15: //PJ0
        *port_j &= ~(0x01 << 0);
        break;
      case 16: //PH1
        *port_h &= ~(0x01 << 1);
        break;
      case 17: //PH0
        *port_h &= ~(0x01 << 0);
        break;
      case 18: //PD3
        *port_d &= ~(0x01 << 3);
        break;
      case 19: //PD2
        *port_d &= ~(0x01 << 2);
        break;
      case 20: //PD1
        *port_d &= ~(0x01 << 1);
        break;
      case 21: //PD0
        *port_d &= ~(0x01 << 0);
        break;
      case 22: //PA0
        *port_a &= ~(0x01 << 0);
        break;
      case 23: //PA1
        *port_a &= ~(0x01 << 1);
        break;
      case 24: //PA2
        *port_a &= ~(0x01 << 2);
        break;
      case 25: //PA3
        *port_a &= ~(0x01 << 3);
        break;
      case 26: //PA4
        *port_a &= ~(0x01 << 4);
        break;
      case 27: //PA5
        *port_a &= ~(0x01 << 5);
        break;
      case 28: //PA6
        *port_a &= ~(0x01 << 6);
        break;
      case 29: //PA7
        *port_a &= ~(0x01 << 7);
        break;
      case 30: //PC7
        *port_c &= ~(0x01 << 7);
        break;
      case 31: //PC6
        *port_c &= ~(0x01 << 6);
        break;
      case 32: //PC5
        *port_c &= ~(0x01 << 5);
        break;
      case 33: //PC4
        *port_c &= ~(0x01 << 4);
        break;
      case 34: //PC3
        *port_c &= ~(0x01 << 3);
        break;
      case 35: //PC2
        *port_c &= ~(0x01 << 2);
        break;
      case 36: //PC1
        *port_c &= ~(0x01 << 1);
        break;
      case 37: //PC0
        *port_c &= ~(0x01 << 0);
        break;
      case 38: //PD7
        *port_d &= ~(0x01 << 7);
        break;
      case 39: //PG2
        *port_g &= ~(0x01 << 2);
        break;
      case 40: //PG1
        *port_g &= ~(0x01 << 1);
        break;
      case 41: //PG0
        *port_g &= ~(0x01 << 0);
        break;
      case 42: //PL7
        *port_l &= ~(0x01 << 7);
        break;
      case 43: //PL6
        *port_l &= ~(0x01 << 6);
        break;
      case 44: //PL5
        *port_l &= ~(0x01 << 5);
        break;
      case 45: //PL4
        *port_l &= ~(0x01 << 4);
        break;
      case 46: //PL3
        *port_l &= ~(0x01 << 3);
        break;
      case 47: //PL2
        *port_l &= ~(0x01 << 2);
        break;
      case 48: //PL1
        *port_l &= ~(0x01 << 1);
        break;
      case 49: //PL0
        *port_l &= ~(0x01 << 0);
        break;
      case 50: //PB3
        *port_b &= ~(0x01 << 3);
        break;
      case 51: //PB2
        *port_b &= ~(0x01 << 2);
        break;
      case 52: //PB1
        *port_b &= ~(0x01 << 1);
        break;
      case 53: //PB0
        *port_b &= ~(0x01 << 0);
        break;
      default: 
        mySerial_println("Error in myDigitalWrite: invalid pin");
        break;
    }
  }
  else 
  {
    mySerial_println("Error in myDigitalWrite: invalid voltage");
  }
}

bool myDigitalRead(int pin)
{
  switch (pin) {
    case 0:  //PE0
      return *pin_e & (0x01 << 0); 
      break;
    case 1:  //PE1
      return *pin_e & (0x01 << 1);
      break;
    case 2:  //PE4
      return *pin_e & (0x01 << 4);
      break;
    case 3:  //PE5
      return *pin_e & (0x01 << 5);
      break;
    case 4:  //PG5
      return *pin_g & (0x01 << 5);
      break;
    case 5:  //PE3
      return *pin_e & (0x01 << 3);
      break;
    case 6:  //PH3
      return *pin_h & (0x01 << 3);
      break;
    case 7:  //PH4
      return *pin_h & (0x01 << 4);
      break;
    case 8:  //PH5
      return *pin_h & (0x01 << 5);
      break;
    case 9:  //PH6
      return *pin_h & (0x01 << 6);
      break;
    case 10: //PB4
      return *pin_b & (0x01 << 4);
      break;
    case 11: //PB5
      return *pin_b & (0x01 << 5);
      break;
    case 12: //PB6
      return *pin_b & (0x01 << 6);
      break;
    case 13: //PB7
      return *pin_b & (0x01 << 7);
      break;
    case 14: //PJ1
      return *pin_j & (0x01 << 1);
      break;
    case 15: //PJ0
      return *pin_j & (0x01 << 0);
      break;
    case 16: //PH1
      return *pin_h & (0x01 << 1);
      break;
    case 17: //PH0
      return *pin_h & (0x01 << 0);
      break;
    case 18: //PD3
      return *pin_d & (0x01 << 3);
      break;
    case 19: //PD2
      return *pin_d & (0x01 << 2);
      break;
    case 20: //PD1
      return *pin_d & (0x01 << 1);
      break;
    case 21: //PD0
      return *pin_d & (0x01 << 0);
      break;
    case 22: //PA0
      return *pin_a & (0x01 << 0);
      break;
    case 23: //PA1
      return *pin_a & (0x01 << 1);
      break;
    case 24: //PA2
      return *pin_a & (0x01 << 2);
      break;
    case 25: //PA3
      return *pin_a & (0x01 << 3);
      break;
    case 26: //PA4
      return *pin_a & (0x01 << 4);
      break;
    case 27: //PA5
      return *pin_a & (0x01 << 5);
      break;
    case 28: //PA6
      return *pin_a & (0x01 << 6);
      break;
    case 29: //PA7
      return *pin_a & (0x01 << 7);
      break;
    case 30: //PC7
      return *pin_c & (0x01 << 7);
      break;
    case 31: //PC6
      return *pin_c & (0x01 << 6);
      break;
    case 32: //PC5
      return *pin_c & (0x01 << 5);
      break;
    case 33: //PC4
      return *pin_c & (0x01 << 4);
      break;
    case 34: //PC3
      return *pin_c & (0x01 << 3);
      break;
    case 35: //PC2
      return *pin_c & (0x01 << 2);
      break;
    case 36: //PC1
      return *pin_c & (0x01 << 1);
      break;
    case 37: //PC0
      return *pin_c & (0x01 << 0);
      break;
    case 38: //PD7
      return *pin_d & (0x01 << 7);
      break;
    case 39: //PG2
      return *pin_g & (0x01 << 2);
      break;
    case 40: //PG1
      return *pin_g & (0x01 << 1);
      break;
    case 41: //PG0
      return *pin_g & (0x01 << 0);
      break;
    case 42: //PL7
      return *pin_l & (0x01 << 7);
      break;
    case 43: //PL6
      return *pin_l & (0x01 << 6);
      break;
    case 44: //PL5
      return *pin_l & (0x01 << 5);
      break;
    case 45: //PL4
      return *pin_l & (0x01 << 4);
      break;
    case 46: //PL3
      return *pin_l & (0x01 << 3);
      break;
    case 47: //PL2
      return *pin_l & (0x01 << 2);
      break;
    case 48: //PL1
      return *pin_l & (0x01 << 1);
      break;
    case 49: //PL0
      return *pin_l & (0x01 << 0);
      break;
    case 50: //PB3
      return *pin_b & (0x01 << 3);
      break;
    case 51: //PB2
      return *pin_b & (0x01 << 2);
      break;
    case 52: //PB1
      return *pin_b & (0x01 << 1);
      break;
    case 53: //PB0
      return *pin_b & (0x01 << 0);
      break;
    default: 
      mySerial_println("Error in myDigitalRead: invalid pin");
      return false;
      break;
  }
  return false;
}

void myDelay(unsigned int msDelayed)
{
  for (int i = 0; i < msDelayed; i++)
  {
    waveform(16000); 
    //the ticks for 1 ms delay. 
    //f_wave = 500 Hz => T_wave = 0.002 s
    //T_wave/2 = 0.001 s
    //f_clk = 16 MHz => T_clk = 0.0000000625
    //(prescaler = 1)
    //ticks = (T_wave/2) / (T_clk) = 16000
    //one clock cycle takes 0.0000000625 s. 
    //16000 ticks = 16000 clock cycles
    //16000 * 0.0000000625 s = 0.001 s = 1 ms
  }
}

//used in myDelay()
void waveform(unsigned int ticks)
{ 
  *myTCCR1A = 0x00;   // normal mode
  //*myTCCR1B = 0x00;   // stop timer
  // stop the timer (set prescaler to 000)
  *myTCCR1B &= 0b11111000; 
  // reset TOV (write a 1 to reset to 0)        
  *myTIFR1 |= 0b00000001;
  // set the counts
  *myTCNT1 = (unsigned int) (65536 - ticks); //*myTCNT1 = (unsigned int) (65536 - ticks);
  // start the timer
  *myTCCR1B |= 0x01;
  // wait for overflow
  while((*myTIFR1 & 0x01) == 0);
  // stop the timer (set prescaler to 000)
  *myTCCR1B &= 0b11111000;   
  // reset TOV (write a 1 to reset to 0)        
  *myTIFR1 |= 0b00000001;
}

/*
TO DO: make sure this works with other baudrates
*/
void mySerial_begin(unsigned long U0baud)
{
  unsigned long FCPU = 16000000;
  unsigned int tbaud;
  tbaud = (FCPU / 16 / U0baud - 1);
  // Same as (FCPU / (16 * U0baud)) - 1;
  *myUCSR0A = 0x20; //103
  *myUCSR0B = 0x18;
  *myUCSR0C = 0x06;
  *myUBRR0  = tbaud;
}

/*
TO DO: make sure this works when returning int as opposed to unsigned char
*/
int mySerial_available()
{
  if (*myUCSR0A & RDA)   // check RXC0 bit (bit 7)
  {
    return 1; // data available
  }
  else
  {
    return 0; // no data
  }
}

//works with individual characters same as Serial.read
unsigned char mySerial_read()
{
  unsigned char ch;
  while(!(*myUCSR0A & 0b10000000));
  ch = *myUDR0;
  return ch;
}

//writes an individual character
void writeChar(unsigned char U0pdata)
{
  while(!(*myUCSR0A & TBE)); //checking if buffer is empty (0b00100000)
  *myUDR0 = U0pdata;
}

void mySerial_print(const char* toPrint)
{
  for (int i = 0; i < strlen(toPrint); i++)
  {
    writeChar(toPrint[i]);
  }
}

void mySerial_print(const char toPrint)
{
  writeChar(toPrint);
}

void mySerial_print(const int toPrint)
{
  //on the ATMega, ints are 16 bits
  //range of values -32768 to 32767
  //string length is 10, so this is 
  //also fine for ints, but not longs
  char buffer[10];   // enough for 16 bits
  itoa(toPrint, buffer, 10);
  mySerial_print(buffer); //a call to mySerial_print(const char*)
}

void mySerial_print(const long toPrint)
{
  //string length is 12, so this is 
  //fine for long, and 32 bit ints
  char buffer[12];   // enough for 32-bit long
  ltoa(toPrint, buffer, 10);
  mySerial_print(buffer); //a call to mySerial_print(const char*)
}

void mySerial_print(const bool toPrint)
{
  if (toPrint)
  {
    mySerial_print("true");
  }
  else 
  {
    mySerial_print("false");
  }
}

void mySerial_print(const double toPrint)
{
  char buffer[20];  // enough for most cases
  dtostrf(toPrint, 0, 6, buffer);  
  mySerial_print(buffer);
}

void mySerial_println(const char* toPrint)
{
  mySerial_print(toPrint);
  mySerial_print('\n');
}

void mySerial_println(const char toPrint)
{
  mySerial_print(toPrint);
  mySerial_print('\n');
}

void mySerial_println(const int toPrint)
{
  mySerial_print(toPrint);
  mySerial_print('\n');
}

void mySerial_println(const long toPrint)
{
  mySerial_print(toPrint);
  mySerial_print('\n');
}

void mySerial_println(const bool toPrint)
{
  mySerial_print(toPrint);
  mySerial_print('\n');
}

void mySerial_println(const double toPrint)
{
  mySerial_print(toPrint);
  mySerial_print('\n');
}

void mySerial_write(const char toPrint)
{
  mySerial_print(toPrint);
}

void mySerial_write(const char* toPrint)
{
  mySerial_print(toPrint);
}






