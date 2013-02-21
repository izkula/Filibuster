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
#include "stateMachine.h"

/*---------------- Module Public Functions ---------------------------*/

/**Runs the state machine according to the selected strategy s */
void RunStateMachine(Strategy s) {

    switch(s) {
	case (KEYBOARD_DRIVE):
	    RunKeyboardDriveSM();
	    break;
	default:
	    RunKeyboardDriveSM();
	    break;
    }

}

/*---------------- Module Private Functions ---------------------------*/
void RunKeyboardDriveSM() {
    
    static State state = INITIALIZE;

    switch(state) {
	case INITIALIZE:
	    //Serial.println("in initialize");
	    InitializeMotors();
	    InitializeSensors();
	    state = STOPPED;
	    break;
	case MOVING:
	    //Serial.println("in moving");
	    //delay(1000);
	    state = STOPPED;

	    //advance the buffer to get rid of garbage if no commands were received.
	    Serial.read(); 
	    
	    break;
	case TURNING:
	    //Serial.println("in turning");
	    //delay(1000);
	    state = STOPPED;

	    //advance the buffer to get rid of garbage if no commands were received.
	    Serial.read(); 
	    
	    break;
	case STOPPED:
	    //delay(1000);
	    //Serial.println("in stopped");
	    if (TestTurnCommRecvd()) {
		int time = GetInputTime();
		Serial.print("Turning for "); Serial.println((int) time);
		state = TURNING;
		
	    }

	    else if (TestMoveCommRecvd()) {
		int time = GetInputTime();
		Serial.print("Moving for "); Serial.println((int) time);
		state = MOVING;
		
		
	    }

	    //advance the buffer to get rid of garbage if no commands were received.
	    Serial.read(); 
	    
	    break;
	default:
	    Serial.println("ERROR: Invalid state received");
	    break;
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