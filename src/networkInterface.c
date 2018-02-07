#include "networkInterface.h"

char serverAddress[255];

void setServerAddr(const char* src) {
    //printf("%s\n", src);
    memset(serverAddress, '\0', sizeof(serverAddress));
    strcpy(serverAddress, src);
    //printf("%s\n", serverAddress);
}

void* networkManager(void* arg) {
    pthread_t tid[2];
    int err;

    err = pthread_create(tid+0, NULL, &dataSender, NULL);
    err = pthread_create(tid+1, NULL, &commandReceiver, NULL);

    pthread_join(*(tid+0), NULL);

    /*
    char recvBuff[255];
    char sendBuff[255];

    int listenfd = 0;
    int connfd = 0;

    int n = 0; // size of recv message

    struct sockaddr_in servAddr;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if(listenfd == -1) {
        perror("socket()");
        return;
    }

    int enable = 1;
    if(setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
        perror("so reuse");
    }

    memset(&servAddr, '0', sizeof(servAddr));
    memset(sendBuff, '0', sizeof(sendBuff));
    memset(recvBuff, '0', sizeof(recvBuff));

    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(5100);

    if(bind(listenfd, (struct sockaddr*)&servAddr, sizeof(servAddr)) == -1) {
        perror("bind()");
    }

    listen(listenfd, 2);

    while(1) {
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);

        if((n = recv(connfd, recvBuff, sizeof(recvBuff)-1,0)) < 0) {
            printf("Nothing\n");
            close(connfd);
            continue;
            }
        recvBuff[n] = '\0';

        printf("Recv: %s\n", recvBuff);

        close(connfd);
    }
    */
}

void* dataSender(void* arg) {
    int sock = 0;
    int n = 0;
    struct sockaddr_in servAddr;

    int noError = 1;
    AllData msg;

    while(noError) {
        delay(DELAY_SENDER);

        // get data
        msg.lumen = getLumenVal();
        msg.sound = 0;
        msg.humidity = getHumidity();
        msg.temperature = getTemperature();


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
        servAddr.sin_port = htons(SENDER_PORT);

        if(inet_pton(AF_INET, serverAddress, &servAddr.sin_addr) <= 0) {
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

        send(sock, &msg, sizeof(msg), 0);
    }
}

void* commandReceiver(void* arg) {
    int listenfd = 0;
    int connfd = 0;
    int n = 0;
    ActuatorData msg;

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
#if DEBUG == 1
            perror("recv()");
#endif
            close(connfd);
            continue;
        }
        close(connfd);

        handleCommand(msg);
    }
}

void handleCommand(ActuatorData command) {
    switch(command.type) {
    case LUMEN:
        //printf("lumen\n");
        setLightTransition(command.value);
        break;

    case CHANGE_COLOR:
        //printf("color\n");
        changeColor();
        break;

    case HEATER:
        //printf("heate\n");
        dualSetColor((int)(command.value));
        break;

    default:
        break;
    }
}
