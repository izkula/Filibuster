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
 * when      who  what/why
 *
 **************************************************************/

/*---------------- Includes ---------------------------------*/
#include "stateMachine.h"
#include "definitions.h"
#include <Timers.h>
#include "drive.h"
#include "sensors.h"


/*---------------- Main Code ---------------------------------*/



void setup() {
    Serial.begin(9600);
    TMRArd_InitTimer(MOVE_TIMER, 0);
    TMRArd_InitTimer(TURN_TIMER, 0);

    Drive_InitMotors();

    Sensors_Initialize();

    Drive_Stop();
    
    //Drive_Turn(LEFT, 255);
    
}

void loop() {

    
    //Drive_Stop();
    //delay(300);
    
    //Drive_Move(BACK, 255);
    
    while(1){
	//Serial.println("SPPPIIIN!");
	
	int sumReadings = 0 ;
	for ( int i = 0 ; i < 15 ; i++){ //25
          //delay(10);
	  sumReadings+= isEnemyInFrontBeacon();
	}
	
	if(sumReadings >= 10*2){ //20
          Serial.println("CHarge !!!!");
	  
	  Drive_Move(FORWARD, 255); // JS Drive move should be the same as  drive turn
	  //delay(10000);
	  break;
	} else {
	    Drive_Turn(LEFT, 150);
	}
    }
    
    //    isEnemyInFrontBeacon();
    //    delay(50);
    
    //RunStateMachine(KEYBOARD_DRIVE);
    
    //digitalWrite(BL_MOTOR_DIR_PIN, BR_FWD);
    //analogWrite(BL_MOTOR_ENABLE_PIN, 255);
    
    
    //Drive_Stop();
    //delay(300);
    
    //Drive_Move(BACK, 255);

    //delay(300);
    
    //int angle = Sensors_AngleToBeacon();
    //Serial.println(angle);
    
    //RunStateMachine(KEYBOARD_DRIVE);

    //digitalWrite(BL_MOTOR_DIR_PIN, BR_FWD);
    //analogWrite(BL_MOTOR_ENABLE_PIN, 255);
    
}
