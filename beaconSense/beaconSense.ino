/**************************************************************
  File:      BeaconSense.pde 
  Contents:  This module digitally determines the frequency of a periodic input
             signal. It does so by attaching an interrupt function to the input pin
             that stores the time of each rising edge of the input signal,
             and then calculating the frequency based on these times. 
             Based on work by Scott Cauble.

  Notes:    Target: Arduino UNO R1 & R2
            Arduino IDE version: 0022

  History:
  when      who  what/why
  ----      ---  -------------------------------------------
  02/18/13  IVK  program created
**************************************************************/

/*---------------- Includes ---------------------------------*/
#include <Timers.h>

 
/*---------------- Module Defines ---------------------------*/
#define numPulsesToAverage 9 //length of buffer that stores clock times from the interrupt function
#define IRFREQreadpin 2 //interrupt pin #0, digital pin #2
#define MAXpulsebufferlength 200000 //in microseconds. max time between first and last entry in pulseBuffer.
                                      //if the span of the pulseBuffer is greater than this than the beacon
                                      //was likely turned off during detection
#define MAXpulseactualtime 1000000 //in microseconds. Ensure that all pulseBuffer values have occured within
                                     // the last second.
                                     
                                 
#define LEDTestPin 13                                     

//TO IDENTIFY BEACON TYPE
#define BOTH 3 //Need to be able to account for both beacons being in field of view
#define SEQUESTER 1
#define ENEMY 2
#define NEITHER 0

/* REQUIRED module-level variables */
unsigned long PULSEFREQ_pulsestream[numPulsesToAverage]; //the pulseBuffer array of interrupt times
int PULSEFREQ_bufferindex = 0; //the index of the oldest value in the array
boolean PULSEFREQ_scanning = false; // 'true' if the interrupt is enabled

/* Function prototypes */
void PULSEFREQ_startscanning(void);
void PULSEFREQ_stopscanning(void);
void PULSEFREQ_detected(void);
unsigned long PULSEFREQ_period(void);
int PULSEFREQ_frequency(void);
int PULSEFREQ_beacontype(int pulsefreq);

/* Variables for testing */
volatile int state = HIGH;
boolean debug = true;
int frequency = 4000;

void setup()
{
  pinMode(IRFREQreadpin, INPUT);
  
  pinMode(LEDTestPin, OUTPUT);
  pinMode(4, OUTPUT);
  tone(LEDTestPin, frequency);

  
  PULSEFREQ_startscanning();
  Serial.begin(9600);
  Serial.println("Starting beaconSense...");


}

void loop()
{
  int freq = PULSEFREQ_frequency();
  if (freq > 0) {
    Serial.print("Frequency: ");
    Serial.println(freq);
  }
  delay(100);
}

void PULSEFREQ_detected() { //callback function that stores the time of the interrupt
  PULSEFREQ_pulsestream[PULSEFREQ_bufferindex] = micros();
  PULSEFREQ_bufferindex = (PULSEFREQ_bufferindex + 1) % numPulsesToAverage;
}

void PULSEFREQ_startscanning(void) {
  PULSEFREQ_bufferindex = 0; //set initial psoition in the pulse buffer array to 0
  PULSEFREQ_scanning = true;
  attachInterrupt(IRFREQreadpin-2, PULSEFREQ_detected, RISING); //Pin #2 is interrupt #0
}

void PULSEFREQ_stopscanning(void) {
  detachInterrupt(IRFREQreadpin -2 );
}

unsigned long PULSEFREQ_period(void) { //in microseconds
  unsigned long sum = 0; //sum all pulse times in the pulseBuffer
  if(PULSEFREQ_scanning != true) {
    Serial.println("Interrupt not currently turned on. Cannot calculate PULSEFREQ_period.");
    return 0;
  }
  PULSEFREQ_stopscanning();
  unsigned long time_span_of_buffer = PULSEFREQ_pulsestream[(PULSEFREQ_bufferindex - 1 + numPulsesToAverage)
                                      %numPulsesToAverage]
                                      - PULSEFREQ_pulsestream[PULSEFREQ_bufferindex];
  unsigned long actual_time_span = micros() - PULSEFREQ_pulsestream[PULSEFREQ_bufferindex];

  Serial.print(time_span_of_buffer);
  Serial.print(" ");
  Serial.print(PULSEFREQ_bufferindex);
  Serial.print(" ");
  if (time_span_of_buffer > MAXpulsebufferlength) {
    Serial.println("Time between detected rising edges is too long.");
    PULSEFREQ_startscanning();
    return 0;
  }
   
  if (actual_time_span > MAXpulseactualtime) {
    Serial.println("Time since last detected rising edge has been too long.");
    PULSEFREQ_startscanning();
    return 0;
  }  

  unsigned long average_time_between_pulses = time_span_of_buffer/(numPulsesToAverage-1);
  
  PULSEFREQ_startscanning();
  return average_time_between_pulses;
}

int PULSEFREQ_frequency(void) {
  if(PULSEFREQ_scanning != true) {
    Serial.println("Interrupt not currently turned on. Cannot calculate PULSEFREQ_frequency.");
    return 0;
  }
  
  unsigned long avgperiod = PULSEFREQ_period();
  
  if(( int(avgperiod) < 100000 ) && (int(avgperiod) > 0)) {
      int freq = 1000000/int(avgperiod);
      return freq;
  } else {
    return 0;
  }
}
  
  
int PULSEFREQ_beacontype(void) {
  int freq = PULSEFREQ_frequency();
  
  if ((830 < freq && freq < 870) || (320 < freq && 360 > freq)) {
    return ENEMY;
  } else if ((4900 < freq && freq < 5100) || (1900 < freq && 2100 > freq)) {
    return SEQUESTER;
  } else {
    return NEITHER;
  }
}

