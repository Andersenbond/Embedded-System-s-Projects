
#include "ADXL345.h"
#include "GPIO.h"
#include <semaphore.h>
#include "Voice_Command.h"


IPC IPC_Thread_Sync;
CButton Controller3WCR;
sem_t semaphore;
sem_t Record_semaphore;




int main(void){
	sem_init(&Record_semaphore, 0, 1);	
	sem_init(&semaphore, 0, 1);
	pthread_mutex_init(&IPC_Thread_Sync.mutex, NULL);
	pthread_cond_init (&IPC_Thread_Sync.cond, NULL);
	IPC_Thread_Sync.value = 0;
	
	
	C3WCR_Accelerometer ADXL345;
	CVoiceCommand LOGITEC;
   	if(ADXL345.init() != 0)  
		exit(1);
   	usleep(100000); // for accelerometer stabilize 
	ADXL345.createThread();
 	
	//buttons - GPIO.cpp	
	Controller3WCR.init();
	Controller3WCR.createThread();


	LOGITEC.createThread();
	
	pthread_exit(NULL);

   return 0;
}

