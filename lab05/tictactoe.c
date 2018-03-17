/***********************************************
* Made By: Mark Shanklin
* Purpose: Tic tac toe board game 
***********************************************/
#include<ncurses.h>
#include<unistd.h>
#include<string.h>

#define X_MAX 33
#define Y_MAX 18

void drawGameBoard();
int gameController();
int setPiece(int, int *);
void insertO(int,int);
void insertX(int,int);

static int boardData[9];

int main(void)
{
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    do
    {
        clear();
        for(int i = 0; i < 10; i++) boardData[i] = i+2;
        drawGameBoard();
    } while(gameController());
    endwin();
    return 0;
}

int gameController()
{
    int player = 0;
    int c = 0;
    int win = 1;
    MEVENT event;
    int setSuccess = 0;
    mousemask(ALL_MOUSE_EVENTS, NULL);
    for(int i = 0; i < 10;)
    {
        if(player == 0)
        {
            mvprintw(22, 1, "It's O Players Turn");
        }
        else if(player == 1)
        {
            mvprintw(22, 1, "It's X Players Turn");
        }
        refresh();
        c = wgetch(stdscr);
        switch(c)
        {
            case KEY_MOUSE:
            if(getmouse(&event) == OK)
            {
                if(event.bstate & BUTTON1_CLICKED)
                {
                    int myX, myY;
                    myX = event.x;
                    myY = event.y;
                    //check for clicks on first row
                    if(myY < 7)
                    {
                        if(myX < 12)
                        {
                            setSuccess = setPiece(1, &player);
                        }
                        else if(myX >= 12 && myX < 22)
                        {    
                            setSuccess = setPiece(2, &player);
                        }
                        else 
                        {
                            setSuccess = setPiece(3, &player);
                        }
                    }
                    //check for clicks on second row
                    else if (myY >= 7 && myY < 14)     
                    {
                        if(myX < 12)
                        {
                            setSuccess = setPiece(4, &player);
                        }
                        else if(myX >= 12 && myX < 22)
                        {
                            setSuccess = setPiece(5, &player);
                        }
                        else 
                        {
                            setSuccess = setPiece(6, &player);
                        }
                    } 
                    //check for clicks on third row
                    else 
                    {
                        if(myX < 12)
                        {
                            setSuccess = setPiece(7, &player);
                        }
                        else if(myX >= 12 && myX < 22)
                        {
                            setSuccess = setPiece(8, &player);
                        }
                        else
                        {
                            setSuccess = setPiece(9, &player);
                        }
                    }
                    {
                        i++;
                    }
                }
            }
            break;
            case 27:
                return FALSE;
                break;
        }   
    if(setSuccess == 1)
    {
        /* Check rows */
        if( boardData[0] == boardData[1] && boardData[1] == boardData[2] )
            win = 1;
        else if( boardData[3] == boardData[4] && boardData[4] == boardData[5] )
            win = 1;
        else if( boardData[6] == boardData[7] && boardData[7] == boardData[8] )
            win = 1;

        /* Check cols */
        else if( boardData[0] == boardData[3] && boardData[3] == boardData[6] )
            win = 1;
        else if( boardData[1] == boardData[4] && boardData[4] == boardData[7] )
            win = 1;
        else if( boardData[2] == boardData[5] && boardData[5] == boardData[8] )
            win = 1;

        /* Check diagonals */
        else if( boardData[0] == boardData[4] && boardData[4] == boardData[8] )
            win = 1;
        else if( boardData[2] == boardData[4] && boardData[4] == boardData[6] )
            win = 1;
        else
            win = 0;
    }
    if(win == 1)
    {
        if(player == 1)
        {
            mvprintw(22, 1, "!!!!!!!!! O Player Wins !!!!!!!!!");
        }
        else if(player == 0)
        {
            mvprintw(22, 1, "!!!!!!!!! X Player Wins !!!!!!!!!");
        }
        refresh();
        sleep(2);
        break;
    }
    if(win == 0 && i == 9)
    {
        mvprintw(22, 1, "!!!!!!!!! Cat's Scratch !!!!!!!!!");
        refresh();
        sleep(2);
        break;
    }    
    }
    return win;
}

