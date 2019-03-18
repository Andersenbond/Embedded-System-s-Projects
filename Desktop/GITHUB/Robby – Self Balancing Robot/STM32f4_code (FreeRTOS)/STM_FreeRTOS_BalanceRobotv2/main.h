#ifndef _MAIN_H_
#define  _MAIN_H_

#include "ComplementaryFilter.h"
#include "stm32f4xx.h"
#include "defines.h"
#include "tm_stm32f4_mpu6050.h"
#include <stdio.h>
#include <stm32f4xx_tim.h>
#include <misc.h>
#include "USART.h"
#include "FreeRTOS.h"
#include "Semphr.h"
#include "queue.h"
#include "task.h"
#include "math.h"
#include "Timer.h"
#include "Kalman.h"
#include "USART.h"
#include "ExternalInterrupt.h"
#include "Extra.h"
#include <string.h>
#include <time.h>
#include "Auto_Tunning.h"
#include "KalmanCA.h"

#define LONG_TIME 0xffff
#define TICKS_TO_WAIT    10




typedef struct{
	int ENCODER_RIGHT_POSITIVE;
	int ENCODER_RIGHT_NEGATIVE;
	int ENCODER_LEFT_NEGATIVE;
	int ENCODER_LEFT_POSITIVE;
}Encoder;


typedef struct{
	int R_PWM;
	int L_PWM;
	short R_Direction;
	short L_Direction;
}PWM;

typedef struct{
	float Accelerometer_Angle;
	float Gyroscope_Rate;
}MPU_Data; 

typedef struct{
	float Output;
	float Diferential;
}Distance_PID; 

typedef struct{
	float Output;
	int Index;
}Movement; 


typedef struct{
	float Kp;
	float Ki;
	float Kd;
}PID_Struct;


typedef struct{
	char Index;
	float Gain;

	}FromUSART_ISR;

typedef struct Node{
	     char *  Task_Name[10];
	     TaskHandle_t *Task_Handle[10];
			 int i;
}Tasks_List;

void Create_Task(TaskFunction_t pxTaskCode,const char * const pcName,const uint16_t usStackDepth,void * const pvParameters,UBaseType_t uxPriority,TaskHandle_t * const pxCreatedTask);
void Insert_List(  char* Name,TaskHandle_t* Task_Handle);

int MPU_Init(void);
void MPU_LED_Init(void);
void FreeRTOS_Init(void);

static void Test(void *args);
static void Update_Encoder(void* args);
static void Motor_Controller(void *args);
static void USART_Send_Function(void* args);
static void Acquisition_Function(void * args);
static void USART_Receive_Function(void* args);
static void PID_Distance_Function(void * args);
static void PID_Controller_Function(void * args);
 

#endif  
