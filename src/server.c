#include "server.h"

char clientAddress[255];

double temperatureGoal = 20;

int main(int argc, char** argv) {
    if(argc != 2) {
        printf("Usage: %s <ip of client>\n", argv[0]);
        return 1;
    }

    setClientAddr(argv[1]);

    pthread_t tid[2];
    int err;

    err = pthread_create(tid+0, NULL, &listener, NULL);
    err = pthread_create(tid+1, NULL, &sender, NULL);

    pthread_join(tid[0], NULL);

    return 0;
}

void setClientAddr(const char* src) {
    //printf("%s\n", src);
    memset(clientAddress, '\0', sizeof(clientAddress));
    strcpy(clientAddress, src);
    //printf("%s\n", serverAddress);
}


void printMsg(AllData msg) {
    printf("====================\n");
    printf("Lumen: %d\n", msg.lumen);
    printf("Sound: %d\n", msg.sound);
    printf("Humidity: %3.1f\n", msg.humidity);
    printf("Temperature: %3.1f\n", msg.temperature);
    printf("====================\n");
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

        printMsg(msg);
    }
}

void* sender(void* arg) {
    int sock = 0;
    int n = 0;
    struct sockaddr_in servAddr;

    int noError = 1;
    ActuatorData msg;

    while(noError) {
        sleep(5);

        msg.type = CHANGE_COLOR;
        msg.value = 0;

        // send data
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if(sock == -1) {
            perror("socket()");
            continue;
        }

        memset(&servAddr, '0', sizeof(servAddr));
        servAddr.sin_family = AF_INET;
        servAddr.sin_port = htons(SEND_PORT);

        if(inet_pton(AF_INET, clientAddress, &servAddr.sin_addr) <= 0) {
            perror("inet_pton()");
            continue;
        }


        if(connect(sock, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0) {
            perror("connect()");
            continue;
        }

        send(sock, &msg, sizeof(msg), 1);
    }
}