int setPiece(int pos, int * player)
{
    int setSuccess = 0;
    if(pos == 1 && boardData[pos-1] > 1)
    {
        if(*player == 0)
        {
            insertO(4,1);
            setSuccess = 1;
        }
        else if(*player == 1)
        {
            insertX(4,1);
            setSuccess = 1;
        }
    }
    else if(pos == 2 && boardData[pos-1] > 1)
    {
        if(*player == 0)
        {
            insertO(15, 1);
            setSuccess = 1;
        }
        else if(*player == 1)
        {
            insertX(15, 1);
            setSuccess = 1;
        }
    }
    else if(pos == 3 && boardData[pos-1] > 1)
    {
        if(*player == 0)
        {
            insertO(26, 1);
            setSuccess = 1;
        }
        else if(*player == 1)
        {
            insertX(26, 1);
            setSuccess = 1;
        }
    }
    else if(pos == 4 && boardData[pos-1] > 1)
    {
        if(*player == 0)
        {
            insertO(4, 7);
            setSuccess = 1;
        }
        else if(*player == 1)
        {
            insertX(4, 7);
            setSuccess = 1;
        }
    }
    else if(pos == 5 && boardData[pos-1] > 1)
    {
        if(*player == 0)
        {
            insertO(15, 7);
            setSuccess = 1;
        }
        else if(*player == 1)
        {
            insertX(15, 7);
            setSuccess = 1;
        }
    }
    else if(pos == 6 && boardData[pos-1] > 1)
    {
        if(*player == 0)
        {
            insertO(26, 7);
            setSuccess = 1;
        }
        else if(*player == 1)
        {
            insertX(26, 7);
            setSuccess = 1;
        }
    }
    else if(pos == 7 && boardData[pos-1] > 1)
    {
        if(*player == 0)
        {
            insertO(4, 13);
            setSuccess = 1;
        }
        else if(*player == 1)
        {
            insertX(4, 13);
            setSuccess = 1;
        }
    }
    else if(pos == 8 && boardData[pos-1] > 1)
    {
        if(*player == 0)
        {
            insertO(15, 13);
            setSuccess = 1;
        }
        else if(*player == 1)
        {
            insertX(15, 13);
            setSuccess = 1;
        }
    }
    else if(pos == 9 && boardData[pos-1] > 1)
    {
        if(*player == 0)
        {
            insertO(26, 13);
            setSuccess = 1;
        }
        else if(*player == 1)
        {
            insertX(26, 13);
            setSuccess = 1;
        }
    }
    if(*player == 0 && setSuccess == 1)
    {
        *player = 1;
    }
    else if(*player == 1 && setSuccess == 1)
    {
        *player = 0;
    }
    boardData[pos-1] = *player;
    mvprintw(24, 1, "Set Success");
    return setSuccess;
}

void insertO(int x, int y)
{
    mvprintw(y+0, x, " OOO  ");
    mvprintw(y+1, x, "O   O ");
    mvprintw(y+2, x, "O   O ");
    mvprintw(y+3, x, "O   O ");
    mvprintw(y+4, x, " OOO  ");
    refresh();
}

void insertX(int x, int y)
{
    mvprintw(y+0, x, "X   X ");
    mvprintw(y+1, x, " X X  ");
    mvprintw(y+2, x, "  X   ");
    mvprintw(y+3, x, " X X  ");
    mvprintw(y+4, x, "X   X ");
    refresh();
}

void drawGameBoard() 
{
    int i, j;
    for(i=0; i<=Y_MAX; i++)
    {
        for(j=0; j<=X_MAX; j++)
        {
            if(i == 0)
            {   
                if(j == 0)
                {
                    mvaddch(i, j, ' ');
                }
                else if(j == X_MAX)
                {
                    mvaddch(i, j, ' ');
                }
                else if(j %(X_MAX/3) == 0)
                {
                    mvaddch(i, j, ' ');
                }
                else
                {
                    mvaddch(i, j, ' ');
                }
            }
            else if(i % (Y_MAX / 3) == 0 && i != Y_MAX)
            {
                if(j == 0)
                {
                    mvaddch(i, j, ' ');
                }
                else if(j == X_MAX)
                {
                    mvaddch(i, j, ' ');
                }
                else if( j % (X_MAX / 3) == 0)
                {
                    mvaddch(i, j, '|');
                }
                else
                {
                    mvaddch(i, j, '-');
                }   
            }
            else if(i == Y_MAX)
            {
                if(j == 0)
                {
                    mvaddch(i, j, ' ');
                }
                else if(j == X_MAX)
                {
                    mvaddch(i, j, ' '); 
                }
                else if(j % (X_MAX / 3) == 0)
                {
                    mvaddch(i, j, ' ');
                }
                else
                {
                    mvaddch(i, j, ' ');
                } 
            }
            else if(j % (X_MAX / 3) == 0)
            {
                if(j == 0)
                {
                    mvaddch(i, j, ' ');
                }
                else if(j == X_MAX)
                {
                    mvaddch(i, j, ' ');
                }
                else
                {
                    mvaddch(i, j, '|');
                }
            }
        }
    }
    refresh();
}
