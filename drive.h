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
#include "definitions.h"
#include <Arduino.h>

/*---------------- Module Level Defines ---------------------------------*/

//motor pin mappings
#define BR_MOTOR_DIR_PIN 2
#define BR_MOTOR_ENABLE_PIN 3
#define BL_MOTOR_DIR_PIN 4
#define BL_MOTOR_ENABLE_PIN 5
#define FR_MOTOR_DIR_PIN 7
#define FR_MOTOR_ENABLE_PIN 6
#define FL_MOTOR_DIR_PIN 8
#define FL_MOTOR_ENABLE_PIN 9

/*---------------- Module Level Variables ---------------------------*/

/*---------------- Module Public Function Prototypes ---------------------------*/
/** Initializes the pins that the motors are being controlled by*/
void Drive_InitMotors();

/**Commands the motors to turn the robot left or right at the specified speed between 0 and 255.*/
void Drive_Turn(Direction dir, int speed);
/**Commands the motors to move the robot forward or backwards at the specified speed.*/
void Drive_Move(Direction dir, int speed);

/**Commands the motors to stop*/
void Drive_Stop();

/*---------------- Module Private Function Prototypes ---------------------------*/

