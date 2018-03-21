/***********************************************************
* File Name: driver.c
* Purpose: ioctl driver
* Creation Date: 03/21/18
* Last Modified: 03/21/18
* Created By: Mark Shanklin
**********************************************************/

int main(int argc, char const *argv[])
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