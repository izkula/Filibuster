/**************************************************************
  File:      interruptTest.pde 
  Contents:  This program demonstrates how to use the interrupt feature
            of an arduino. Notice that pin2 = interrupt pin #0

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
#define IRFREQreadpin 2 //interrupt pin #0, digital pin #2
            
volatile int state = HIGH;           
                                     
void setup()
{
  pinMode(IRFREQreadpin, INPUT);
  pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);
  attachInterrupt(IRFREQreadpin-2, respondToInterrupt, RISING);
  tone(5, 850);
}

void loop()
{
  digitalWrite(4, state);
}

void respondToInterrupt()
{
  state = !state;
}
  
  
