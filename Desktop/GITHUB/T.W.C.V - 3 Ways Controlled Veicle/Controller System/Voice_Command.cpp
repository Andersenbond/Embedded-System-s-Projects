#include "Voice_Command.h"



extern sem_t Record_semaphore;
extern IPC Button_Voice_Sync;



void * CVoiceCommand::VoiceCommandHandler(void *args){
	char send[2];	
	char temp[2];
	while(1){
		sem_wait(&Record_semaphore);
		
		pthread_mutex_lock(&IPC_Thread_Sync.mutex);
		if(IPC_Thread_Sync.value == 1){
			 pthread_cond_wait(&IPC_Thread_Sync.cond, &IPC_Thread_Sync.mutex);
		}
		pthread_mutex_unlock(&IPC_Thread_Sync.mutex);
		/*
		send [0] = '1';
		send [1] = '1';
		Input_sock.send(&send[0],1);
		memset(&Receive, 0, 32);
		Input_sock.recv(&Receive, 32);
		printf("Received: %s", Receive);
		// find a word in the string related to one of the 5 commands available
		send[1] = checkSCommand(&Received);
		Output_sock.send(&send[1],1);*/
		printf("Voice Command\n");
		
	}
	pthread_exit(NULL);
}









   
CVoiceCommand::CVoiceCommand(void){
	memset(&Voice_Command, 0, 32);;
	Command_ID = 0;		
	//Input_sock(SERVER, PORT);
	//Output_sock(SERVER2, PORT2);
}
CVoiceCommand::~CVoiceCommand(void){
}


int  CVoiceCommand::createThread(void){
	int AHthread = pthread_create(&VoiceCommandThreadID, NULL, VoiceCommandHandler, NULL);
       	if (AHthread){
         	printf("Error creating Voice Command Thread\n");
         	exit(-1);
	}
}


