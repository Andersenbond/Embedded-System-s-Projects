#include "ADXL345.h"
#include "pthread.h"
#include "string.h"
#include "PracticalSocket.h"
#include "I2C.h"
#include <cmath>
#include "semaphore.h"

#include <sched.h>
#include <errno.h>

//extern CButton Controller3WCR;
extern IPC IPC_Thread_Sync; 

float RAD_TO_DEG = 57.29578;
CI2CCommunication I2C_1;
bool SendAngle = false;

sem_t TimerTick_semaphore;
//TCPSocket sock(SERVER, PORT);	












void SetupThread(int prio,pthread_attr_t *pthread_attr,struct sched_param *pthread_param)
{
	int rr_min_priority, rr_max_priority;

	pthread_attr_setschedpolicy (pthread_attr, SCHED_RR);
	rr_min_priority = sched_get_priority_min (SCHED_RR);
	rr_max_priority = sched_get_priority_max (SCHED_RR);
	pthread_param->sched_priority = prio;
	
	printf ("SCHED_RR priority range is %d to %d: using %d\n",rr_min_priority,rr_max_priority,pthread_param->sched_priority);

	pthread_attr_setschedparam (pthread_attr, pthread_param);
}



void CheckFail(int status)
{
	if(status)
	{
		if(status==EPERM)
			fprintf(stderr,"pthread_create() got EPERM\n");
		else if(status==EINVAL)
      			fprintf(stderr,"pthread_create() got EINVAL\n");
    		else
      			fprintf(stderr,"pthread_create() got neither EPERM nor EINVAL\n");

    		fprintf(stderr,"pthread_create() got error %d\n",status);
    		errno=status;
    		perror("pthread_create()");
    		exit(1);
  	}
}














//----------------------------------------------------- Signal---------------------------------------------------------------

 
static void TimerTick_Handler(int sig, siginfo_t *si, void *uc){	
	sem_post(&TimerTick_semaphore);
	//printf("Signal\n");			
}


 
int TimerTick_Configuration(long freqMili){
	 
	timer_t timerid;
       	struct sigevent sev;
	struct itimerspec its;
      	sigset_t mask;
       	struct sigaction sa;
	//signal
	sa.sa_flags = SA_SIGINFO;
        sa.sa_sigaction = TimerTick_Handler;
        sigemptyset(&sa.sa_mask);
        if (sigaction(SIG, &sa, NULL) == -1){
        	printf("Error Signation!\n");
		return -1;
	}

	//block the signal
	sigemptyset(&mask);
        sigaddset(&mask, SIG);
        if (sigprocmask(SIG_SETMASK, &mask, NULL) == -1){
        	printf("Error Blocking the timer signal!\n");
		return -1;
	}

	// Create timer using Real time clock
 	sev.sigev_notify = SIGEV_SIGNAL;
        sev.sigev_signo = SIG;
        sev.sigev_value.sival_ptr = &timerid;
        if (timer_create(CLOCKID, &sev, &timerid) == -1){
        	printf("Error Blocking the timer signal!\n");
		return -1;
	}

        its.it_value.tv_sec = freqMili / 1000;
        its.it_value.tv_nsec = (freqMili % 1000)*1000000 ;
        its.it_interval.tv_sec = its.it_value.tv_sec;
        its.it_interval.tv_nsec = its.it_value.tv_nsec;
	if (timer_settime(timerid, 0, &its, NULL) == -1){
                printf("Error Setting the timer!\n");
		return -1;
	}

	// Unblock timer signal 
	if (sigprocmask(SIG_UNBLOCK, &mask, NULL) == -1){
        	printf("Error Unblocking the timer signal!\n");
		return -1;
	}
 	return 0;
}


//----------------------------------------------------------------- Class  Accelerometer Implementation 





// Class Accelerometer Default Constructor 
C3WCR_Accelerometer::C3WCR_Accelerometer(void){
	AccelerometerX = 0;
	AccelerometerY = 0;
	AccelerometerZ = 0;
	devAddr = ADXL345Adress;
}

