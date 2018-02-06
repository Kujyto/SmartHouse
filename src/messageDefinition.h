#ifndef DEF_MESSAGE_H
#define DEF_MESSAGE_H

#define delimiter ','

#define GET_TEMP "getTemp"

/*
typedef enum {
    LUMEN, // light level
    CHANGE_COLOR,
    HEATER // > 0 heater, < 0 air-conditioner, 0 off
} DataType;
*/

// types
#define LUMEN 0
#define CHANGE_COLOR 1
#define HEATER 2

typedef struct {
    int type;
    double value;
} ActuatorData;

typedef struct {
    int lumen;
    int sound;
    double humidity;
    double temperature;
} AllData;

#endif
