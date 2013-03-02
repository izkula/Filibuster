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
 * when			who		what/why
 *
 **************************************************************/

/*---------------- Includes ---------------------------------*/
//#include /*include the module's .h file*/

/*---------------- Module Public Functions ---------------------------*/

/*---------------- Module Private Functions ---------------------------*/

/*---------------- Test Harness ---------------------------*/

#ifdef TESTING

void main(void) {
    //write test harness here
}

#endif /* TESTING */


//--------- Isaac's beacon sense stuff---------------//

/**************************************************************
 *  File:      BeaconSense.pde
 *  Contents:  This module digitally determines the frequency of a periodic input
 *             signal. It does so by attaching an interrupt function to the input pin
 *             that stores the time of each rising edge of the input signal,
 *             and then calculating the frequency based on these times.
 *             Based on work by Scott Cauble.
 * 
 *  Notes:    Target: Arduino UNO R1 & R2
 *            Arduino IDE version: 0022
 * 
 *  History:
 *  when      who  what/why
 *  ----      ---  -------------------------------------------
 *  02/18/13  IVK  program created
 **************************************************************/

/*---------------- Includes ---------------------------------*/
#include <Timers.h>
#include "sensors.h"


/*---------------- Arduino Pins ---------------------------*/
#define IRFREQreadpin 2 //interrupt pin #0, digital pin #2

//Output to multiplexer
// #define s0 4 //The inputs from the arduino to the multiplexer
// #define s1 5 // to set which pin is being read
// #define s2 6
// #define s3 7

#define s0 A0 //The inputs from the arduino to the multiplexer
#define s1 A1 // to set which pin is being read
#define s2 A2
#define s3 A3

#define LEDTestPin 12



/*---------------- Multiplex Pins ---------------------------*/
#define backBeaconSensor  4
#define backRightBeaconSensor  13
#define frontRightBeaconSensor  10
#define frontBeaconSensor  8
#define frontLeftBeaconSensor  7
#define backLeftBeaconSensor  3


int numBeaconSensors = 6;
int beaconSensorMultiplexPins[] = {frontBeaconSensor, frontRightBeaconSensor,
backRightBeaconSensor, backBeaconSensor,
backLeftBeaconSensor, frontLeftBeaconSensor};




/*---------------- Module Defines ---------------------------*/
#define numPulsesToAverage 9 //length of buffer that stores clock times from the interrupt function. This must be at least 3.
#define MAXpulsebufferlength 200000 //in microseconds. max time between first and last entry in pulseBuffer.
//if the span of the pulseBuffer is greater than this than the beacon
//was likely turned off during detection
#define MAXpulseactualtime 1000000 //in microseconds. Ensure that all pulseBuffer values have occured within
// the last second.



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
void PULSEFREQ_detected(void); //interrupt callback function
unsigned long PULSEFREQ_period(void);
int PULSEFREQ_frequency(void);
int PULSEFREQ_beacontype(int pulsefreq);

/* Variables for testing */
volatile int state = HIGH;
boolean debug = true;
int frequency = 850;



void Sensors_Initialize() {
    pinMode(IRFREQreadpin, INPUT);
    
    //Pins to controller the multiplexer
    pinMode(s0, OUTPUT);
    pinMode(s1, OUTPUT);
    pinMode(s2, OUTPUT);
    pinMode(s3, OUTPUT);

    PULSEFREQ_startscanning();
}

// void setup() {
//     pinMode(IRFREQreadpin, INPUT);
//     
//     //Pins to controller the multiplexer
//     pinMode(s0, OUTPUT);
//     pinMode(s1, OUTPUT);
//     pinMode(s2, OUTPUT);
//     pinMode(s3, OUTPUT);
//     
//     //For testing
//     pinMode(LEDTestPin, OUTPUT);
//     //pinMode(4, OUTPUT);
//     //tone(LEDTestPin, frequency); //Outputs a square wave to test the program
//     
//     
//     PULSEFREQ_startscanning();
//     Serial.begin(9600);
//     Serial.println("Starting beaconSense...");
// }
// 
// void loop() {
//     angleToBeacon();
//     /*
//      *  //Serial.println(beaconSensorCounter);
//      *  delay(20);
//      * 
//      * 
//      *  int freq = PULSEFREQ_frequency();
//      *  if (freq > 0) {
//      *    Serial.print(beaconSensorCounter);
//      *    printBeaconSensor(beaconSensorCounter);
//      * //    Serial.print(" Frequency: ");
//      * //    Serial.println(freq);
//      }
//      if ((830 < freq && freq < 870)) { // || (320 < freq && 360 > freq)) {
// 	 Serial.println("YES");
// 	 digitalWrite(LEDTestPin, HIGH);
//      } else if ((3900 < freq && freq < 4100)) {   //  || (1900 < freq && 2100 > freq)) {
// 	 Serial.println("YES");
// 	 digitalWrite(LEDTestPin, HIGH);
//      } else {
// 	 Serial.println("NO");
// 	 digitalWrite(LEDTestPin, LOW);
//      }
//      */
// }
     



