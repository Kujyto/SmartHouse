#ifndef DEF_SENSORS_MANAGER_H
#define DEF_SENSORS_MANAGER_H

#include "definition.h"

#define MAXTIMINGS 85 // humiture

#define DELAY_HUMITURE 5000 // 5sec
#define DELAY_LUMEN 1000 // 1sec

void* sensorsManager(void* arg);

int getLumenVal();
double getHumidity();
double getTemperature();

// private
void* buttonManager(void* arg);
void* humitureManager(void* arg);
void* lumenManager(void* arg);

void read_dht11_dat();

#endif
