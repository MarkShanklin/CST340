/***********************************************************
* File Name: server.c
* Purpose: Server to process calculations with driver
* Creation Date: 03/19/2018 
* Last Modified: Mon 19 Mar 2018 07:54 PM PDT
* Created By: Mark Shanklin
**********************************************************/

#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include "calcDriver.h"

#define PORT 8080

void calc(char *);

int main(int argc, char const *argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
      
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
      
    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                  &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
      
    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address, 
                                 sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    while(1)
    {
        if (listen(server_fd, 3) < 0)
        {   
            perror("listen");
            exit(EXIT_FAILURE);
        }
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
                       (socklen_t*)&addrlen))<0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        valread = read( new_socket , buffer, 1024);
        calc(&buffer);
        printf("%s\n",buffer );
        send(new_socket , buffer , strlen(buffer) , 0 );
        printf("Data message sent\n");
    }
    return 0;
};

void calc(char * data)
{
    printf("calc called\n");
    int op = 0;
    int correct = 0;
    char sendData[1024];
    int len = strlen(data);
    int fd;
    char *file_name = "/dev/query";
    fd = open(file_name, O_RDWR);
    if(fd == -1)
    {
        strcpy(data,"Error open");
        return;
    }
    query_arg_t cArg;
    for(int i = 0; i < len; i++)
    {
        if(data[i] == '*')
        {
            op = 1;
            correct++;
        }
        else if(data[i] == '+')
        {
            op = 2;
            correct++;
        }
        else if(data[i] == '-')
        {
            op = 3;
            correct++;
        }
        else
        {
            sendData[i] = data[i];
            printf("sendData: %c, data: %c\n", sendData[i], data[i]);
        }

        if(correct == 1)
        {
            cArg.leftArg = atoi(sendData);
            printf("Left: %d\n", cArg.leftArg);
            memset(sendData,0,strlen(sendData));
            correct = 0;
            cArg.ans = op;
            printf("OP: %d\n", op);
        }

    }

    cArg.rightArg = atoi(sendData);
    printf("Right: %d\n",cArg.rightArg);
    if(ioctl(fd, QUERY_SET_VARIABLES, &cArg) == -1)
    {
        strcpy(data,"Error ioctl");
        return;
    }
    if(ioctl(fd, QUERY_GET_VARIABLES, &cArg) == -1)
    {
        strcpy(data,"Error ioctl");
        return;
    }
    memset(data, 0, strlen(data));
    printf("Answer: %d\n",cArg.ans);
    snprintf(data,strlen(data), "%d", cArg.ans);
    //itoa(cArg.rightArg,data,10);
    return;
}