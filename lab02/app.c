/***********************************************************
* File Name: app.c
* Purpose: ioctl Application test frame
* Creation Date: 03/21/18
* Last Modified: 03/21/18
* Created By: Mark Shanklin
**********************************************************/
#include <getopt.h>
#include <unistd.h>


int nullFill()
{
    char *file_name = "/dev/query";
    int fd;
    fd = open(file_name, O_RDWR);
    if(ioctl(fd, QUERY_NULL_FILL, &args) == -1)
    {
        perror("Driver Null Fill Error.")
    }
    
    close(fd);
    return 0;
}

int bufferStatus()
{
    char *file_name = "/dev/query";
    int fd;
    int retVal = -1;
    fd = open(file_name, O_RDWR);
    int bufStat = ioctl(fd, QUERY_STATUS_BUFFER, &args);
    switch(bufStat)
    {
        case -1:
            perror("Driver Status Failure");
            break;
        case 0:
            retVal = 0;
            break;
        case 1:
            retVal = 1;
            break;
        default:
            perror("Driver Error Unknown Results");
            break;
    }

    close(fd);
    return retVal;
}

int aFill()
{
    char *file_name = "/dev/query";
    int fd;
    fd = open(file_name, O_RDWR);
    if(ioctl(fd, QUERY_A_FILL, &args) == -1)
    {
        perror("Driver A Fill Error.")
    }


    close(fd);
    return 0;
}

int main(int argc, char const *argv[])
{
    int command = 0;

    while ((command = getopt(argc, argv, "zash")) != -1) 
    {
        switch (command)
        {
        case 'z':
            nullFill();
            break;
        case 'a':
            printf("Buffer Status: %d\n", aFill());
            break;
        case 's':
            bufferStatus();
            break;
        case 'h':
            printf("This program is used to "
                   "test ioctl functions from driver.\n\n"
                   "-z \t<fill with NULL's>\n"
                   "-a \t<fill with A's>\n"
                   "-s \t<get the status of buffer>\n");
            break;
        case '?':
            break;
        }
    }

    return 0;
}