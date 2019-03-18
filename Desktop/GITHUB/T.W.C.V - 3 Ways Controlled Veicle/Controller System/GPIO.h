#ifndef _GPIO_H_
#define _GPIO_H_

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <pthread.h>

#include <mqueue.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <semaphore.h>

#include "My_Var_Types.h"

void ISR_ButtonOnOff(void);
void ISR_ButtonRecord(void);
void ISR_ButtonVoiceAcc(void);



class CButton{
     private: 
	bool m_INPUTMODE;
	bool m_ONOFF;
	pthread_t ButtonHandlerID;
	static void *Button_HandlerThread(void *args);

     public:
	CButton(void);
	~CButton(void);
	int init(void);
	bool getINPUTMODE(void);
	bool getONOFF(void);
	void changeInputMode(void);
	void sendStartOrStopToRobot(void);
	int  createThread(void);
};

extern CButton Controller3WCR;
extern sem_t semaphore;
extern IPC IPC_Thread_Sync;
extern sem_t Record_semaphore;

#define BUTTON_RECORD 0 
#define BUTTON_ONOFF  2 // Use GPIO Pin 17, which is Pin 0 for wiringPi library
#define BUTTON_VOACC  3 // Use GPIO Pin 22, which is Pin 3 for wiringPi library
#define RECORDING_LED 1
#define ACCELEROMETER_IN_LED 4
#define VOICE_COMMAND_IN_LED 5
#define ONOFF_LED 6
#define HIGH 1
#define LOW 0



#endif

