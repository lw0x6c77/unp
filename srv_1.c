#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define ADDR        "192.168.230.128"
#define PORT        6000
#define BACKLOG     128

int main(int argc, char *argv[])
{
    struct sockaddr_in servaddr, cliaddr;
    socklen_t addrlen = sizeof(struct sockaddr_in);
    int listenfd = 0, connfd = 0;
    char str[] = "hello world\n";
    size_t slen = sizeof(str) / sizeof(char);

    memset(&servaddr, 0, addrlen);
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    if(inet_pton(AF_INET, ADDR, &servaddr.sin_addr) != 1) {
        printf("inet_pton error\n");
        exit(1);
    }

    if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        printf("create socket error\n");
        exit(1);
    }

    if(bind(listenfd, (const struct sockaddr *)&servaddr, addrlen) != 0) {
        printf("bind error\n");
        exit(1);
    }

    if(listen(listenfd, BACKLOG) != 0) {
        printf("listen error\n");
        exit(1);
    }

    for( ; ; ) {
        if((connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &addrlen)) == -1) {
            printf("accept error\n");
            exit(1);
        }
        printf("accepted connection, fd = %d\n", connfd);
        if(write(connfd, str, slen) != slen) {
            printf("write error\n");
        }
        close(connfd);
    }

    return 0;
}
