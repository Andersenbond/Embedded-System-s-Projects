#ifndef _VARIABLES_H_
#define _VARIABLES_H_

#include "pthread.h"
#include "stdio.h"
#include <mqueue.h>
#include <sys/stat.h>
#include <sys/types.h>


#define SERVER "192.168.43.2"
#define PORT 9000

#define SERVER2 "localhost"
#define PORT2 50008


struct IPC{
	pthread_mutex_t mutex;
 	pthread_cond_t  cond;
	int value;
};

#endif
