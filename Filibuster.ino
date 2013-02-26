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

/*---------------- Main Code ---------------------------------*/


void setup() {
    Serial.begin(9600);
    TMRArd_InitTimer(MOVE_TIMER, 0);
    TMRArd_InitTimer(TURN_TIMER, 0);

    Drive_InitMotors();
}

void loop() {
    //RunStateMachine(KEYBOARD_DRIVE);
    Drive_Move(FORWARD, 255);
    Serial.println("Moving forward");
    delay(3000);
    Drive_Move(BACK, 255);
    Serial.println("Moving back");
    
    delay(3000);
    Drive_Turn(LEFT, 255);
    Serial.println("Turning left");
    
    delay(3000);
    Drive_Turn(RIGHT, 255);
    Serial.println("Turning right");
    
    delay(3000);
    
}
