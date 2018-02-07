#include "server.h"

char clientAddress[255];

double temperatureGoal = 20;

int heater = 0;
double temp = 20;
static pthread_mutex_t mutexTemp = PTHREAD_MUTEX_INITIALIZER;

int lumen = 0;
static pthread_mutex_t mutexLumen = PTHREAD_MUTEX_INITIALIZER;


int main(int argc, char** argv) {
    if(argc != 2) {
        printf("Usage: %s <ip of client>\n", argv[0]);
        return 1;
    }

    setClientAddr(argv[1]);

    pthread_t tid[3];
    int err;

    err = pthread_create(tid+0, NULL, &listener, NULL);
    err = pthread_create(tid+1, NULL, &sender, NULL);
    err = pthread_create(tid+2, NULL, &temperatureManager, NULL);

    pthread_join(tid[0], NULL);

    return 0;
}

void setClientAddr(const char* src) {
    memset(clientAddress, '\0', sizeof(clientAddress));
    strcpy(clientAddress, src);
}


void printMsg(AllData msg) {
    printf("====================\n");
    printf("Lumen: %d\n", msg.lumen);
    printf("Sound: %d\n", msg.sound);
    printf("Humidity: %3.1f\n", msg.humidity);
    printf("Temperature: %3.1f\n", msg.temperature);
    printf("====================\n");
}

void updateValues(AllData msg) {
    pthread_mutex_lock(&mutexTemp);
    temp = msg.temperature;
    pthread_mutex_unlock(&mutexTemp);

    pthread_mutex_lock(&mutexLumen);
    lumen = msg.lumen;
    pthread_mutex_unlock(&mutexLumen);
}

void* listener(void* arg) {
    int listenfd = 0;
    int connfd = 0;
    int n = 0;
    AllData msg;

    struct sockaddr_in servAddr;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if(listenfd == -1) {
        perror("socket()");
        return NULL;
    }

    int enable = 1;
    if(setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
        perror("so reuse");
    }

    memset(&servAddr, '0', sizeof(servAddr));

    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(RECV_PORT);

    if(bind(listenfd, (struct sockaddr*)&servAddr, sizeof(servAddr)) == -1) {
        perror("bind()");
        exit(errno);
    }

    listen(listenfd,2);

    while(1) {
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);

        if((n = recv(connfd, &msg, sizeof(msg),0)) < 0) {
            close(connfd);
            continue;
        }

        updateValues(msg);
        //printMsg(msg);
    }
}

double getLightLevel() {
    pthread_mutex_lock(&mutexLumen);
    int lumenVal = lumen;
    pthread_mutex_unlock(&mutexLumen);

    int lightPercent = 100 * (lumenVal-LIGHT_MIN) / (LIGHT_MAX - LIGHT_MIN);

    if(lightPercent < 1) {
        lightPercent = 1;
    }
    else if(lightPercent > 100) {
        lightPercent = 100;
    }

    return (100-lightPercent) / 100.0;
}

void* sender(void* arg) {
    int sock = 0;
    int n = 0;
    struct sockaddr_in servAddr;

    int noError = 1;
    ActuatorData msg;

    while(noError) {
        sleep(5);

        // send data
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if(sock == -1) {
#if DEBUG == 1
            perror("socket()");
#endif
            continue;
        }

        memset(&servAddr, '0', sizeof(servAddr));
        servAddr.sin_family = AF_INET;
        servAddr.sin_port = htons(SEND_PORT);

        if(inet_pton(AF_INET, clientAddress, &servAddr.sin_addr) <= 0) {
#if DEBUG == 1
            perror("inet_pton()");
#endif
            continue;
        }


        if(connect(sock, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0) {
#if DEBUG == 1
            perror("connect()");
#endif
            continue;
        }


        // temperature manager
        msg.type = HEATER;
        msg.value = -1000;
        pthread_mutex_lock(&mutexTemp);
        if(temp >= temperatureGoal - DELTA_TEMP) {
            if(temp > temperatureGoal + DELTA_TEMP && heater >= 0) {
                msg.value = -1;
            }
            else if(heater > 0) {
                msg.value = 0;
            }
        }
        else if(temp <= temperatureGoal + DELTA_TEMP) {
            if(temp < temperatureGoal - DELTA_TEMP && heater <= 0) {
                msg.value = 1;
            }
            else if(heater < 0) {
                msg.value = 0;
            }
        }
        pthread_mutex_unlock(&mutexTemp);

        if(msg.value > -100) {
            send(sock, &msg, sizeof(msg), 0);
            heater = msg.value;
        }

        // light manager
        msg.type = LUMEN;
        msg.value = getLightLevel();
        send(sock, &msg, sizeof(msg),0);
    }
}

void clean(const char *buffer, FILE *fp) {
    char *p = strchr(buffer,'\n');
    if(p != NULL) {
        *p = 0;
    }
    else {
        int c;
        while((c = fgetc(fp)) != '\n' && c != EOF);
    }
}

void* temperatureManager(void* arg) {
    int tempRef = temperatureGoal;

    char s[255];

    while(1) {
        printf("Current temperature goal: %d\n", tempRef);
        printf("New temperature goal: ");

        fgets(s, sizeof(s), stdin);
        clean(s,stdin);
        sscanf(s,"%d",&tempRef);

        pthread_mutex_lock(&mutexTemp);
        temperatureGoal = tempRef;
        pthread_mutex_unlock(&mutexTemp);
        printf("New temperature goal registered\n\n");
    }
}
