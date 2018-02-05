#include "server.h"

int main() {
    int listenfd = 0;
    int connfd = 0;
    int n = 0;
    AllData msg;

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


void printMsg(AllData msg) {
    printf("====================\n");
    printf("Lumen: %d\n", msg.lumen);
    printf("Sound: %d\n", msg.sound);
    printf("Humidity: %f\n", msg.humidity);
    printf("Temperature: %f\n", msg.temperature);
    printf("====================\n");
}
