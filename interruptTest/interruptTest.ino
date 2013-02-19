/**************************************************************
  File:      interruptTest.pde 
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
#define numPulsesToAverage 3 //length of buffer that stores clock times from the interrupt function
#define IRFREQreadpin 2 //interrupt pin #0, digital pin #2
#define MAXpulsebufferlength 200000 //in microseconds?. max time between first and last entry in pulseBuffer.
                                      //if the span of the pulseBuffer is greater than this than the beacon
                                      //was likely turned off during detection
#define MAXpulseactualtime 1000000 //in microseconds. Ensure that all pulseBuffer values have occured within
                                     // the last second.
                                     
//TO IDENTIFY BEACON TYPE
#define SEQUESTER 1
#define ENEMY 2
#define NEITHER 0

/* REQUIRED module-level variables */
unsigned long PULSEFREQ_pulsebuffer[numPulsesToAverage]; //the pulseBuffer array of interrupt times
char PULSEFREQ_bufferindex = 0; //the index of the oldest value in the array
boolean PULSEFREQ_scanning = false; // 'true' if the interrupt is enabled

/* Function prototypes */
void PULSEFREQ_startscanning(void);
void PULSEFREQ_stopscanning(void);
void PULSEFREQ_detected(void);
unsigned long PULSEFREQ_period(void);
int PULSEFREQ_frequency(void);
int PULSEFREQ_beacontype(int pulsefreq);


void setup()
{
  pinMode(IRFREQreadpin, INPUT);
  pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);
  attachInterrupt(IRFREQreadpin, respondToInterrupt, RISING);
}

void loop()
{
  analogWrite(5, 123);
  digitalWrite(4, LOW);
}

void respondToInterrupt()
{
  digitalWrite(4, HIGH);
}
  
  
