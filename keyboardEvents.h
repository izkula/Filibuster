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
#include "definitions.h"
#include "Arduino.h"

/*---------------- Module Level Defines ---------------------------------*/

//37-40 are the ascii characters for the keyboard arrows.
#define LEFT_KEY 'a'
#define UP_KEY 'w'
#define RIGHT_KEY 'd'
#define DOWN_KEY 's'

/*---------------- Module Level Variables ---------------------------*/
static char time = 0;

/*---------------- Module Public Function Prototypes ---------------------------*/
//bool ParseKeyboardInput();

int GetInputTime();

bool TestTurnCommRecvd();
bool TestMoveCommRecvd();


/*---------------- Module Private Function Prototypes ---------------------------*/

void ClearSerialBuffer();
