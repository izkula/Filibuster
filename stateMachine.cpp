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

	    if (TestMoveTimerExp()) {
		Drive_Stop();
		state = STOPPED;
	    }

	    else if (TestTurnCommRecvd()) {
		int duration = KeyboardEvents_GetDuration();

		TMRArd_StopTimer(MOVE_TIMER);
		
		TMRArd_SetTimer(TURN_TIMER, duration*SEC);
		TMRArd_StartTimer(TURN_TIMER);
		
		if (KeyboardEvents_GetComm() == 'R') {
		    Drive_Turn(RIGHT, 255);
		    Serial.print("Turning '"); Serial.print('R'); Serial.print("for "); Serial.println(duration);
		    
		}
		
		
		if (KeyboardEvents_GetComm() == 'L') {
		    Serial.print("Turning '"); Serial.print('L'); Serial.print("for "); Serial.println(duration);
		    
		    Drive_Turn(LEFT, 255);
		}

		state = TURNING;
	    }
	    
	    

	    //advance the buffer to get rid of garbage if no commands were received.
	    Serial.read(); 
	    
	    break;
	    
	case TURNING:
	  // Serial.println("in turning");
	    //delay(1000);
	    if (TestTurnTimerExp()) {
		Drive_Stop();
		state = STOPPED;
	    }

	    else if (TestMoveCommRecvd()) {
		int duration = KeyboardEvents_GetDuration();
		Serial.print("Moving for "); Serial.println(duration);

		TMRArd_StopTimer(TURN_TIMER);
		
		TMRArd_SetTimer(MOVE_TIMER, duration*SEC);
		TMRArd_StartTimer(MOVE_TIMER);
		
		if (KeyboardEvents_GetComm() == 'F') {
		    Serial.print("Moving '"); Serial.print('F'); Serial.print("for "); Serial.println(duration);
		    
		    Drive_Move(FORWARD, 255);
		}
		
		
		if (KeyboardEvents_GetComm() == 'B') {
		    Serial.print("Moving '"); Serial.print('B'); Serial.print("for "); Serial.println(duration);
		    
		    Drive_Move(BACK, 255);
		}
		
		state = MOVING;
		
	    }
	    
	    //advance the buffer to get rid of garbage if no commands were received.
	    //Serial.read(); 
	    
	    break;
	    
	case STOPPED:
	    //delay(1000);
	    //Serial.println("in stopped");
	    if (TestTurnCommRecvd()) {
		int duration = KeyboardEvents_GetDuration();
		Serial.print("Turning for "); Serial.println(duration);

		TMRArd_SetTimer(TURN_TIMER, duration*SEC);
		TMRArd_StartTimer(TURN_TIMER);

		if (KeyboardEvents_GetComm() == 'R') {
		    Drive_Turn(RIGHT, 255);
		}

		
		if (KeyboardEvents_GetComm() == 'L') {
		    Drive_Turn(LEFT, 255);
		}

		state = TURNING;
		
	    }

	    else if (TestMoveCommRecvd()) {
		int duration = KeyboardEvents_GetDuration();
		Serial.print("Moving for "); Serial.println(duration);

		TMRArd_SetTimer(MOVE_TIMER, duration*SEC);
		TMRArd_StartTimer(MOVE_TIMER);
		
		if (KeyboardEvents_GetComm() == 'F') {
		    Drive_Move(FORWARD, 255);
		}
		
		
		if (KeyboardEvents_GetComm() == 'B') {
		    Drive_Move(BACK, 255);
		}
				
		state = MOVING;
		
		
	    }

	    //advance the buffer to get rid of garbage if no commands were received.
	    //Serial.read(); 
	    
	    break;
	    
	default:
	    Serial.println("ERROR: Invalid state received");
	    break;
    }
}

bool TestMoveCommRecvd() {
    return KeyboardEvents_TestMoveCommRecvd();
}

bool TestTurnCommRecvd() {
    return KeyboardEvents_TestTurnCommRecvd();
}

bool TestTurnTimerExp() {
    bool expired = TMRArd_IsTimerExpired(TURN_TIMER);

    return expired;
}

static bool TestMoveTimerExp() {
    bool expired = TMRArd_IsTimerExpired(MOVE_TIMER);

    return expired;
}

/*---------------- Test Harness ---------------------------*/


// #ifdef TESTING
// 
// void main(void) {
//     //write test harness here
// }
// 
// #endif /* TESTING */