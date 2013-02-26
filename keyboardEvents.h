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
static char comm = 0; //the command passed in by the keyboard
static int duration = 0; //the amount of time to execute that command for, if relevant
static bool ready = false; //a boolean to make sure the same command isn't read twice. Is set true upon reading a command (via parseInput), set false after testing for the command.

/*---------------- Module Public Function Prototypes ---------------------------*/

//Gets the last command input via the keyboard
char KeyboardEvents_GetComm();
//Gets the last duration input via the keyboard
int KeyboardEvents_GetDuration();

//checks to see if a turn command has been received and does the necessary book-keeping to make sure the same one is not read twice.
bool KeyboardEvents_TestTurnCommRecvd();
//checks to see if a move command has been received and does the necessary book-keeping to make sure the same one is not read twice.
bool KeyboardEvents_TestMoveCommRecvd();

/*---------------- Module Private Function Prototypes ---------------------------*/
static void ClearSerialBuffer();
static bool ParseInput();

