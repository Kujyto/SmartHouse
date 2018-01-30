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

void* networkManager(void* arg);

#endif
