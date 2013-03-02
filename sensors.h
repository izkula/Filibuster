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

/*---------------- Module Level Variables ---------------------------*/

/*---------------- Module Public Function Prototypes ---------------------------*/
int Sensors_AngleToBeacon(void);
void Sensors_Initialize();
int isEnemyInFrontBeacon();

/*---------------- Module Private Function Prototypes ---------------------------*/

