#include "client.h"

int main(int argc, char** argv) {
    if(argc != 2) {
        printf("Usage: %s <ip of server>\n", argv[0]);
        return 1;
    }

    int sockfd = 0;
    int n = 0;
    struct sockaddr_in servAddr;

    int noError = 1;
    char sendBuff[255];

    while(noError) {
        snprintf(sendBuff, sizeof(sendBuff), "%s", GET_TEMP);

        memset(&servAddr, '0', sizeof(servAddr));
        servAddr.sin_family = AF_INET;
        servAddr.sin_port = htonl(5001);

        if(inet_pton(AF_INET, argv[1], &servAddr.sin_addr) <= 0) {
            perror("inet_pton()");
            continue;
        }

        if(connect(sockfd, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0) {
            perror("connect()");
            continue;
        }

        send(sockfd, sendBuff, strlen(sendBuff), 0);
    }
}