// Class Accelerometer destructor 
C3WCR_Accelerometer::~C3WCR_Accelerometer(void){

}

// Class Accelerometer Constructor 
// this contructer is used when the user wants to gives the address of the 
// accelerometer sensor.
C3WCR_Accelerometer::C3WCR_Accelerometer(uint8_t devAddr){
	AccelerometerX = 0;
	AccelerometerY = 0;
	AccelerometerZ = 0;
	this->devAddr = devAddr;
}

//this function is a member of the class Accelerometer and it is used to 
//get the value of the X axis of the accelerometer
uint16_t C3WCR_Accelerometer::getX(){
	return AccelerometerX; 
}

//this function is a member of the class Accelerometer and it is used to 
//get the value of the Y axis of the accelerometer
uint16_t C3WCR_Accelerometer::getY(){
	return AccelerometerY; 
}

//this function is a member of the class Accelerometer and it is used to 
//get the value of the Z axis of the accelerometer
uint16_t C3WCR_Accelerometer::getZ(){
	return AccelerometerZ; 
}

//this function is a member of the class Accelerometer and it is used to 
//get the id of the Accelerometer thread
int C3WCR_Accelerometer::getThreadID(void){
	return accelerometerHandlerID;
}


//this function is a member of the class Accelerometer and it is used to 
//init the accelerometer sensor.  the function acces uses the i2c communcation 
//to communicate with the accelerometer sensor
int C3WCR_Accelerometer::init(){
   	char buf[6];                              			// Buffer for data being read/ written on the i2c bus
   	I2C_1.init();
   	if (ioctl(I2C_1.getFiledescriptor(), I2C_SLAVE, ADXL345Adress) < 0){ 			// check the accelerometer sensor
		fprintf(stderr, "ADXL345 sensor not found2");       		
		return -2;
	} 
   	buf[0] = ADXL345_POWER_CTL;                                    	// auto sleep
   	buf[1] = 0x18;
  	if(I2C_1.writeAccelerometer(buf,2)!=0){	
		fprintf(stderr, "write 1");
		return -3;
   	} 
   	buf[0] = ADXL345_DATA_FORMAT;                               	// data format register
   	buf[1] = 0x08; 							//full resolution , A 2g
   	if(I2C_1.writeAccelerometer(buf,2)!=0){
		fprintf(stderr, "write 2");
       		return -4;
   	}
   	 
   return 0;
}


//Behavior of the Accelerometer sensor, the thread receives a signal from a timer and 
//reads the sensor and sends the value X and Y axis to the Robot system.
// the thread also tells the robot how to move according to the accelerometer position
// in order to control the Robot with the Accelerometer the user must choose the accelerometer
// as the input mode using the buttons 
void* C3WCR_Accelerometer::Accelerometer_HandlerThread(void * arg){
	int auxiliar = 0, angle1, angle2;
	short ax, ay, az;
	float AccXangle, AccYangle;
	char send[9];
	while (1){
		// Wait for a signal from the Timer
		sem_wait(&TimerTick_semaphore);
		
		//check the input mode
		pthread_mutex_lock(&IPC_Thread_Sync.mutex);
		if(IPC_Thread_Sync.value == 2){
			 pthread_cond_wait(&IPC_Thread_Sync.cond, &IPC_Thread_Sync.mutex);
		}
		pthread_mutex_unlock(&IPC_Thread_Sync.mutex);

		//printf("**ACC Start**\n");
		I2C_1.readAccelerometer(ax,ay,az);
		AccXangle = (float) (atan2(ay,az)+M_PI)*RAD_TO_DEG;
          	AccYangle = (float) (atan2(ax,az)+M_PI)*RAD_TO_DEG;
       		if (AccXangle >180)
                	AccXangle -= (float)360.0;
        	
        	if (AccYangle >180)
                	AccYangle -= (float)360.0;

		if(SendAngle){
			send [2] = ((ax>>12) & 0xF)  | 0x80;
			send [1] = ((ax>>6)  & 0x3F) | 0x80;
			send [0] = ((ax)     & 0x3F) | 0xC0;
			send [5] = ((ay>>12) & 0xF)  | 0x80;
			send [4] = ((ay>>6)  & 0x3F) | 0x80;
			send [3] = ((ay)     & 0x3F) | 0x80;
			send [8] = ((az>>12) & 0xF)  | 0x80;
			send [7] = ((az>>6)  & 0x3F) | 0x80;
			send [6] = ((az)     & 0x3F) | 0x80;
			for(int i = 0; i<9; i++){
				//sock.send(&send[i],1);
			
			}
		SendAngle = false;
		}
		GetMotion(AccXangle, AccYangle);
		printf("**ACC done**\n");
	}
	pthread_exit(NULL);
}


