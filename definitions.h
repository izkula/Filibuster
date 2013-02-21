#pragma once

typedef enum {
    INITIALIZE, SEARCHING_FOR_OPP, TURNING_TO_FACE, CHARGING, GRAPPLING, TURNING, MOVING, STOPPED
} State;

typedef enum {
    KEYBOARD_DRIVE, KEYBOARD_EVENTS, WALL_HUGGER
} Strategy;

typedef enum {
    RIGHT, LEFT, FORWARD, BACK, NONE
} Direction;

typedef struct {
    char c;
    int time;
    Direction dir;
} KeyboardEvent;