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

//returns the the last command given
char KeyboardEvents_GetComm() {

    return comm;
}

//returns the duration of the last command
int KeyboardEvents_GetDuration() {

    return duration;
}

bool KeyboardEvents_TestTurnCommRecvd() {

    ParseInput();

    if (comm == 'R' || comm == 'L') {
	if (ready == true) {
	    ready = false;
	    return true;
	}
    }

    return false;
    
}

bool KeyboardEvents_TestMoveCommRecvd() {

    ParseInput();

    if (comm == 'F' || comm == 'B') {
	if (ready == true) {
	    ready = false;
	    return true;
	}
    }

    return false;

}

/**Parses the serial input buffer one character at a time. Returns false if there was nothing to parse.*/
bool ParseInput() {

    if (Serial.available()) {

	//Serial.println("available!");
	byte b = Serial.peek();

	//reads in the command
	switch(b) {
	    case 'R':
	    case 'L':
	    case 'F':
	    case 'B':
		comm = Serial.read();
		Serial.print("command '"); Serial.print((char) comm); Serial.println("' received!");
		break;
	    case '#':
		ready = true;
		Serial.read();
		break;
	    default:
		duration = Serial.read() - 48; //48 to account for ascii numbering
		Serial.print("duration '"); Serial.print(duration); Serial.println("' received!");
		break;

	}

	return true;
    }

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