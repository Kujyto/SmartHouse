#include "networkInterface.h"

void* networkManager(void* arg) {
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
}

void udp() {

}
