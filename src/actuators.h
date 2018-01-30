#ifndef DEF_ACTUATORS_H
#define DEF_ACTUATORS_H

#include "definition.h"

typedef enum Color{RED, GREEN, BLUE} Color;

void changeColor();
void setLightLevel(double level);

void setColor(Color c);

void ledInit();
void ledColorSet(uchar r_val, uchar g_val, uchar b_val);

void* updateActuators(void* arg);
#endif