void printBeaconSensor(int beaconSensorNumber) {
    switch(beaconSensorNumber) {
	case 0:
	    Serial.print("  Front       ");
	    break;
	case 1:
	    Serial.print("  Front Right ");
	    break;
	case 2:
	    Serial.print("  Back Right  ");
	    break;
	case 3:
	    Serial.print("  Back        ");
	    break;
	case 4:
	    Serial.print("  Back Left   ");
	    break;
	case 5:
	    Serial.print("  Front Left   ");
	    break;

    }

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
    PULSEFREQ_scanning = false;
}
     
unsigned long PULSEFREQ_period(void) { //in microseconds
     //Calculate the average period. This will deal with the case of two beacons overlapping.
     
     unsigned long sum = 0; //sum all pulse times in the pulseBuffer
     if(PULSEFREQ_scanning != true) {
	 //Serial.println("Interrupt not currently turned on. Cannot calculate PULSEFREQ_period.");
	 return 0;
     }
     PULSEFREQ_stopscanning();
     //We are doing a simple version of averaging = (last - first)/sizeOfBuffer
     unsigned long time_span_of_buffer = PULSEFREQ_pulsestream[(PULSEFREQ_bufferindex - 1 + numPulsesToAverage)
     %numPulsesToAverage]
     - PULSEFREQ_pulsestream[PULSEFREQ_bufferindex];
     unsigned long actual_time_span = micros() - PULSEFREQ_pulsestream[PULSEFREQ_bufferindex]; //This is to ensure that the buffer was actually recently written to, that the values are not old
     
     if (time_span_of_buffer > MAXpulsebufferlength) {
	 //Serial.println("Time between detected rising edges is too long.");
	 PULSEFREQ_startscanning();
	 return 0;
     }
     
     if (actual_time_span > MAXpulseactualtime) {
	 Serial.println("Time since last detected rising edge has been too long.");
	 PULSEFREQ_startscanning();
	 return 0;
     }
     
     //To account for overlapping beacons, determine the average period between three pulses,
     // calculated starting at five consecutive indices. If all five average periods match
     // then we are seeing just one beacon. If they do not all match, then we are seeing two beacons.
     // STILL NEED TO TEST THIS WITH TWO BEACONS!!
     
     boolean accountForOverlappingBeacons = false; //This part may not completely work, so set to false to revert to older, robust version
     if(accountForOverlappingBeacons) {
	 
	 int numPulsesInSubset = 3;
	 int numSubsets = numPulsesToAverage - numPulsesInSubset;
	 unsigned long avg_periods[numSubsets];
	 for (int i=0; i<numSubsets; i++) {
	     avg_periods[i] = (PULSEFREQ_pulsestream[(PULSEFREQ_bufferindex + i + numPulsesInSubset + numPulsesToAverage)%numPulsesToAverage] -
	     PULSEFREQ_pulsestream[(PULSEFREQ_bufferindex + i + numPulsesToAverage)%numPulsesToAverage]);
	     avg_periods[i] = avg_periods[i]/(numPulsesInSubset);
	 }
	 
	 int errorMargin = 10; //How far apart two consecutive measured periods can be (in microseconds).
	 for (int k=1; k<numSubsets; k++) {
	     // Serial.print("periods: "); Serial.print(avg_periods[k-1]); Serial.print(" "); Serial.println(avg_periods[k]);
	     if ( ( avg_periods[k-1] > avg_periods[k] && (avg_periods[k-1] - avg_periods[k]) > errorMargin )
		 || ( avg_periods[k] > avg_periods[k-1] && (avg_periods[k] - avg_periods[k-1]) > errorMargin ) ){
		 Serial.println("There are two beacons!");
	     PULSEFREQ_startscanning();
	     return BOTH; //With BOTH set at 3, we are assuming that we will never see a signal with a period of 3 microseconds (or 333kHz)
		 }
	 }
	 PULSEFREQ_startscanning();
	 return avg_periods[0];
     } else {
	 // This line below does not account for the case of overlapping beacons
	 unsigned long average_time_between_pulses = time_span_of_buffer/(numPulsesToAverage-1);
	 PULSEFREQ_startscanning();
	 return average_time_between_pulses;
     }
}

