#pragma once

#define MOVE_TIMER 0
#define TURN_TIMER 1
#define SEC 1000

typedef enum {
    INITIALIZE, SEARCHING_FOR_OPP, TURNING_TO_FACE, CHARGING, GRAPPLING, TURNING, MOVING, STOPPED
} State;

typedef enum {
    KEYBOARD_DRIVE, SIMPLE_BRICK_BEATER, WALL_HUGGER
} Strategy;

typedef enum {
    RIGHT, LEFT, FORWARD, BACK, NONE
} Direction;

typedef struct {
    char c;
    int time;
    Direction dir;
} KeyboardEvent;