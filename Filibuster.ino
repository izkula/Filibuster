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

/*---------------- Main Code ---------------------------------*/


void setup() {
    Serial.begin(9600);
}

void loop() {
    if (Serial.available()) {
	char c = Serial.read();
	Serial.println(c);
    }
}