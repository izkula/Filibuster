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

/** Initializes the pins that the motors are being controlled by*/
void Drive_InitMotors() {
    pinMode(RIGHT_MOTORS_DIR_PIN, OUTPUT);
    pinMode(LEFT_MOTORS_DIR_PIN, OUTPUT);
    pinMode(RIGHT_MOTORS_ENABLE_PIN, OUTPUT);
    pinMode(LEFT_MOTORS_ENABLE_PIN, OUTPUT);
   
}

/**Commands the motors to turn the robot left or right at the specified speed between 0 and 255.*/
void Drive_Turn(Direction dir, int speed) {
    
    //turns the robot left at the specified speed.
    if (dir == LEFT) {
	digitalWrite(RIGHT_MOTORS_DIR_PIN, HIGH);
	digitalWrite(LEFT_MOTORS_DIR_PIN, LOW);
	
	analogWrite(RIGHT_MOTORS_ENABLE_PIN, speed);
	analogWrite(LEFT_MOTORS_ENABLE_PIN, speed);
    }

    //turns the robot right at the specified speed.
    else if (dir == RIGHT) {
	digitalWrite(RIGHT_MOTORS_DIR_PIN, LOW);
	digitalWrite(LEFT_MOTORS_DIR_PIN, HIGH);
	
	analogWrite(RIGHT_MOTORS_ENABLE_PIN, speed);
	analogWrite(LEFT_MOTORS_ENABLE_PIN, speed);
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
	digitalWrite(RIGHT_MOTORS_DIR_PIN, HIGH);
	digitalWrite(LEFT_MOTORS_DIR_PIN, HIGH);
	
	analogWrite(RIGHT_MOTORS_ENABLE_PIN, speed);
	analogWrite(LEFT_MOTORS_ENABLE_PIN, speed);
    }

    //moves the robot backwards at the specified speed.
    else if (dir == BACK) {
	//turns of the other channel of the H-bridge to prevent shoot-through.
	digitalWrite(LEFT_MOTORS_DIR_PIN, LOW);
	digitalWrite(RIGHT_MOTORS_DIR_PIN, LOW);
	
	//turns the robot right.
	analogWrite(LEFT_MOTORS_ENABLE_PIN, speed);
	analogWrite(RIGHT_MOTORS_ENABLE_PIN, speed);
    }

    //give error if passed bad parameter
    else {
	Serial.println("Error in Drive_Move! Passed bad direction parameter.");
    }
}

void Drive_Stop() {
    digitalWrite(LEFT_MOTORS_ENABLE_PIN, LOW);
    digitalWrite(RIGHT_MOTORS_ENABLE_PIN, LOW);

}
/*---------------- Module Private Functions ---------------------------*/

/*---------------- Test Harness ---------------------------*/

#ifdef TESTING

void main(void) {
    //write test harness here
}

#endif /* TESTING */