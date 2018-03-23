/***********************************************************
* File Name: driver.h
* Purpose: ioctl driver header
* Creation Date: 03/21/18
* Last Modified: 03/21/18
* Created By: Mark Shanklin
**********************************************************/
#ifndef _MYDRIVER_H
#define _MYDRIVER_H
#include <linux/ioctl.h>
 
typedef struct
{
    int leftArg, rightArg, answer;
    char buffer[512];
} query_arg_t;

#define QUERY_GET_VARIABLES _IOR('q', 1, query_arg_t *)
#define QUERY_CLR_VARIABLES _IO('q', 2)
#define QUERY_SET_VARIABLES _IOW('q', 3, query_arg_t *)
#define QUERY_NULL_FILL _IOW('q', 4, query_arg_t *)
#define QUERY_A_FILL _IOW('q', 5, query_arg_t *)
#define QUERY_STATUS_BUFFER _IOR('q', 6, query_arg_t *) 
#endif