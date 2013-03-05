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
#include "beaconSense.h"
#include <Arduino.h>

/*---------------- Module Public Functions ---------------------------*/

//Initializes the arduino pin assignments and sets the interrupts
void BeaconSense_Initialize() {
    pinMode(IRFREQreadpin, INPUT);
    
    //Initializes the pins to controller the multiplexer channel selection.
    pinMode(s0, OUTPUT);
    pinMode(s1, OUTPUT);
    pinMode(s2, OUTPUT);
    pinMode(s3, OUTPUT);
    
    BeaconSense_StartScanning();
}

//Attaches the arduino interrupts to start scanning for pulses, and does bookkeeping.
void BeaconSense_StartScanning(void) {
    pulseBufferIndex = 0; //set initial position in the pulse buffer array to 0
    scanning = true;
    attachInterrupt(IRFREQreadpin - 2, PulseDetectedISR, RISING); //Pin #2 is interrupt #0
}

//Detaches the arduino interrupts to start scanning for pulses, and does bookkeeping.
void BeaconSense_StopScanning(void) {
    detachInterrupt(IRFREQreadpin - 2);
    scanning = false;

}

//Returns which targets are visible to the beacon.
int BeaconSense_VisibleTargets(void) {
    int freq = GetFreq();
    
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


/*---------------- Module Private Functions ---------------------------*/

//Callback function that stores the time of the interrupt when a pulse comes in (rising edge).
void PulseDetectedISR() {
    //store the time of the interrupt in the pulse buffer.
    pulseBuffer[pulseBufferIndex] = micros();
    //Advance to next index in circular buffer
    pulseBufferIndex = (pulseBufferIndex + 1) % NUM_PULSES_TO_AVERAGE;
}

//Calculates the average period of pulses in microseconds. This should deal with the case of two beacons overlapping.
unsigned long GetPeriod(void) {
    
    //sum of all pulse times in the pulseBuffer
    unsigned long sum = 0;
    
    //Return an error if not currently scanning for pulses.
    if(scanning != true) {
	Serial.println("Interrupt not currently turned on. Cannot calculate GetPeriod.");
	return 0;
    }
    
    //Temporarily stop scanning for pulses so that incoming interrupts do not modify pulse buffer while analyzing
    BeaconSense_StopScanning();
    
    //Calculate average period of pulses in buffer. We are doing a simple/faster version of averaging = (last - first)/sizeOfBuffer
    unsigned long bufferTimeSpan = pulseBuffer[(pulseBufferIndex - 1 + NUM_PULSES_TO_AVERAGE) % NUM_PULSES_TO_AVERAGE] - pulseBuffer[pulseBufferIndex];
    //This is to ensure that the buffer was actually recently written to, that the values are not old
    unsigned long actualTimeSpan = micros() - pulseBuffer[pulseBufferIndex];
    
    if (bufferTimeSpan > MAX_PULSE_BUFFER_TIME) {
	Serial.println("Time between detected rising edges is too long.");
	BeaconSense_StartScanning();
	return 0;
    }
    
    if (actualTimeSpan > MAX_PULSE_BUFFER_ACTUAL_TIME) {
	Serial.println("Time since last detected rising edge has been too long.");
	BeaconSense_StartScanning();
	return 0;
    }
    
    //To account for overlapping beacons, determine the average period between three pulses,
    // calculated starting at five consecutive indices. If all five average periods match
    // then we are seeing just one beacon. If they do not all match, then we are seeing two beacons.
    // STILL NEED TO TEST THIS WITH TWO BEACONS!!
    
    boolean accountForOverlappingBeacons = false; //This part may not completely work, so set to false to revert to older, robust version
    if(accountForOverlappingBeacons) {
	
	int numPulsesInSubset = 3;
	int numSubsets = NUM_PULSES_TO_AVERAGE - numPulsesInSubset;
	unsigned long avg_periods[numSubsets];
	for (int i=0; i<numSubsets; i++) {
	    avg_periods[i] = (pulseBuffer[(pulseBufferIndex + i + numPulsesInSubset + NUM_PULSES_TO_AVERAGE)%NUM_PULSES_TO_AVERAGE] -
	    pulseBuffer[(pulseBufferIndex + i + NUM_PULSES_TO_AVERAGE)%NUM_PULSES_TO_AVERAGE]);
	    avg_periods[i] = avg_periods[i]/(numPulsesInSubset);
	}
	
	int errorMargin = 10; //How far apart two consecutive measured periods can be (in microseconds).
	for (int k=1; k<numSubsets; k++) {
	    // Serial.print("periods: "); Serial.print(avg_periods[k-1]); Serial.print(" "); Serial.println(avg_periods[k]);
	    if ( ( avg_periods[k-1] > avg_periods[k] && (avg_periods[k-1] - avg_periods[k]) > errorMargin )
		|| ( avg_periods[k] > avg_periods[k-1] && (avg_periods[k] - avg_periods[k-1]) > errorMargin ) ){
		Serial.println("There are two beacons!");
	    BeaconSense_StartScanning();
	    return BOTH; //With BOTH set at 3, we are assuming that we will never see a signal with a period of 3 microseconds (or 333kHz)
		}
	}
	BeaconSense_StartScanning();
	return avg_periods[0];
    } else {
	// This line below does not account for the case of overlapping beacons
	unsigned long avgPeriod = bufferTimeSpan/(NUM_PULSES_TO_AVERAGE-1);
	//Reattaches the pulse interrupts
	BeaconSense_StartScanning();
	return avgPeriod;
    }
}

int GetFreq(void) {
    if(scanning != true) {
	Serial.println("Interrupt not currently turned on. Cannot calculate GetFreq.");
	return 0;
    }
    
    unsigned long avgPeriod = GetPeriod();
    
    if (avgPeriod == BOTH) {
	return BOTH; //Again, with BOTH set at 3, we are assuming that we will never see a signal with a period of 3 microseconds (or 333kHz)
    }
    if(( int(avgPeriod) < 100000 ) && (int(avgPeriod) > 0)) {
	int freq = 1000000/int(avgPeriod); //remember that the period was in microseconds
	return freq;
    } else {
	return 0;
    }
}

//Selects a multiplex channel
void setMultiplexChannel(int channel) {
    
    switch(channel) {
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


/*---------------- Test Harness ---------------------------*/

#ifdef TESTING

void main(void) {
    //write test harness here
}

#endif /* TESTING */

/*---------------- Module Defines ---------------------------*/



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
//     BeaconSense_StartScanning();
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
//      *  int freq = GetFreq();
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







//returns -1 if no sensors tripped.
// int Sensors_AngleToBeacon(void) {
//     bool sensorValue[NUM_BEACON_SENSORS];
//     int beaconSensorCounter = 0; //to iterate through the detectors through the multiplexer
//
//     for(int i = 0; i< NUM_BEACON_SENSORS; i++) {
// 	beaconSensorCounter = i;
// 	setMultiplexChannel(beaconSensorMultiplexPins[beaconSensorCounter]);
// 	delay(50);
// 	sensorValue[i] = BeaconSense_VisibleTargets();
// 	printBeaconSensor(i);
// 	Serial.print(sensorValue[i]);
//     }
//     Serial.println();
//
//     float angle = 0.0;
//     int numOnSensors = 0;
//     int sensorValueOn = 0;
//     float dDegrees = 360.0/NUM_BEACON_SENSORS;
//     for (int i=0; i < NUM_BEACON_SENSORS; i++) {
// 	if( sensorValue[i] != 0) {
// 	    numOnSensors += 1;
// 	    sensorValueOn = 1;
// 	    angle += (i*dDegrees);
// 	    if (i == (NUM_BEACON_SENSORS - 1) && sensorValue[0] != 0) {
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
    bool sensorValue[NUM_BEACON_SENSORS];
    
    int beaconSensorCounter = 0; //to iterate through the detectors through the multiplexer

    for(int i = 0; i< NUM_BEACON_SENSORS; i++) {
	beaconSensorCounter = i;
	setMultiplexChannel(beaconSensorMultiplexPins[beaconSensorCounter]);
	delay(50);
	sensorValue[i] = BeaconSense_VisibleTargets();
	printBeaconSensor(i);
	Serial.print(sensorValue[i]);
    }
    Serial.println();

    float angle = 0.0;
    int numOnSensors = 0;
    int sensorValueOn = 0;
    float dDegrees = 360.0/NUM_BEACON_SENSORS;
    for (int i=0; i < NUM_BEACON_SENSORS; i++) {
	if( sensorValue[i] != 0) {
	    numOnSensors += 1;
	    sensorValueOn = 1;
	    angle += (i*dDegrees);
	    if (i == (NUM_BEACON_SENSORS - 1) && sensorValue[0] != 0) {
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
//     for (int i = 0; i < NUM_BEACON_SENSORS; i++) {
// 	setMultiplexChannel(beaconSensorMultiplexPins[i]);
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

    setMultiplexChannel(beaconSensorMultiplexPins[beaconNumber]);
    //delay(50);

    int sensorValue = BeaconSense_VisibleTargets();
    Serial.println(sensorValue);

    return sensorValue;
}


// #pragma once
// 
// //#define TESTING //comment this out to use test harness
// 
// /**************************************************************
//  * Filename:
//  * Contents:
//  *
//  * Notes:    Target: Arduino UNO R1 & R2
//  * Arduino IDE version: 0022
//  *
//  * History:
//  * when			who		what/why
//  *
//  **************************************************************/
// 
// /*---------------- Includes ---------------------------------*/
// //#include /*include the module's .h file*/
// 
// /*---------------- Module Public Functions ---------------------------*/
// 
// /*---------------- Module Private Functions ---------------------------*/
// 
// /*---------------- Test Harness ---------------------------*/
// 
// #ifdef TESTING
// 
// void main(void) {
//     //write test harness here
// }
// 
// #endif /* TESTING */
// 
// 
// //--------- Isaac's beacon sense stuff---------------//
// 
// /**************************************************************
//  *  File:      BeaconSense.pde
//  *  Contents:  This module digitally determines the frequency of a periodic input
//  *             signal. It does so by attaching an interrupt function to the input pin
//  *             that stores the time of each rising edge of the input signal,
//  *             and then calculating the frequency based on these times.
//  *             Based on work by Scott Cauble.
//  * 
//  *  Notes:    Target: Arduino UNO R1 & R2
//  *            Arduino IDE version: 0022
//  * 
//  *  History:
//  *  when      who  what/why
//  *  ----      ---  -------------------------------------------
//  *  02/18/13  IVK  program created
//  **************************************************************/
// 
// /*---------------- Includes ---------------------------------*/
// #include <Timers.h>
// #include "sensors.h"
// 
// 
// /*---------------- Arduino Pins ---------------------------*/
// #define IRFREQreadpin 2 //interrupt pin #0, digital pin #2
// 
// //Output to multiplexer
// // #define s0 4 //The inputs from the arduino to the multiplexer
// // #define s1 5 // to set which pin is being read
// // #define s2 6
// // #define s3 7
// 
// #define s0 A0 //The inputs from the arduino to the multiplexer
// #define s1 A1 // to set which pin is being read
// #define s2 A2
// #define s3 A3
// 
// #define LEDTestPin 12
// 
// 
// 
// /*---------------- Multiplex Pins ---------------------------*/
// #define backBeaconSensor  4
// #define backRightBeaconSensor  13
// #define frontRightBeaconSensor  10
// #define frontBeaconSensor  8
// #define frontLeftBeaconSensor  7
// #define backLeftBeaconSensor  3
// 
// 
// int NUM_BEACON_SENSORS = 6;
// int beaconSensorMultiplexPins[] = {frontBeaconSensor, frontRightBeaconSensor,
// backRightBeaconSensor, backBeaconSensor,
// backLeftBeaconSensor, frontLeftBeaconSensor};
// 
// 
// 
// 
// /*---------------- Module Defines ---------------------------*/
// #define NUM_PULSES_TO_AVERAGE 9 //length of buffer that stores clock times from the interrupt function. This must be at least 3.
// #define MAX_PULSE_BUFFER_TIME 200000 //in microseconds. max time between first and last entry in pulseBuffer.
// //if the span of the pulseBuffer is greater than this than the beacon
// //was likely turned off during detection
// #define MAX_PULSE_BUFFER_ACTUAL_TIME 1000000 //in microseconds. Ensure that all pulseBuffer values have occured within
// // the last second.
// 
// 
// 
// //TO IDENTIFY BEACON TYPE
// #define BOTH 3 //Need to be able to account for both beacons being in field of view
// #define SEQUESTER 1
// #define ENEMY 2
// #define NEITHER 0
// 
// /* REQUIRED module-level variables */
// unsigned long pulseBuffer[NUM_PULSES_TO_AVERAGE]; //the pulseBuffer array of interrupt times
// int pulseBufferIndex = 0; //the index of the oldest value in the array
// boolean scanning = false; // 'true' if the interrupt is enabled
// 
// /* Function prototypes */
// void BeaconSense_StartScanning(void);
// void BeaconSense_StopScanning(void);
// void PulseDetectedISR(void); //interrupt callback function
// unsigned long GetPeriod(void);
// int GetFreq(void);
// int BeaconSense_VisibleTargets(int pulsefreq);
// 
// /* Variables for testing */
// volatile int state = HIGH;
// boolean debug = true;
// int frequency = 850;
// 
// 
// 
// void Sensors_Initialize() {
//     pinMode(IRFREQreadpin, INPUT);
//     
//     //Pins to controller the multiplexer
//     pinMode(s0, OUTPUT);
//     pinMode(s1, OUTPUT);
//     pinMode(s2, OUTPUT);
//     pinMode(s3, OUTPUT);
// 
//     BeaconSense_StartScanning();
// }
// 
// // void setup() {
// //     pinMode(IRFREQreadpin, INPUT);
// //     
// //     //Pins to controller the multiplexer
// //     pinMode(s0, OUTPUT);
// //     pinMode(s1, OUTPUT);
// //     pinMode(s2, OUTPUT);
// //     pinMode(s3, OUTPUT);
// //     
// //     //For testing
// //     pinMode(LEDTestPin, OUTPUT);
// //     //pinMode(4, OUTPUT);
// //     //tone(LEDTestPin, frequency); //Outputs a square wave to test the program
// //     
// //     
// //     BeaconSense_StartScanning();
// //     Serial.begin(9600);
// //     Serial.println("Starting beaconSense...");
// // }
// // 
// // void loop() {
// //     angleToBeacon();
// //     /*
// //      *  //Serial.println(beaconSensorCounter);
// //      *  delay(20);
// //      * 
// //      * 
// //      *  int freq = GetFreq();
// //      *  if (freq > 0) {
// //      *    Serial.print(beaconSensorCounter);
// //      *    printBeaconSensor(beaconSensorCounter);
// //      * //    Serial.print(" Frequency: ");
// //      * //    Serial.println(freq);
// //      }
// //      if ((830 < freq && freq < 870)) { // || (320 < freq && 360 > freq)) {
// // 	 Serial.println("YES");
// // 	 digitalWrite(LEDTestPin, HIGH);
// //      } else if ((3900 < freq && freq < 4100)) {   //  || (1900 < freq && 2100 > freq)) {
// // 	 Serial.println("YES");
// // 	 digitalWrite(LEDTestPin, HIGH);
// //      } else {
// // 	 Serial.println("NO");
// // 	 digitalWrite(LEDTestPin, LOW);
// //      }
// //      */
// // }
//      
// 
// 
// 
// void printBeaconSensor(int beaconSensorNumber) {
//     switch(beaconSensorNumber) {
// 	case 0:
// 	    Serial.print("  Front       ");
// 	    break;
// 	case 1:
// 	    Serial.print("  Front Right ");
// 	    break;
// 	case 2:
// 	    Serial.print("  Back Right  ");
// 	    break;
// 	case 3:
// 	    Serial.print("  Back        ");
// 	    break;
// 	case 4:
// 	    Serial.print("  Back Left   ");
// 	    break;
// 	case 5:
// 	    Serial.print("  Front Left   ");
// 	    break;
// 
//     }
// 
// }
//      
// void PulseDetectedISR() { //callback function that stores the time of the interrupt
//     pulseBuffer[pulseBufferIndex] = micros();
//     pulseBufferIndex = (pulseBufferIndex + 1) % NUM_PULSES_TO_AVERAGE;
// }
//      
// void BeaconSense_StartScanning(void) {
//     pulseBufferIndex = 0; //set initial psoition in the pulse buffer array to 0
//     scanning = true;
//     attachInterrupt(IRFREQreadpin-2, PulseDetectedISR, RISING); //Pin #2 is interrupt #0
// }
//      
// void BeaconSense_StopScanning(void) {
//     detachInterrupt(IRFREQreadpin -2 );
//     scanning = false;
// }
//      
// unsigned long GetPeriod(void) { //in microseconds
//      //Calculate the average period. This will deal with the case of two beacons overlapping.
//      
//      unsigned long sum = 0; //sum all pulse times in the pulseBuffer
//      if(scanning != true) {
// 	 //Serial.println("Interrupt not currently turned on. Cannot calculate GetPeriod.");
// 	 return 0;
//      }
//      BeaconSense_StopScanning();
//      //We are doing a simple version of averaging = (last - first)/sizeOfBuffer
//      unsigned long time_span_of_buffer = pulseBuffer[(pulseBufferIndex - 1 + NUM_PULSES_TO_AVERAGE)
//      %NUM_PULSES_TO_AVERAGE]
//      - pulseBuffer[pulseBufferIndex];
//      unsigned long actual_time_span = micros() - pulseBuffer[pulseBufferIndex]; //This is to ensure that the buffer was actually recently written to, that the values are not old
//      
//      if (time_span_of_buffer > MAX_PULSE_BUFFER_TIME) {
// 	 //Serial.println("Time between detected rising edges is too long.");
// 	 BeaconSense_StartScanning();
// 	 return 0;
//      }
//      
//      if (actual_time_span > MAX_PULSE_BUFFER_ACTUAL_TIME) {
// 	 Serial.println("Time since last detected rising edge has been too long.");
// 	 BeaconSense_StartScanning();
// 	 return 0;
//      }
//      
//      //To account for overlapping beacons, determine the average period between three pulses,
//      // calculated starting at five consecutive indices. If all five average periods match
//      // then we are seeing just one beacon. If they do not all match, then we are seeing two beacons.
//      // STILL NEED TO TEST THIS WITH TWO BEACONS!!
//      
//      boolean accountForOverlappingBeacons = false; //This part may not completely work, so set to false to revert to older, robust version
//      if(accountForOverlappingBeacons) {
// 	 
// 	 int numPulsesInSubset = 3;
// 	 int numSubsets = NUM_PULSES_TO_AVERAGE - numPulsesInSubset;
// 	 unsigned long avg_periods[numSubsets];
// 	 for (int i=0; i<numSubsets; i++) {
// 	     avg_periods[i] = (pulseBuffer[(pulseBufferIndex + i + numPulsesInSubset + NUM_PULSES_TO_AVERAGE)%NUM_PULSES_TO_AVERAGE] -
// 	     pulseBuffer[(pulseBufferIndex + i + NUM_PULSES_TO_AVERAGE)%NUM_PULSES_TO_AVERAGE]);
// 	     avg_periods[i] = avg_periods[i]/(numPulsesInSubset);
// 	 }
// 	 
// 	 int errorMargin = 10; //How far apart two consecutive measured periods can be (in microseconds).
// 	 for (int k=1; k<numSubsets; k++) {
// 	     // Serial.print("periods: "); Serial.print(avg_periods[k-1]); Serial.print(" "); Serial.println(avg_periods[k]);
// 	     if ( ( avg_periods[k-1] > avg_periods[k] && (avg_periods[k-1] - avg_periods[k]) > errorMargin )
// 		 || ( avg_periods[k] > avg_periods[k-1] && (avg_periods[k] - avg_periods[k-1]) > errorMargin ) ){
// 		 Serial.println("There are two beacons!");
// 	     BeaconSense_StartScanning();
// 	     return BOTH; //With BOTH set at 3, we are assuming that we will never see a signal with a period of 3 microseconds (or 333kHz)
// 		 }
// 	 }
// 	 BeaconSense_StartScanning();
// 	 return avg_periods[0];
//      } else {
// 	 // This line below does not account for the case of overlapping beacons
// 	 unsigned long average_time_between_pulses = time_span_of_buffer/(NUM_PULSES_TO_AVERAGE-1);
// 	 BeaconSense_StartScanning();
// 	 return average_time_between_pulses;
//      }
// }
// 
// int GetFreq(void) {
//     if(scanning != true) {
// 	Serial.println("Interrupt not currently turned on. Cannot calculate GetFreq.");
// 	return 0;
//     }
//     
//     unsigned long avgperiod = GetPeriod();
//     
//     if (avgperiod == BOTH) {
// 	return BOTH; //Again, with BOTH set at 3, we are assuming that we will never see a signal with a period of 3 microseconds (or 333kHz)
//     }
//     if(( int(avgperiod) < 100000 ) && (int(avgperiod) > 0)) {
// 	int freq = 1000000/int(avgperiod); //remember that the period was in microseconds
// 	return freq;
//     } else {
// 	return 0;
//     }
// }
// 
// 
// //TODO: changed it to ignore everything but enemy for beating brick. Change it back.
// int BeaconSense_VisibleTargets(void) {
//     int freq = GetFreq();
//     
//     if (freq == BOTH) {
// 	return NEITHER;
//     }
//     if ((830 < freq && freq < 870)) { // || (320 < freq && 360 > freq)) {
// 	    return ENEMY;
//     } else if ((3900 < freq && freq < 4100)) {   //  || (1900 < freq && 2100 > freq)) {
// 	    return NEITHER;
//     } else {
// 	return NEITHER;
//     }
//  }
//     
//     
//     
// void setMultiplexChannel(int multiplex_pin_number) {
// 	
//     switch(multiplex_pin_number) {
// 	case 0:
// 	    digitalWrite(s0, LOW);
// 	    digitalWrite(s1, LOW);
// 	    digitalWrite(s2, LOW);
// 	    digitalWrite(s3, LOW);
// 	    break;
// 	case 1:
// 	    digitalWrite(s0, HIGH);
// 	    digitalWrite(s1, LOW);
// 	    digitalWrite(s2, LOW);
// 	    digitalWrite(s3, LOW);
// 	    break;
// 	case 2:
// 	    digitalWrite(s0, LOW);
// 	    digitalWrite(s1, HIGH);
// 	    digitalWrite(s2, LOW);
// 	    digitalWrite(s3, LOW);
// 	    break;
// 	case 3:
// 	    digitalWrite(s0, HIGH);
// 	    digitalWrite(s1, HIGH);
// 	    digitalWrite(s2, LOW);
// 	    digitalWrite(s3, LOW);
// 	    break;
// 	case 4:
// 	    digitalWrite(s0, LOW);
// 	    digitalWrite(s1, LOW);
// 	    digitalWrite(s2, HIGH);
// 	    digitalWrite(s3, LOW);
// 	    break;
// 	case 5:
// 	    digitalWrite(s0, HIGH);
// 	    digitalWrite(s1, LOW);
// 	    digitalWrite(s2, HIGH);
// 	    digitalWrite(s3, LOW);
// 	    break;
// 	case 6:
// 	    digitalWrite(s0, LOW);
// 	    digitalWrite(s1, HIGH);
// 	    digitalWrite(s2, HIGH);
// 	    digitalWrite(s3, LOW);
// 	    break;
// 	case 7:
// 	    digitalWrite(s0, HIGH);
// 	    digitalWrite(s1, HIGH);
// 	    digitalWrite(s2, HIGH);
// 	    digitalWrite(s3, LOW);
// 	    break;
// 	case 8:
// 	    digitalWrite(s0, LOW);
// 	    digitalWrite(s1, LOW);
// 	    digitalWrite(s2, LOW);
// 	    digitalWrite(s3, HIGH);
// 	    break;
// 	case 9:
// 	    digitalWrite(s0, HIGH);
// 	    digitalWrite(s1, LOW);
// 	    digitalWrite(s2, LOW);
// 	    digitalWrite(s3, HIGH);
// 	    break;
// 	case 10:
// 	    digitalWrite(s0, LOW);
// 	    digitalWrite(s1, HIGH);
// 	    digitalWrite(s2, LOW);
// 	    digitalWrite(s3, HIGH);
// 	    break;
// 	case 11:
// 	    digitalWrite(s0, HIGH);
// 	    digitalWrite(s1, HIGH);
// 	    digitalWrite(s2, LOW);
// 	    digitalWrite(s3, HIGH);
// 	    break;
// 	case 12:
// 	    digitalWrite(s0, LOW);
// 	    digitalWrite(s1, LOW);
// 	    digitalWrite(s2, HIGH);
// 	    digitalWrite(s3, HIGH);
// 	    break;
// 	case 13:
// 	    digitalWrite(s0, HIGH);
// 	    digitalWrite(s1, LOW);
// 	    digitalWrite(s2, HIGH);
// 	    digitalWrite(s3, HIGH);
// 	    break;
// 	case 14:
// 	    digitalWrite(s0, LOW);
// 	    digitalWrite(s1, HIGH);
// 	    digitalWrite(s2, HIGH);
// 	    digitalWrite(s3, HIGH);
// 	    break;
// 	case 15:
// 	    digitalWrite(s0, HIGH);
// 	    digitalWrite(s1, HIGH);
// 	    digitalWrite(s2, HIGH);
// 	    digitalWrite(s3, HIGH);
// 	    break;
// 	default:
// 	    digitalWrite(s0, LOW);
// 	    digitalWrite(s1, LOW);
// 	    digitalWrite(s2, LOW);
// 	    digitalWrite(s3, LOW);
// 	    break;
//     }
// }
// 
// //returns -1 if no sensors tripped.
// // int Sensors_AngleToBeacon(void) {
// //     bool sensorValue[NUM_BEACON_SENSORS];
// //     int beaconSensorCounter = 0; //to iterate through the detectors through the multiplexer
// //     
// //     for(int i = 0; i< NUM_BEACON_SENSORS; i++) {
// // 	beaconSensorCounter = i;
// // 	setMultiplexChannel(beaconSensorMultiplexPins[beaconSensorCounter]);
// // 	delay(50);
// // 	sensorValue[i] = BeaconSense_VisibleTargets();
// // 	printBeaconSensor(i);
// // 	Serial.print(sensorValue[i]);
// //     }
// //     Serial.println();
// //     
// //     float angle = 0.0;
// //     int numOnSensors = 0;
// //     int sensorValueOn = 0;
// //     float dDegrees = 360.0/NUM_BEACON_SENSORS;
// //     for (int i=0; i < NUM_BEACON_SENSORS; i++) {
// // 	if( sensorValue[i] != 0) {
// // 	    numOnSensors += 1;
// // 	    sensorValueOn = 1;
// // 	    angle += (i*dDegrees);
// // 	    if (i == (NUM_BEACON_SENSORS - 1) && sensorValue[0] != 0) {
// // 		angle += 360.0;
// // 	    }
// // 	} else {
// // 	    sensorValueOn = 0;
// // 	}
// //     }
// //     float avgAngle = angle/numOnSensors;
// //     Serial.println(avgAngle);
// //     return avgAngle;
// // }
// 
// //return -1 if no sensors on.
// int Sensors_AngleToBeacon(void) {
//     bool sensorValue[NUM_BEACON_SENSORS];
//     int beaconSensorCounter = 0; //to iterate through the detectors through the multiplexer
//     
//     for(int i = 0; i< NUM_BEACON_SENSORS; i++) {
// 	beaconSensorCounter = i;
// 	setMultiplexChannel(beaconSensorMultiplexPins[beaconSensorCounter]);
// 	delay(50);
// 	sensorValue[i] = BeaconSense_VisibleTargets();
// 	printBeaconSensor(i);
// 	Serial.print(sensorValue[i]);
//     }
//     Serial.println();
//     
//     float angle = 0.0;
//     int numOnSensors = 0;
//     int sensorValueOn = 0;
//     float dDegrees = 360.0/NUM_BEACON_SENSORS;
//     for (int i=0; i < NUM_BEACON_SENSORS; i++) {
// 	if( sensorValue[i] != 0) {
// 	    numOnSensors += 1;
// 	    sensorValueOn = 1;
// 	    angle += (i*dDegrees);
// 	    if (i == (NUM_BEACON_SENSORS - 1) && sensorValue[0] != 0) {
// 		angle += 360.0;
// 	    }
// 	} else {
// 	    sensorValueOn = 0;
// 	}
//     }
//     float avgAngle = angle/numOnSensors;
// 
//     if (numOnSensors == 0) {
// 	avgAngle = -1;
//     }
// 
//     Serial.println(avgAngle);
//     
// 
//     return avgAngle;
// }
// 
// // int Sensors_AngleToBeacon() {
// //     int numSensorsOn;
// //     
// //     for (int i = 0; i < NUM_BEACON_SENSORS; i++) {
// // 	setMultiplexChannel(beaconSensorMultiplexPins[i]);
// // 	int 
// //     }
// // }
// 
// //return -1 if never read
// int Sensors_AvgAngleToBeacon(void) {
//     int angleSum = 0;
//     int numAngles = 0;
//     
//     for (int i = 0; i < 10; i++) {
// 	int angle = Sensors_AngleToBeacon();
// 
// 	if (angle != -1) {
// 	    angleSum += angle;
// 	    numAngles++;
// 	}
// 
//     }
// 
//     float avgAngle = (float) angleSum / numAngles;
//     
// }
// 
// // Returns if the enemy is in front of one of the beacons
// // Returns 0 if no enemy in front. Returns a non-zero code if yes (ENEMY = 2)
// int isEnemyInFrontBeacon(){
//     
//     // Manually check if a beacon is seeing enemey
//     
//     int beaconNumber = 1; // The beacon number on the multiplexer
//     //JS: Beacon # 1 is the front right relative to breadboard but physcially on the robot #1 is the front
//     
//     setMultiplexChannel(beaconSensorMultiplexPins[beaconNumber]);
//     //delay(50);
//     
//     int sensorValue = BeaconSense_VisibleTargets();
//     Serial.println(sensorValue);
//     
//     return sensorValue;
// }