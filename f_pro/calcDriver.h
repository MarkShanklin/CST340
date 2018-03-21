#ifndef _MYIOCTL_H
#define _MYIOCTL_H
#include <linux/ioctl.h>
 
typedef struct
{
    int leftArg, rightArg, ans;
} query_arg_t;
#define MULT 1
#define ADD 2
#define SUB 3 
#define QUERY_GET_VARIABLES _IOR('q', 1, query_arg_t *)
#define QUERY_CLR_VARIABLES _IO('q', 2)
#define QUERY_SET_VARIABLES _IOW('q', 3, query_arg_t *)
 
#endif
