#ifndef DEF_LIGHTS_MANAGER_H
#define DEF_LIGHTS_MANAGER_H

#include "definition.h"

#include "actuators.h"
#include "sensorsManager.h"

#define DELAY_LIGHTS 1000
#define LIGHT_MAX 200
#define LIGHT_MIN 50

#define LIGHT_TRANSITION 50

void* lightsManager(void* arg);

void setLightToPercent(int percent);

#endif
