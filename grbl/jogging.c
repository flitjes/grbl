#include "jogging.h"

void jog(AXIS a, float increment);
void reset_axis(AXIS a);
char debug_str[50];
#define delay_debounce 0x3FF
void jogging_state_machine(struct jog_state_input* data){
    static JOG_STATE state = INIT;
    static JOG_STATE next_state = INIT;
    static unsigned int delay = 0;
    float x = 0;
    float y = 0;
    float z = 0;

/*    sprintf(debug_str, "joy: %d , %d \t", data->x, data->y);
    printString(debug_str);
    sprintf(debug_str, "but: %d, %d\n", data->button_a, data->button_b);
    printString(debug_str);*/

    switch(state){
        case INIT:
            if(!data->button_a && !data->button_b){
                next_state = JOG_XY;
                printString("Jogging XY\n");
                delay = delay_debounce;
                state = WAIT;
            }
        break;
        case JOG_XY:
            if(data->x == 0){
                x = -1;
                jog(X, x);
            } else if(data->x == 255){
                x = 1;
                jog(X, x);
            }
            
            if(data->y == 0){
                y = -1;
                jog(Y, y);
            } else if(data->y == 255){
                y = 1;
                jog(Y, y);
            }

            if(!data->button_a){
                next_state = JOG_Z;
                printString("Jogging Z\n");
                delay = delay_debounce;
                state = WAIT;
            } else {
                next_state = JOG_XY;
                delay = delay_debounce;
                state = WAIT;
            }
        break;
        case JOG_Z:
            if(data->y == 0){
                jog(Z, -1);
            } else if(data->y == 255){
                jog(Z, 1);
            }

            if(!data->button_b){
                next_state = JOG_XY;
                printString("Jogging XY\n");
                delay = delay_debounce;
                state = WAIT;
            } else if(!data->button_a){
                next_state = ZERO_X;
                printString("Reset zero X?\n");
                delay = delay_debounce;
                state = WAIT;
            } else {
                next_state = JOG_Z;
                delay = delay_debounce;
                state = WAIT;
            }
    
        break;
        case ZERO_X:
            if(!data->button_a){
                reset_axis(X);
            }
            if(!data->button_a || !data->button_b){
                next_state = ZERO_Y;
                printString("Reset zero Y?\n");
                delay = delay_debounce;
                state = WAIT;
            }
        break;
        case ZERO_Y:
            if(!data->button_a){
                reset_axis(Y);
            }
            if(!data->button_a || !data->button_b){
                next_state = ZERO_Z;
                printString("Reset zero Z?\n");
                delay = delay_debounce;
                state = WAIT;
            }
        break;
        case ZERO_Z:
            if(!data->button_a){
                reset_axis(Z);
            }
            if(!data->button_a || !data->button_b){
                next_state = INIT;
                printString("Jogging back to init\n");
                delay = delay_debounce;
                state = WAIT;
            }
        case WAIT:
            if (delay == 0)
                state = next_state;
            else
                delay--;
        break;
    }

    
}

/*Because arduino's stdio doens't support sprintf of floats..*/
char *ftoa(char *a, double f, int precision)
{
 long p[] = {0,10,100,1000,10000,100000,1000000,10000000,100000000};
 
 char *ret = a;
 long heiltal = (long)f;
 itoa(heiltal, a, 10);
 while (*a != '\0') a++;
 *a++ = '.';
 long desimal = abs((long)((f - heiltal) * p[precision]));
 itoa(desimal, a, 10);
 return ret;
}

void jog(AXIS a, float increment){
    char gcode[20];
    char float_str[20]; 
    switch(a){
        case X:
            sprintf(gcode, "%s%s%s",JOG_GCODE_BASE,"X", ftoa(float_str, increment, 5));
            printString(gcode);
            protocol_execute_line(gcode);
            printString("\n");
        break;
        case Y:
            sprintf(gcode, "%s%s%s",JOG_GCODE_BASE,"Y", ftoa(float_str, increment, 5));
            printString(gcode);
            protocol_execute_line(gcode);
            printString("\n");
        break;
        case Z:
            sprintf(gcode, "%s%s%s",JOG_GCODE_BASE,"Z", ftoa(float_str, increment, 5));
            printString(gcode);
            protocol_execute_line(gcode);
            printString("\n");
        break;
        default:
            printString("Incorrect axis\n");
        break;
    }
}

void reset_axis(AXIS a){
    char gcode[20];
    switch(a){
        case X:
            sprintf(gcode, "%s%s",RESET_AXIS_BASE,"X0");
            printString(gcode);
            protocol_execute_line(gcode);
        break;
        case Y:
            sprintf(gcode, "%s%s",RESET_AXIS_BASE,"Y0");
            printString(gcode);
            protocol_execute_line(gcode);
        break;
        case Z:
            sprintf(gcode, "%s%s",RESET_AXIS_BASE,"Z0");
            printString(gcode);
            protocol_execute_line(gcode);
        break;
        default:
            printString("Incorrect axis\n");
        break;
    }
}