//function member of the accelerometer class used to create the thread
// the function also initialize some variables and peripherals used in 
// the accelerometer thread
int C3WCR_Accelerometer::createThread(void){
	int thread_policy;
	pthread_attr_t thread_attr;
	struct sched_param thread_param;

	pthread_attr_init (&thread_attr);
	pthread_attr_getschedpolicy (&thread_attr, &thread_policy);
	pthread_attr_getschedparam (&thread_attr, &thread_param);
	
	printf (
		"Default policy is %s, priority is %d\n",
		(thread_policy == SCHED_FIFO ? "FIFO"
		: (thread_policy == SCHED_RR ? "RR"
		: (thread_policy == SCHED_OTHER ? "OTHER"
		: "unknown"))),thread_param.sched_priority);

	SetupThread(60,&thread_attr,&thread_param);
	
	printf ("Creating thread at RR/%d\n",thread_param.sched_priority);
	pthread_attr_setinheritsched (&thread_attr, PTHREAD_EXPLICIT_SCHED);
	int status_2 = pthread_create (&accelerometerHandlerID, &thread_attr, Accelerometer_HandlerThread, NULL);

	 
	CheckFail(status_2);
	
	pthread_join (accelerometerHandlerID, NULL);

	/*int AHthread = pthread_create(&accelerometerHandlerID, NULL, Accelerometer_HandlerThread, NULL);
       	if (AHthread){
         	printf("Error creating Accelerometer Thread\n");
         	exit(-1);
	}*/
	TimerTick_Configuration(100);
	sem_init(&TimerTick_semaphore, 0, 0);
}

 
 
// this function is used by the accelerometer thread to calculate the direction of the movement and sends 
//it to the Robot system in a TCP IP socket  
int GetMotion(float X_Angle, float Y_Angle){	 
	static int last_Aux, auxiliar = -1;
	if (abs(X_Angle) < abs(Y_Angle)){

		if(Y_Angle > 0 && abs(Y_Angle) > 40) 
			auxiliar = 1;//forward
		else if(Y_Angle < 0 && abs(Y_Angle) > 40)
			auxiliar = 2; //backward
		else 
			auxiliar = 0;
	}
	else{
		if(X_Angle > 0 && abs(X_Angle) > 40) 
			auxiliar = 3;//right
		else if(X_Angle < 0 && abs(X_Angle) > 40)
			auxiliar = 4;//left
		else 
			auxiliar = 0;	
	}
	
// printf part 
	if(auxiliar != last_Aux){
		last_Aux = auxiliar; 
		switch(auxiliar){	
			case 1:printf("1\n"); break;//sock.send("1",1); break;
			case 2:printf("2\n"); break;//sock.send("2",1); break;
			case 3:printf("3\n"); break;//sock.send("3",1); break;
			case 4:printf("4\n"); break;//sock.send("4",1); break;	
			default:printf("0\n"); break;//sock.send("0",1);	
		}
	}	
}







