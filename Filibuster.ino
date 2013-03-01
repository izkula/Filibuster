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



void setup() {
    Serial.begin(9600);
    TMRArd_InitTimer(MOVE_TIMER, 0);
    TMRArd_InitTimer(TURN_TIMER, 0);

    Drive_InitMotors();

    
    
}

void loop() {

    //Drive_Stop();
    //delay(300);
    
    //Drive_Move(BACK, 255);
    
    
    RunStateMachine(KEYBOARD_DRIVE);

    //digitalWrite(BL_MOTOR_DIR_PIN, BR_FWD);
    //analogWrite(BL_MOTOR_ENABLE_PIN, 255);
    
}
