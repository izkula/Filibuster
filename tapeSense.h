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
 * when		who  		what/why
 * 
 **************************************************************/

/*---------------- Includes ---------------------------------*/

/*---------------- Module Level Defines ---------------------------------*/
#define FR_pin 1
#define FL_pin 2
#define BR_pin 3
#define BL_pin 4

/*---------------- Module Level Variables ---------------------------*/

/*---------------- Module Public Function Prototypes ---------------------------*/

//Initializes the tape sensors.
void TapeSense_Initialize();

//returns the a character encoding which tape sensors currently see tape as the 4 least significant bits.
//Bits are MSB <--- FR,FL,BR,BL --> LSB
char TapeSense_ReadTapeSensors();

//Prints which tape sensors can currently can see tape.
void TapeSense_PrintActiveSensors();

/*---------------- Module Private Function Prototypes ---------------------------*/

//returns true if the sensor sees tape, false otherwise
static bool TapeDetectedFR();
static bool TapeDetectedFL();
static bool TapeDetectedBL();
static bool TapeDetectedBR();

