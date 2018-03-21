/***********************************************************
* File Name     : calc.c
* Purpose       : Ncurses Style Calculator
* Creation Date : 03-12-2018
* Last Modified : Fri 16 Mar 2018 05:43:34 PM PDT
* Created By    : Mark Shanklin 
***********************************************************/
#include<ncurses.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>

#define X_MAX 30
#define PORT 50050

void drawCalc();
int calcController();
char* sendData(char*);

int main(void)
{
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    do
    {
        clear();
        refresh();
        drawCalc();
    } while(calcController());
    endwin();
    return 0;
}

int calcController()
{
    int c = 0;
    int exit = FALSE;
    char calcData[X_MAX];
    memset(calcData, 0, X_MAX);
    MEVENT event;
    mousemask(ALL_MOUSE_EVENTS, NULL);
    while(!exit)
    {
        int len = strlen(calcData);
        if(len > 25) len = 25;
        c = wgetch(stdscr);
        switch(c)
        {
            case KEY_MOUSE:
            if(getmouse(&event) == OK)
            {
                if(event.bstate & BUTTON1_CLICKED)
                {
                    if (event.y > 4 && event.y <= 7)
                    {
                        if(event.x >= 0 && event.x < 5)
                        {
                            calcData[len] = '7';
                        }
                        if(event.x >= 6 && event.x < 11)
                        {
                            calcData[len] = '8';
                        }
                        if(event.x >= 12 && event.x < 17)
                        {
                            calcData[len] = '9';
                        }
                        if(event.x >= 18 && event.x < 23)
                        {
                            calcData[len] = '+';
                        }
                        if(event.x >= 24 && event.x < 29)
                        {
                            memset(calcData, 0, X_MAX);
                            return TRUE;
                        }
                    }
                    else if (event.y > 7 && event.y <= 10)
                    {
                        if(event.x >= 0 && event.x < 5)
                        {
                            calcData[len] = '4';
                        }
                        if(event.x >= 6 && event.x < 11)
                        {
                            calcData[len] = '5';
                        }
                        if(event.x >= 12 && event.x < 17)
                        {
                            calcData[len] = '6';
                        }
                        if(event.x >= 18 && event.x < 23)
                        {
                            calcData[len] = '-';
                        }
                        if(event.x >= 24 && event.x < 29)
                        {
                            calcData[len] = '*';
                        }
                    }
                    else if (event.y > 10 && event.y <= 13)
                    {
                        if(event.x >= 0 && event.x < 5)
                        {
                            calcData[len] = '1';
                        }
                        if(event.x >= 6 && event.x < 11)
                        {
                            calcData[len] = '2';
                        }
                        if(event.x >= 12 && event.x < 17)
                        {
                            calcData[len] = '3';
                        }
                        if(event.x >= 18 && event.x < 23)
                        {
                            calcData[len] = '0';
                        }
                        if(event.x >= 24 && event.x < 29)
                        {
                            mvprintw(3,1,"                         ");
                            mvprintw(3,(X_MAX - 3)-len, sendData(calcData));
                        }
                    }
                }
            }
            break;
            case 'c':
                memset(calcData, 0, X_MAX);
                return TRUE;
                break;
            case 27:
                exit = TRUE;
                break;
            case KEY_BACKSPACE:
                calcData[len-1] = '\0';
                len--;
                mvprintw(1,1,"                         ");
                break;
            case KEY_DC:
                calcData[len-1] = '\0';
                len--;
                mvprintw(1,1,"                         ");
                break;
            case KEY_ENTER:
                mvprintw(3,1,"                         ");
                mvprintw(3,(X_MAX - 3)-len, calcData);
                break;
            case '=':
                mvprintw(3,1,"                         ");
                mvprintw(3,(X_MAX - 3)-len, calcData);
                break;
            case '+':
                calcData[len] = '+';
                break;
            case '*':
                calcData[len] = '*';
                break;
            case '-':
                calcData[len] = '-';
                break;
            case '1':
                calcData[len] = '1';
                break;
            case '2':
                calcData[len] = '2';
                break;
            case '3':
                calcData[len] = '3';
                break;
            case '4':
                calcData[len] = '4';
                break;
            case '5':
                calcData[len] = '5';
                break;
            case '6':
                calcData[len] = '6';
                break;
            case '7':
                calcData[len] = '7';
                break;
            case '8':
                calcData[len] = '8';
                break;
            case '9':
                calcData[len] = '9';
                break;
            case '0':
                calcData[len] = '0';
                break;
        }
        mvprintw(1,1,calcData);   
    }
    return !exit;
}

char* sendData(char sendBuff[32])
{
    int sockfd = 0, n = 0;
    char recvBuff[32];

    struct sockaddr_in serverAddr;
    memset(recvBuff, '0',sizeof(recvBuff));

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        fprintf(stderr,"\n ERROR : COULDN'T CREATE SOCKET \n");
        return EXIT_FAILURE;
    }

    memset(&serverAddr, '0', sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(50050); 

    if( connect(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
       fprintf(stderr,"\n ERROR : CONNECT FAILED \n");
       return EXIT_FAILURE;
    }

    write(sockfd, sendBuff, strlen(sendBuff));

    while ( (n = read(sockfd, recvBuff, sizeof(recvBuff)-1)) > 0)
    {
        //do the test needed for the lab

        recvBuff[n] = 0;
        if(fputs(recvBuff, stdout) == EOF)
        {
            fprintf(stderr,"\n ERROR : FPUTS ERROR\n");
        }
    } 
    if(n < 0)
    {
        fprintf(stderr,"\n ERROR : READ ERROR \n");
    }

    return recvBuff;
}

void drawCalc() 
{
    int x,y;
    x= 0;
    y= 0;
    
    mvhline(y, x, '-', X_MAX-1);
    mvhline(y+4, x, '-', X_MAX-1);
    mvvline(y+1, x, '|', 3);
    mvvline(y+1, x+X_MAX-2, '|', 3);
    y = 5;
    for(int i=0;i<15; i++)
    {   
        char data[]= "789+C456-*1230=";
        if(i == 0)
            {}
        else if(i == 5 || i == 10 || i == 15)
            {y = y+3; x = 0;}
        else
            {x = x+(X_MAX/5);}
        mvhline(y, x, '-', (X_MAX/5)-1);
        mvhline(y+2, x, '-', (X_MAX/5)-1);
        mvvline(y+1, x, '|', 1);
        mvvline(y+1, x+(X_MAX/5)-2, '|', 1); 
        mvprintw(y+1, x+2, "%c", data[i]);   
    }
    move(1,1);
    refresh();
}
