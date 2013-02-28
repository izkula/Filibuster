#pragma once

//#define TESTING //comment this out to use test harness

/**************************************************************
 * Filename: stateMachine.h
 * Contents: h file for state machine module for ME210 SumoSenator
 * 
 * Notes:    Target: Arduino UNO R1 & R2
 * Arduino IDE version: 0022
 * 
 * History:
 * when		who		what/why
 * Feb 19    	Evan 		created
 * 
 **************************************************************/

/*---------------- Includes ---------------------------------*/
#include "sensors.h"
#include "drive.h"
#include "navigation.h"
#include "keyboardEvents.h"
#include "definitions.h"
#include "initialize.h"
#include <Arduino.h>
#include <Timers.h>

/*---------------- Module Level Defines ---------------------------------*/

/*---------------- Module Level Variables ---------------------------*/

/*---------------- Module Public Function Prototypes ---------------------------*/
void RunStateMachine(Strategy s);

/*---------------- Module Private Function Prototypes ---------------------------*/
static void RunKeyboardDriveSM();
static bool TestMoveCommRecvd();
static bool TestTurnCommRecvd();
static bool TestTurnTimerExp();
static bool TestMoveTimerExp();

