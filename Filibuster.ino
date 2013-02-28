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
#include <AFMotor.h>


/*---------------- Main Code ---------------------------------*/

static AF_DCMotor motor1(1, MOTOR12_64KHZ); // create motor #2, 64KHz pwm
//static AF_DCMotor motor2(2, MOTOR12_64KHZ); // create motor #2, 64KHz pwm


void setup() {
    Serial.begin(9600);
    TMRArd_InitTimer(MOVE_TIMER, 0);
    TMRArd_InitTimer(TURN_TIMER, 0);

    Drive_InitMotors();
    //pinMode(RIGHT_MOTORS_DIR_PIN, OUTPUT);

    motor1.setSpeed(255);
    //motor2.setSpeed(255);
}

void loop() {
    
    RunStateMachine(KEYBOARD_DRIVE);
   
    
}
