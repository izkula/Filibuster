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

/*---------------- Module Level Defines ---------------------------------*/

//37-40 are the ascii characters for the keyboard arrows.
#define LEFT_KEY 'a'
#define UP_KEY 'w'
#define RIGHT_KEY 'd'
#define DOWN_KEY 's'

/*---------------- Module Level Variables ---------------------------*/

/*---------------- Module Public Function Prototypes ---------------------------*/

/**returns whether the specified key has arrived on the serial line*/
boolean TestForwardKeyPressed() {

    if (Serial.available()) {
	char c = Serial.read()

	if (c == FORWARD_KEY) return true;
	else return false;
    }

    return false;
    
}

/**returns whether the specified key has arrived on the serial line*/
boolean TestLeftKeyPressed() {
    
    if (Serial.available()) {
	char c = Serial.read()
	
	if (c == LEFT_KEY) return true;
	else return false;
    }
    
    return false;
    
}

/**returns whether the specified key has arrived on the serial line*/
boolean TestRightKeyPressed() {
    
    if (Serial.available()) {
	char c = Serial.read()
	
	if (c == RIGHT_KEY) return true;
	else return false;
    }
    
    return false;
    
}

/**returns whether the specified key has arrived on the serial line*/
boolean TestDownKeyPressed() {
    
    if (Serial.available()) {
	char c = Serial.read()
	
	if (c == DOWN_KEY) return true;
	else return false;
    }
    
    return false;
    
}

/*---------------- Module Private Function Prototypes ---------------------------*/
