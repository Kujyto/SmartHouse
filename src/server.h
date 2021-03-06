#ifndef DEF_SERVER_H
#define DEF_SERVER_H

#define DEBUG 0

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>

#include <pthread.h>

#include "messageDefinition.h"

#define RECV_PORT 5001 // recever port
#define SEND_PORT 5002

#define DELTA_TEMP 0.25

#define LIGHT_MIN 50
#define LIGHT_MAX 200

void setClientAddr(const char* src);
void printMsg(AllData msg);

void* listener(void* arg);
void* sender(void* arg);
void* temperatureManager(void* arg);

#endif
