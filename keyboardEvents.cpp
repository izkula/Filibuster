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
#include "keyboardEvents.h"
//#include /*include the module's .h file*/

/*---------------- Module Public Functions ---------------------------*/

/**Parses the serial input buffer one character at a time. Returns false if there was nothing to parse. Input times should be 0-9*/
/* bool ParseKeyboardInput() {
    while(Serial.available()) {

	char c = Serial.read();

	switch(c) {
	    case 'R':
	    case 'L':
	    case 'F':
	    case 'B':
		comm = c;
		break;
	    default: //anything besides R, L, F, B treated as a time number
		time = c;
		break;
	}

	return true;
    }

    return false; //nothing on serial buffer.
} */

int GetInputTime() {
    return time;
}

bool TestTurnCommRecvd() {

    if (Serial.available()) {
	if (Serial.peek() == 'R' || Serial.peek() == 'L') {
	    //discard the command character
	    Serial.read();
	    //read the command duration
	    time = Serial.read();
	    Serial.println(time);
	    ClearSerialBuffer();
	    
	    return true;
	}
	
	return false;
    }

    time = 0;
    return false;
    
}

bool TestMoveCommRecvd() {
    
    if (Serial.available()) {
	if (Serial.peek() == 'F' || Serial.peek() == 'B') {
	    //discard the command character
	    Serial.read();
	    //read the command duration
	    time = Serial.read();
	    Serial.println(time);
	    
	    ClearSerialBuffer();
	    
	    return true;
	}

	return false;
    }

    time = 0;
    return false;

}

/*---------------- Module Private Functions ---------------------------*/

/**Removes any characters remaining in the serial buffer. NOTE: Serial.flush doesn't do what you think it would!!! This does that. */
void ClearSerialBuffer() {
    while(Serial.available()) {
	Serial.read();
    }
}


/*---------------- Test Harness ---------------------------*/

// #ifdef TESTING
// 
// void main(void) {
//     //write test harness here
// }
// 
// #endif /* TESTING */