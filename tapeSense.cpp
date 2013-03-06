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
#include "tapeSense.h"
#include <Arduino.h>

/*---------------- Module Public Functions ---------------------------*/

void TapeSense_Initialize() {
    
}

//returns the a character encoding which tape sensors currently see tape as the 4 least significant bits.
//Bits are MSB <--- FR,FL,BR,BL ---> LSB
char TapeSense_ReadTapeSensors() {
    char activeSensors = 0;

    if (TapeDetectedBL()) {
	activeSensors &= 0x01;
    }

    if (TapeDetectedBR()) {
	activeSensors &= (0x01 << 1);
    }

    if (TapeDetectedFL()) {
	activeSensors &= (0x01 << 2);
    }

    if (TapeDetectedFR()) {
	activeSensors &= (0x01 << 3);
    }

    return activeSensors;
}

void TapeSense_PrintActiveSensors() {
    char activeSensors = TapeSense_ReadTapeSensors();

    Serial.println("Tape Sensor Values:");
    Serial.print("BL: "); Serial.println(activeSensors & 0x01)
    Serial.print("BR: "); Serial.println(activeSensors & (0x01 << 1) )
    Serial.print("FL: "); Serial.println(activeSensors & (0x01 << 2) )
    Serial.print("FR: "); Serial.println(activeSensors & (0x01 << 3) )
    
}

/*---------------- Module Private Functions ---------------------------*/
static bool TapeDetectedFR() {
    return digitalRead(FR_pin);
}

static bool TapeDetectedFL() {
    return digitalRead(FL_pin);
}

static bool TapeDetectedBR() {
    return digitalRead(BR_pin);
}

static bool TapeDetectedBL() {
    return digitalRead(BL_pin);
}


/*---------------- Test Harness ---------------------------*/

#ifdef TESTING

void main(void) {
    //write test harness here
}

#endif /* TESTING */