/***********************************************************
* File Name: server.c
* Purpose: Server to process calculations with driver
* Creation Date: 03/19/2018 
* Last Modified: Mon 19 Mar 2018 07:54 PM PDT
* Created By: Mark Shanklin
**********************************************************/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>

#define MAXLINE 32
#define LISTENQ 1
#define SERV_PORT 50050

int main(int argc,char *argv[])
{
    int sockfd;
    int clientLen;

    char buff[MAXLINE];

    struct sockaddr_in serverAddr;
    struct sockaddr_in clientAddr;

    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset((char*)&serverAddr, 0, sizeof(serverAddr));

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(SERV_PORT);

    if(bind(listenfd, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) != 0)
    {
        perror("BIND ERROR!");
        exit(-1);
    }
    int bound = sock2port(listenfd);
    printf("\nBound to port: %d", bound);

    listen(listenfd, LISTENQ);
    sockfd = accept(listenfd, (struct sockaddr *) &clientAddr, &clientLen);
    int calc = true;
    int readLen;

    while(calc){

        if((readLen = read(sockfd, buff, sizeof(buff))) == 0) {
            calc = false;
            break;
        } 
        else 
        {
            //calc(&buff);
            write(sockfd, buff, strlen(buff));
        }
        close(sockfd);
    }
    return 0;
};

