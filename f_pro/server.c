/***********************************************************
* File Name: server.c
* Purpose: Server to process calculations with driver
* Creation Date: 03/19/2018 
* Last Modified: Mon 19 Mar 2018 07:54 PM PDT
* Created By: Mark Shanklin
**********************************************************/
#include <stdio.h>



#define MAXLINE 32

int main(int argv,char argc[])
{
    int listenfd;
    int sockfd;
    int clientLen;
    struct sockaddr_in clientaddr;
    struct sockaddr_in serveraddr;
    char buf[MAXLINE];

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(SERV_PORT);

    if(bind(listenfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) != 0)
    {
        perror("BIND ERROR!");
        exit(-1);
    }

    listen(listenfd, LISTENQ);
    sockfd = accept(listenfd, (struct sockaddr *) &clientaddr, &clientLen);
    int calc = true;
    int readLen;

    while(calc){

        if((readLen = read(sockfd, buf, sizeof(buf))) == 0) {
            calc = false;
            close(sockfd);
            break;
        } 
        else 
        {
            calc(&buf);
            write(sockfd, buf, strlen(buf));
        }
    }
    return 0;
};

