#ifndef DEF_DEFINITION_H
#define DEF_DEFINITION_H

#include <stdio.h>
#include <stdlib.h>
#include <pcf8591.h>
#include <pthread.h>
#include <softPwm.h>
#include <wiringPi.h>
#include <stdint.h>

#define PCF 120
#define LUMEN_PIN PCF+0

#define HumiturePin  0

#define DualGreenPin 2
#define DualRedPin   3

#define LedPinRed    4
#define LedPinGreen  5
#define LedPinBlue   6

#define ButtonPin    26

#define uchar unsigned char

#endif
