#include "GPIO.h"
#include <iostream>
#include <queue>
#include <stdlib.h>
#include <time.h>
#include <unistd.h> 
#include "My_Var_Types.h"

extern sem_t semaphore;
char Button;
extern CButton Controller3WCR;
extern IPC IPC_Thread_Sync;
extern sem_t Record_semaphore;

void ISR_ButtonOnOff(void){
	Button = '1';
	sem_post(&semaphore);

}
void ISR_ButtonRecord(void){
	if(Controller3WCR.getINPUTMODE()){	
		Button = '2';
		sem_post(&semaphore);
	}	
//usleep(1000);		
}

void ISR_ButtonVoiceAcc(void){
	Button = '3';
//	usleep(1000);	
	sem_post(&semaphore);
}

//+******************************************* Functions ***************************************************

 

void CButton::changeInputMode(void){
	if(m_INPUTMODE){
		digitalWrite (ACCELEROMETER_IN_LED, HIGH);
		digitalWrite (VOICE_COMMAND_IN_LED, LOW);
		m_INPUTMODE = false;
		printf (" Input mode: Acelerometer\n");
	}
	else{
		digitalWrite (ACCELEROMETER_IN_LED, LOW);
		digitalWrite (VOICE_COMMAND_IN_LED, HIGH);
		m_INPUTMODE = true;
		printf (" Input mode: Voice Command\n");
	}

usleep(1000);

}

CButton::CButton(void){
	m_INPUTMODE = false;
	m_ONOFF     = true;	
}

CButton::~CButton(void){

}




void CButton::sendStartOrStopToRobot(void){
	if(m_ONOFF){
		m_INPUTMODE = false;	
		digitalWrite (ONOFF_LED, LOW);
		printf("Stop Sent\n");
		//write on the queue to sendwifi
	}
	else{
		m_INPUTMODE = true;	
		digitalWrite (ONOFF_LED, HIGH);
		printf("Start Sent\n");
		//write on the queue to sendwifi	
	}
}


void* CButton::Button_HandlerThread(void* args){
	Button == '0';
	while(1){
		sem_wait(&semaphore);
       		
		//printf("Button: %c pressed\n", Button);
		//Button = '0';
			switch (Button){
				case '1':{
					printf("Recieved 1\n");
					Controller3WCR.sendStartOrStopToRobot();        
					Button = '0';
					break;
				}
				case '2':{
					printf("Recieved 2\n");
					sem_post(&Record_semaphore);
					Button = '0';
					break;
				}
				case '3':{ 
					printf("Recieved 3\n");
					Button = '0';	
					pthread_mutex_lock(&IPC_Thread_Sync.mutex);
					//change the Input mode
					Controller3WCR.changeInputMode();
					if(Controller3WCR.getINPUTMODE()){
						// signal voice Command handler thread 
						IPC_Thread_Sync.value = 2;
						pthread_cond_signal(&IPC_Thread_Sync.cond);
					}
					else{
						// Signal Accelerometer Handler Thread
						IPC_Thread_Sync.value = 1;
						pthread_cond_signal(&IPC_Thread_Sync.cond);
					}
					pthread_mutex_unlock(&IPC_Thread_Sync.mutex);
					break;
				}	
				default: Button = '0';
			}
		
	}
	pthread_exit(NULL);
}



/********************************************************************/

int CButton::createThread(void){
	int AHthread = pthread_create(&ButtonHandlerID, NULL, Button_HandlerThread, NULL);
       	if (AHthread){
         	printf("Error creating Button Thread\n");
         	exit(-1);
	}
}

bool CButton::getINPUTMODE(void){
	return m_INPUTMODE;
}
bool CButton::getONOFF(void){
	return m_ONOFF;
}

int CButton::init(void){
	// sets up the wiringPi library
  	if (wiringPiSetup() < 0) {
      		printf ("Unable to setup wiringPi \n");
      		return -1;
  	}
			
  	// set Pin 17/0 generate an interrupt falling
  	// and attach ISR_ButtonVoiceAcc() to the interrupt
  	if ( wiringPiISR (BUTTON_VOACC, INT_EDGE_FALLING, &ISR_ButtonVoiceAcc) < 0 ) {
     		printf ("Unable to setup ISR\n");
      		return -1;
  	}
	 
  	// set Pin 22/0 generate an interrupt 
  	// and attach ISR_ButtonRecord() to the interrupt
  	if ( wiringPiISR (BUTTON_RECORD, INT_EDGE_BOTH, &ISR_ButtonRecord) < 0 ) {
      		printf ("Unable to setup ISR\n");
      		return -1;
  	}
	// and attach ISR_ButtonOnOff() to the interrupt
  	if ( wiringPiISR (BUTTON_ONOFF, INT_EDGE_FALLING, &ISR_ButtonOnOff) < 0 ) {
     		printf ("Unable to setup ISR\n");
      		return -1;
  	}
	return 0;
}


 


