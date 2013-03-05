#pragma once

//#define TESTING //comment this out to use test harness

/**************************************************************
 * Filename:
 * Contents:
 * 
 * Notes:    Target: Arduino UNO R1 & R2
 * Arduino IDE version: 0022
 * 
 * History:
 * when		who  		what/why
 * 
 **************************************************************/

/*---------------- Includes ---------------------------------*/
#include <Timers.h>
#include "definitions.h"

/*---------------- Module Level Definitions ---------------------------------*/

//Pin assignments------

    //Sensor->Multiplexer Pins assignments------
    
    #define backBeaconSensor  4
    #define backRightBeaconSensor  13
    #define frontRightBeaconSensor  10
    #define frontBeaconSensor  8
    #define frontLeftBeaconSensor  7
    #define backLeftBeaconSensor  3

    //Arduino Pin assignments:--------
    
    //interrupt pin #0, digital pin #2
    #define IRFREQreadpin 2
    //Arduino->Multiplexor pins corresponding to the 4 bits of multiplexer input to select a channel.
    #define s0 A0 //multiplex select bit 0
    #define s1 A1
    #define s2 A2
    #define s3 A3

    //Indicator LED pin for testing
    #define LEDTestPin 12

//General defines---------

//number of phototransistor sensors on the detector.
#define NUM_BEACON_SENSORS 6
//length of buffer that stores clock times from the interrupt function. This must be at least 3.
#define NUM_PULSES_TO_AVERAGE 9
//max time between first and last entry in pulseBuffer in microseconds.
#define MAX_PULSE_BUFFER_TIME 200000
//if the span of the pulseBuffer is greater than this than the beacon was likely turned off during detection

//Used to ensure all pulseBuffer values are fresh. If the times are more than a second old, don't use them.
#define MAX_PULSE_BUFFER_ACTUAL_TIME 1000000

// //To be used to identify which types of target are visible. To be returned by BeaconSense_VisibleTargets()
#define BOTH 3
#define SEQUESTER 1
#define ENEMY 2
#define NEITHER 0

/*---------------- Module Level Variables ---------------------------*/

//array of multiplexer pins connected to their corresponding sensors in clockwise order.
static int beaconSensorMultiplexPins[] = {frontBeaconSensor, frontRightBeaconSensor,
backRightBeaconSensor, backBeaconSensor,
backLeftBeaconSensor, frontLeftBeaconSensor};

//the circular buffer of pulse interrupt times (updated by PulseDetectedISR() ).
static unsigned long pulseBuffer[NUM_PULSES_TO_AVERAGE];
//the index of the oldest value in the pulse buffer.
static int pulseBufferIndex = 0;
//Whether or not the beacon detector is enabled. 'true' if the detector is enabled
static boolean scanning = false;

/* Variables for testing */
static volatile int state = HIGH;
static boolean debug = true;
static int frequency = 850;

/*---------------- Module Public Function Prototypes ---------------------------*/
int Sensors_AngleToBeacon(void);
void BeaconSense_Initialize();
void BeaconSense_StartScanning(void);
void BeaconSense_StopScanning(void);
int BeaconSense_VisibleTargets(int pulsefreq);
int isEnemyInFrontBeacon();

/*---------------- Module Private Function Prototypes ---------------------------*/
static void PulseDetectedISR(void); //interrupt callback function
static unsigned long GetPeriod(void);
static int GetFreq(void);
static void setMultiplexChannel(int channel);

