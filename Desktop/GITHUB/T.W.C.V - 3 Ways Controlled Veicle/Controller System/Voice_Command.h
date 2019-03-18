#ifndef _VOICE_H_
#define _VOICE_H_

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <pthread.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <semaphore.h>
#include "My_Var_Types.h"
#include "PracticalSocket.h"  // For Socket and SocketException



class CVoiceCommand{
     private: 
	pthread_t VoiceCommandThreadID;
	static void *VoiceCommandHandler(void *args);
        char Voice_Command[32];
	int Command_ID;
	TCPSocket Input_sock;
	TCPSocket Output_sock;
     public:
	CVoiceCommand(void);
	~CVoiceCommand(void);
	int init(void);
	int  createThread(void);
};


extern IPC IPC_Thread_Sync;
extern IPC Button_Voice_Sync;




#endif

