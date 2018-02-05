#ifndef DEF_SERVER_H
#define DEF_SERVER_H

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

#define RECV_PORT 5001 // recever port

void printMsg(AllData msg);

#endif
