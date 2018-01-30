/*
  Change color of RGB LED if double clap
 */

#ifndef DEF_CLAP_H
#define DEF_CLAP_H

#include "definition.h"
#include "actuators.h"
#include "sensorsManager.h"

#define DELAY_DOUBLE 400
#define MAX_COUNT 10
#define MIN_COUNT 1

#define SOUND_LIMIT 5

void* detectClap(void* arg);

#endif
