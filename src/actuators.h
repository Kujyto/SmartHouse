#ifndef DEF_ACTUATORS_H
#define DEF_ACTUATORS_H

#include "definition.h"

#define LIGHT_TRANSITION 50

typedef enum Color{RED, GREEN, BLUE, WHITE} Color;

void toggleLights();
void changeColor();
void setLightTransition(double level);
void setLightLevel(double level);

void setColor(Color c, uchar l);

void ledInit();
void ledColorSet(uchar r_val, uchar g_val, uchar b_val);

/*
  Set color of Dual-Color LED
  i < 0 => G
  i > 0 => R
  i == 0 => no color
 */
void dualSetColor(int i);

void* updateActuators(void* arg);
#endif