int PULSEFREQ_frequency(void) {
    if(PULSEFREQ_scanning != true) {
	Serial.println("Interrupt not currently turned on. Cannot calculate PULSEFREQ_frequency.");
	return 0;
    }
    
    unsigned long avgperiod = PULSEFREQ_period();
    
    if (avgperiod == BOTH) {
	return BOTH; //Again, with BOTH set at 3, we are assuming that we will never see a signal with a period of 3 microseconds (or 333kHz)
    }
    if(( int(avgperiod) < 100000 ) && (int(avgperiod) > 0)) {
	int freq = 1000000/int(avgperiod); //remember that the period was in microseconds
	return freq;
    } else {
	return 0;
    }
}

int PULSEFREQ_beacontype(void) {
    int freq = PULSEFREQ_frequency();
    
    if (freq == BOTH) {
	return BOTH;
    }
    if ((830 < freq && freq < 870)) { // || (320 < freq && 360 > freq)) {
	    return ENEMY;
    } else if ((3900 < freq && freq < 4100)) {   //  || (1900 < freq && 2100 > freq)) {
	    return SEQUESTER;
    } else {
	return NEITHER;
    }
 }
    
    
    
void setMultiplex(int multiplex_pin_number) {
	
    switch(multiplex_pin_number) {
	case 0:
	    digitalWrite(s0, LOW);
	    digitalWrite(s1, LOW);
	    digitalWrite(s2, LOW);
	    digitalWrite(s3, LOW);
	    break;
	case 1:
	    digitalWrite(s0, HIGH);
	    digitalWrite(s1, LOW);
	    digitalWrite(s2, LOW);
	    digitalWrite(s3, LOW);
	    break;
	case 2:
	    digitalWrite(s0, LOW);
	    digitalWrite(s1, HIGH);
	    digitalWrite(s2, LOW);
	    digitalWrite(s3, LOW);
	    break;
	case 3:
	    digitalWrite(s0, HIGH);
	    digitalWrite(s1, HIGH);
	    digitalWrite(s2, LOW);
	    digitalWrite(s3, LOW);
	    break;
	case 4:
	    digitalWrite(s0, LOW);
	    digitalWrite(s1, LOW);
	    digitalWrite(s2, HIGH);
	    digitalWrite(s3, LOW);
	    break;
	case 5:
	    digitalWrite(s0, HIGH);
	    digitalWrite(s1, LOW);
	    digitalWrite(s2, HIGH);
	    digitalWrite(s3, LOW);
	    break;
	case 6:
	    digitalWrite(s0, LOW);
	    digitalWrite(s1, HIGH);
	    digitalWrite(s2, HIGH);
	    digitalWrite(s3, LOW);
	    break;
	case 7:
	    digitalWrite(s0, HIGH);
	    digitalWrite(s1, HIGH);
	    digitalWrite(s2, HIGH);
	    digitalWrite(s3, LOW);
	    break;
	case 8:
	    digitalWrite(s0, LOW);
	    digitalWrite(s1, LOW);
	    digitalWrite(s2, LOW);
	    digitalWrite(s3, HIGH);
	    break;
	case 9:
	    digitalWrite(s0, HIGH);
	    digitalWrite(s1, LOW);
	    digitalWrite(s2, LOW);
	    digitalWrite(s3, HIGH);
	    break;
	case 10:
	    digitalWrite(s0, LOW);
	    digitalWrite(s1, HIGH);
	    digitalWrite(s2, LOW);
	    digitalWrite(s3, HIGH);
	    break;
	case 11:
	    digitalWrite(s0, HIGH);
	    digitalWrite(s1, HIGH);
	    digitalWrite(s2, LOW);
	    digitalWrite(s3, HIGH);
	    break;
	case 12:
	    digitalWrite(s0, LOW);
	    digitalWrite(s1, LOW);
	    digitalWrite(s2, HIGH);
	    digitalWrite(s3, HIGH);
	    break;
	case 13:
	    digitalWrite(s0, HIGH);
	    digitalWrite(s1, LOW);
	    digitalWrite(s2, HIGH);
	    digitalWrite(s3, HIGH);
	    break;
	case 14:
	    digitalWrite(s0, LOW);
	    digitalWrite(s1, HIGH);
	    digitalWrite(s2, HIGH);
	    digitalWrite(s3, HIGH);
	    break;
	case 15:
	    digitalWrite(s0, HIGH);
	    digitalWrite(s1, HIGH);
	    digitalWrite(s2, HIGH);
	    digitalWrite(s3, HIGH);
	    break;
	default:
	    digitalWrite(s0, LOW);
	    digitalWrite(s1, LOW);
	    digitalWrite(s2, LOW);
	    digitalWrite(s3, LOW);
	    break;
    }
}

