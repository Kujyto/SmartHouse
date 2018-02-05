#ifndef DEF_MESSAGE_H
#define DEF_MESSAGE_H

#define delimiter ','

#define GET_TEMP "getTemp"

typedef enum {LUMEN, SOUND, HUMIDITY, TEMPERATURE} DataType;

typedef struct {
    DataType type;
    double value;
} SensorsData;

typedef struct {
    int lumen;
    int sound;
    double humidity;
    double temperature;
} AllData;

#endif
