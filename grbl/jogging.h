#ifndef JOGGING_H_
#define JOGGING_H_
#include "grbl.h"
#include <stdio.h>

#define JOG_GCODE_BASE "G21 G91 G0"
#define JOG_GCODE_END "G90 G21"

#define RESET_AXIS_BASE "G10 P0 L20"

typedef enum {
    X,
    Y,
    Z 
} AXIS;

typedef enum {
    INIT,
    JOG_XY,
    JOG_Z,
    ZERO_X,
    ZERO_Y,
    ZERO_Z,
    WAIT
} JOG_STATE;

struct jog_state_input{
    int x;
    int y;
    int button_a;
    int button_b;
};

void jogging_state_machine(struct jog_state_input* data);
#endif