//returns -1 if no sensors tripped.
// int Sensors_AngleToBeacon(void) {
//     bool sensorValue[numBeaconSensors];
//     int beaconSensorCounter = 0; //to iterate through the detectors through the multiplexer
//     
//     for(int i = 0; i< numBeaconSensors; i++) {
// 	beaconSensorCounter = i;
// 	setMultiplex(beaconSensorMultiplexPins[beaconSensorCounter]);
// 	delay(50);
// 	sensorValue[i] = PULSEFREQ_beacontype();
// 	printBeaconSensor(i);
// 	Serial.print(sensorValue[i]);
//     }
//     Serial.println();
//     
//     float angle = 0.0;
//     int numOnSensors = 0;
//     int sensorValueOn = 0;
//     float dDegrees = 360.0/numBeaconSensors;
//     for (int i=0; i < numBeaconSensors; i++) {
// 	if( sensorValue[i] != 0) {
// 	    numOnSensors += 1;
// 	    sensorValueOn = 1;
// 	    angle += (i*dDegrees);
// 	    if (i == (numBeaconSensors - 1) && sensorValue[0] != 0) {
// 		angle += 360.0;
// 	    }
// 	} else {
// 	    sensorValueOn = 0;
// 	}
//     }
//     float avgAngle = angle/numOnSensors;
//     Serial.println(avgAngle);
//     return avgAngle;
// }

//return -1 if no sensors on.
int Sensors_AngleToBeacon(void) {
    bool sensorValue[numBeaconSensors];
    int beaconSensorCounter = 0; //to iterate through the detectors through the multiplexer
    
    for(int i = 0; i< numBeaconSensors; i++) {
	beaconSensorCounter = i;
	setMultiplex(beaconSensorMultiplexPins[beaconSensorCounter]);
	delay(50);
	sensorValue[i] = PULSEFREQ_beacontype();
	printBeaconSensor(i);
	Serial.print(sensorValue[i]);
    }
    Serial.println();
    
    float angle = 0.0;
    int numOnSensors = 0;
    int sensorValueOn = 0;
    float dDegrees = 360.0/numBeaconSensors;
    for (int i=0; i < numBeaconSensors; i++) {
	if( sensorValue[i] != 0) {
	    numOnSensors += 1;
	    sensorValueOn = 1;
	    angle += (i*dDegrees);
	    if (i == (numBeaconSensors - 1) && sensorValue[0] != 0) {
		angle += 360.0;
	    }
	} else {
	    sensorValueOn = 0;
	}
    }
    float avgAngle = angle/numOnSensors;

    if (numOnSensors == 0) {
	avgAngle = -1;
    }

    Serial.println(avgAngle);
    

    return avgAngle;
}

// int Sensors_AngleToBeacon() {
//     int numSensorsOn;
//     
//     for (int i = 0; i < numBeaconSensors; i++) {
// 	setMultiplex(beaconSensorMultiplexPins[i]);
// 	int 
//     }
// }

//return -1 if never read
int Sensors_AvgAngleToBeacon(void) {
    int angleSum = 0;
    int numAngles = 0;
    
    for (int i = 0; i < 10; i++) {
	int angle = Sensors_AngleToBeacon();

	if (angle != -1) {
	    angleSum += angle;
	    numAngles++;
	}

    }

    float avgAngle = (float) angleSum / numAngles;
    
}

// Returns if the enemy is in front of one of the beacons
// Returns 0 if no enemy in front. Returns a non-zero code if yes (ENEMY = 2)
int isEnemyInFrontBeacon(){
    
    // Manually check if a beacon is seeing enemey
    
    int beaconNumber = 1; // The beacon number on the multiplexer
    //JS: Beacon # 1 is the front right relative to breadboard but physcially on the robot #1 is the front
    
    setMultiplex(beaconSensorMultiplexPins[beaconNumber]);
    //delay(50);
    
    int sensorValue = PULSEFREQ_beacontype();
    Serial.println(sensorValue);
    
    return sensorValue;
}