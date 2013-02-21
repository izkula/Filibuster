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

/*---------------- Main Code ---------------------------------*/


void setup() {
    Serial.begin(115200);
}

void loop() {
    RunStateMachine(KEYBOARD_DRIVE);

    
}
