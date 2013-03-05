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
	case (SIMPLE_BRICK_BEATER):
	   RunSimpleBrickBeaterSM();
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

static void RunSimpleBrickBeaterSM() {
    static State state = INITIALIZE;
    static int angleToTarget = 0;

    switch(state) {
	case INITIALIZE:
	    //TODO correct this
	    InitializeMotors();
	    InitializeSensors();
	    state = SEARCHING_FOR_OPP;
	    break;

	case SEARCHING_FOR_OPP:
	    angleToTarget = Sensors_AngleToBeacon();

	    if (TestOppLocated()) {
		Drive_Move(FORWARD, 100);
		state = CHARGING;
	    }

	    state = CHARGING;
	    break;

	case CHARGING:
	    angleToTarget = Sensors_AngleToBeacon();

	    if (TestOppLost()) {
		Drive_Turn(RIGHT, 100);
		state = SEARCHING_FOR_OPP;
	    }

	    break;

	default:
	    Serial.println("Error! Received invalid state!");
	    break;

    }


}

/*static void RunBrickBeaterSM() {
    static State state = INITIALIZE;

    switch(state) {
	case INITIALIZE:
	    //TODO correct this
	    //Serial.println("in initialize");
	    InitializeMotors();
	    InitializeSensors();
	    state = STOPPED;
	    break;
	case SEARCHING_FOR_OPP:
	    if (TestOppLocated()) {
		TurnTowardsOpp();
	    }

	    state = ALIGNING_OPP;
	    break;
	case ALIGNING_OPP:
	    if (TestOppAligned()) {
		StartChargeTimer();

	    }
	    break;
	case CHARGING_OPP;
	    break;
    }


}*/

/*static void RunBrickBeaterSM() {
    static State state = INITIALIZE;

    switch(state) {
	case INITIALIZE:
	    //TODO correct this
	    //Serial.println("in initialize");
	    InitializeMotors();
	    InitializeSensors();
	    state = STOPPED;
	    break;
	case SEARCHING_FOR_OPP:
	    if (TestOppLocated()) {
		TurnTowardsOpp();
	    }

	    state = ALIGNING_OPP;
	    break;
	case ALIGNING_OPP:
	    if (TestOppAligned()) {
		StartChargeTimer();
		
	    }
	    break;
	case CHARGING_OPP;
	    break;
    }

    
}*/


//-------------- Keyboard state machine functions -------------------

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

//-------------------Simple brick beater state machine functions -------------

bool TestOppLocated() {
    if (Sensors_AngleToBeacon == 0) {
	return true;
    }

    else return false;
}

bool TestOppLost() {
    if (Sensors_AngleToBeacon != 0) {
	return true;
    }

    else return false;
}


/*---------------- Test Harness ---------------------------*/


// #ifdef TESTING
// 
// void main(void) {
//     //write test harness here
// }
// 
// #endif /* TESTING */