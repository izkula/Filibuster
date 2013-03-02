#pragma once

//#define TESTING //comment this out to use test harness

/**************************************************************
 * Filename: drive.cpp
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
#include "drive.h"

/*---------------- Module Public Functions ---------------------------*/

/** Initializes the pins that the motors are being controlled by as outputs. We should NOT initialize PWM pins as outputs in arduino.*/
void Drive_InitMotors() {
    pinMode(BL_MOTOR_DIR_PIN, OUTPUT);
    pinMode(BR_MOTOR_DIR_PIN, OUTPUT);
    pinMode(FL_MOTOR_DIR_PIN, OUTPUT);
    pinMode(FR_MOTOR_DIR_PIN, OUTPUT);
   
}

/**Commands the motors to turn the robot left or right at the specified speed between 0 and 255.*/
void Drive_Turn(Direction dir, int speed) {
    
    //turns the robot left at the specified speed.
    if (dir == LEFT) {
	digitalWrite(BL_MOTOR_DIR_PIN, BL_BWD);
	digitalWrite(FL_MOTOR_DIR_PIN, FL_BWD);
	digitalWrite(BR_MOTOR_DIR_PIN, BR_FWD);
	digitalWrite(FR_MOTOR_DIR_PIN, FR_FWD);


// 	digitalWrite(BL_MOTOR_DIR_PIN, LOW);
// 	digitalWrite(FL_MOTOR_DIR_PIN, LOW);
// 	digitalWrite(BR_MOTOR_DIR_PIN, HIGH);
// 	digitalWrite(FR_MOTOR_DIR_PIN, HIGH);
	
	analogWrite(BL_MOTOR_ENABLE_PIN, speed);
	analogWrite(FL_MOTOR_ENABLE_PIN, speed);
	analogWrite(BR_MOTOR_ENABLE_PIN, speed);
	analogWrite(FR_MOTOR_ENABLE_PIN, speed);
    }

    //turns the robot right at the specified speed.
    else if (dir == RIGHT) {
	digitalWrite(BL_MOTOR_DIR_PIN, BL_FWD);
	digitalWrite(FL_MOTOR_DIR_PIN, FL_FWD);
	digitalWrite(BR_MOTOR_DIR_PIN, BR_BWD);
	digitalWrite(FR_MOTOR_DIR_PIN, FR_BWD);

// 	digitalWrite(BL_MOTOR_DIR_PIN, HIGH);
// 	digitalWrite(FL_MOTOR_DIR_PIN, HIGH);
// 	digitalWrite(BR_MOTOR_DIR_PIN, LOW);
// 	digitalWrite(FR_MOTOR_DIR_PIN, LOW);
	
	analogWrite(BL_MOTOR_ENABLE_PIN, speed);
	analogWrite(FL_MOTOR_ENABLE_PIN, speed);
	analogWrite(BR_MOTOR_ENABLE_PIN, speed);
	analogWrite(FR_MOTOR_ENABLE_PIN, speed);
    }


    //give error if passed bad parameter
    else {
	Serial.println("Error in Drive_Turn! Passed bad direction parameter.");
    }
}

/**Commands the motors to move the robot forward or backwards at the specified speed.*/
void Drive_Move(Direction dir, int speed){
    
    //moves the robot forward at the specified speed
    if (dir == FORWARD) {
	digitalWrite(BL_MOTOR_DIR_PIN, BL_FWD);
	digitalWrite(FL_MOTOR_DIR_PIN, FL_FWD);
	digitalWrite(BR_MOTOR_DIR_PIN, BR_FWD);
	digitalWrite(FR_MOTOR_DIR_PIN, FR_FWD);

// 	digitalWrite(BL_MOTOR_DIR_PIN, HIGH);
// 	digitalWrite(FL_MOTOR_DIR_PIN, HIGH);
// 	digitalWrite(BR_MOTOR_DIR_PIN, HIGH);
// 	digitalWrite(FR_MOTOR_DIR_PIN, HIGH);
	
	analogWrite(BL_MOTOR_ENABLE_PIN, speed);
	analogWrite(FL_MOTOR_ENABLE_PIN, speed);
	analogWrite(BR_MOTOR_ENABLE_PIN, speed);
	analogWrite(FR_MOTOR_ENABLE_PIN, speed);
    }

    //moves the robot backwards at the specified speed.
    else if (dir == BACK) {
	digitalWrite(BL_MOTOR_DIR_PIN, BL_BWD);
	digitalWrite(FL_MOTOR_DIR_PIN, FL_BWD);
	digitalWrite(BR_MOTOR_DIR_PIN, BR_BWD);
	digitalWrite(FR_MOTOR_DIR_PIN, FR_BWD);

// 	digitalWrite(BL_MOTOR_DIR_PIN, LOW);
// 	digitalWrite(FL_MOTOR_DIR_PIN, LOW);
// 	digitalWrite(BR_MOTOR_DIR_PIN, LOW);
// 	digitalWrite(FR_MOTOR_DIR_PIN, LOW);
	
	analogWrite(BL_MOTOR_ENABLE_PIN, speed);
	analogWrite(FL_MOTOR_ENABLE_PIN, speed);
	analogWrite(BR_MOTOR_ENABLE_PIN, speed);
	analogWrite(FR_MOTOR_ENABLE_PIN, speed);
    }

    //give error if passed bad parameter
    else {
	Serial.println("Error in Drive_Move! Passed bad direction parameter.");
    }
}

void Drive_Stop() {
    digitalWrite(BL_MOTOR_ENABLE_PIN, LOW);
    digitalWrite(FL_MOTOR_ENABLE_PIN, LOW);
    digitalWrite(BR_MOTOR_ENABLE_PIN, LOW);
    digitalWrite(FR_MOTOR_ENABLE_PIN, LOW);

}

void Drive_TurnTowardsTarget(int angle, int speed) {
    if (0 < angle && angle <= 180) {
	Drive_Turn(RIGHT, speed);
    }

    else if (180 < angle && angle < 360) {
	Drive_Turn(LEFT, speed);
    }

    else {
	Serial.println("Invalid angle passed to Drive_TurnTowardsTarget()!");
    }
}

/*---------------- Module Private Functions ---------------------------*/

/*---------------- Test Harness ---------------------------*/

#ifdef TESTING

void main(void) {
    //write test harness here
}

#endif /* TESTING */