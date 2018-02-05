#ifndef DEF_NETWORK_INTERFACE_H
#define DEF_NETWORK_INTERFACE_H

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

#include "messageDefinition.h"
#include "portableSockets.h"

#include "sensorsManager.h"

#define SENDER_PORT 5001 // raspberry sender port

#define DELAY_SENDER 1000 // 1sec

void setServerAddrr(const char* src);

void* networkManager(void* arg);

void* dataSender(void* arg); // send data via UDP

#endif
