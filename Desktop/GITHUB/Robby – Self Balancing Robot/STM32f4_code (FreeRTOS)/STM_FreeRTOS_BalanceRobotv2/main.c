#include "main.h"
/*********************************************************************************************************
**------------------------------------------------------------------------------------------------------**
**------------------------------------ Global Variables ------------------------------------------------**
**------------------------------------------------------------------------------------------------------**
**********************************************************************************************************/

//Used By both of the PID Tasks to exchange Data (Distance PID output and Encoders Differential) 
Distance_PID DistancePID;

//Used to store the encoders transition By the ISR (this Variable is also used by the Distance PID
// task and in order to Sincronize the tasks and the ISR's all interruptions are turn off when the 
// distance PID is reading this Variable) 
Encoder s_BalancingRobotEncoder;

//This semaphore is used to Sincronize the access to the DistancePID Struct (between both PID Tasks)
SemaphoreHandle_t Output_DistancePID_mutex;

//This semaphore is used in case we choose to use the ISR to sigal the encoder tasks to Increment the 
//Encoders transitions
SemaphoreHandle_t BalancingRobotEncoder_mutex;

//this Semaphore is used by the ISR for the TIMER2 to signal the Acquisition task when to do read the 
//sensors (sampling periond 10ms )
SemaphoreHandle_t Timer_ISR_Semaphore = NULL;

//this semaphore is used by the ISR for the TIMER5 to signal de Distance PID task to calculate the
//output for the distance PID and the Differential in the encoders 
SemaphoreHandle_t Timer_ISR_Encoders_Semaphore = NULL;

//this message queue is used to send the Angle and the Gyroscope Rate from the Acquisition Task to the 
//Balance PID Task
QueueHandle_t Angle_queue; 

//this message queue is used to send the data received via Bluetooth to the Receive_USART Function 
QueueHandle_t Receive_USART_queue; 

//in case we are using a task to update the encoder transition we used this queue in the ISR for the 
//external interrupt
QueueHandle_t Encoder_queue; 

//To send the output of the Balancing PID (PWM) to the motor controller task we use this queue
QueueHandle_t PWM_queue; 

//We use this variable to make sure the ISR doesnt give the semaphore before the semaphore is created
// normally it doesnt happens but we still didnt uderstand why when we initialize the timer the execution 
//flow goes to the ISR
int  f_Read = 0, f_Read2 = 0;
//This variable is defined int the TM MPU6050 library, and is used to store the value of the accelerometer
//and gyroscope Axis, the variable is used first in the initialization of the MPU sensor and after the 
//initialization is only used in the Acquisition task, so thats why we dont need to sincronize the access
TM_MPU6050_t MPU6050_Data0;

// this variable is used to Store the handle and the name of all the tasks created, this struct will be 
//used in the cleanup to delete all the tasks
Tasks_List BalancingRobotTaskList;

// this Variable has the Gains Kp, Ki and Kd for the Balance PID 
PID_Struct s_BalancePID;
//Since we can update the PID gains we are going to use this mutex to protect the access to the s_Balance_PID
// struct
SemaphoreHandle_t s_BalancePID_mutex;

// This Variable has the holds the Gains for the Distance PID
PID_Struct s_DistancePID;

//Similar as the s_BalancePID this variable can be updated so the needs to use a mutex to protect the access to 
//the s_DistancePID
SemaphoreHandle_t s_DistancePID_mutex;

//Kalman Struct 
KAl_Individual Struct_KalmanIndividualACC;	
KAl_Individual Struct_KalmanIndividualGyro;
KAl_Total Struct_KalmanTotal;
 								
float speed_need;
float turn_need;
long int now;
int time_ = 0;
float Kalman_Angle = 0;	float Kalman_Angle2 = 0;	
float Complimentary_Angle = 0;	
float OUTPUT_CONTROLLER = 0;	
float matrixPACC  = 0,matrixPGyro = 0;
 
Auto_Tunning AT;
int Stop_Flag = 0;


/*********************************************************************************************************
**------------------------------------------------------------------------------------------------------**
**----------------------------------------- ISRs -------------------------------------------------------**
**------------------------------------------------------------------------------------------------------**
This part of the code we have the Interrupt Service Routines for:								  											**
 + Timer2 - For the Acquisition Tick																																		**
 + Timer5 - Tick to the Distance PID task 																															**
 + External Interrupt 3 - For the Encoder Right (count Transitions)																			**	
 + External Interrupt 0 - For the Encoder Left (count Transitions)																			**
**********************************************************************************************************/


/**
  * @brief  Used to count the transitions for the Right Encoder connected to the pins GPIOA (Encoder A -> PA2
	* 				Encoder B -> PA3)
  * @param  None
  * @retval None
  */

void EXTI3_IRQHandler(void){
		if((EXTI_GetITStatus(EXTI_Line3) != RESET)){
				EXTI_ClearITPendingBit(EXTI_Line3);
				
				if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3) == 1 && GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2) == 1){ // 11
					  s_BalancingRobotEncoder.ENCODER_RIGHT_NEGATIVE++;
				}
				else if((GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3) == 0) && GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2) ==1){ // 10
					  s_BalancingRobotEncoder.ENCODER_RIGHT_POSITIVE++;

				}
				else if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2) == 1 && (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3) == 0)){ //01
					  s_BalancingRobotEncoder.ENCODER_RIGHT_POSITIVE++;
				}
				else if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2) == 0 && (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3) == 0)){
						s_BalancingRobotEncoder.ENCODER_RIGHT_NEGATIVE++;

				}
		}	
}

/**
  * @brief  Used to count the transitions for the Left Encoder connected to the pins GPIOA (Encoder A -> PA0
	* 				Encoder B -> PA1)
  * @param  None
  * @retval None
  */
void EXTI0_IRQHandler(void){
		if(EXTI_GetITStatus(EXTI_Line0) != RESET){
				EXTI_ClearITPendingBit(EXTI_Line0);
				
				if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0 == 1) && GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) == 1){ // 11
						 s_BalancingRobotEncoder.ENCODER_LEFT_POSITIVE++;
				}
				else if((GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0) && GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) == 1){ // 10
					  	s_BalancingRobotEncoder.ENCODER_LEFT_NEGATIVE++;

				}
				else if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0 == 1) && (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) == 0)){ //01
					    s_BalancingRobotEncoder.ENCODER_LEFT_NEGATIVE++;
				}
				else if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0 == 0) && (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) == 0)){  ///////
						  s_BalancingRobotEncoder.ENCODER_LEFT_POSITIVE++;

				}
		}
}

/**
  * @brief  The Acquisition period is set by the timer 2, this ISR sends a signal to the Acquisition task 
* 				every 10ms, the signal is sent using the Timer_ISR_Semaphore
  * @param  None
  * @retval None
  */
void TIM2_IRQHandler(void){
		if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET){
				TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
				if(f_Read == 1){
						f_Read = 0;
						now++;
						now = (now%1000);
						BaseType_t xHigherPriorityTaskWoken;
						xHigherPriorityTaskWoken = pdFALSE;
						
						xSemaphoreGiveFromISR( Timer_ISR_Semaphore, &xHigherPriorityTaskWoken );//Unblock the task by releasing the semaphore.
						if (xHigherPriorityTaskWoken == pdTRUE) // if true means that the task unblocked has higher priority than the one running
								portYIELD_FROM_ISR( xHigherPriorityTaskWoken ); // call the scheduler for the context switching 
			 }  
	  }
}
 

/**
  * @brief  The Acquisition period is set by the timer 5, this ISR sends a signal to the Distance PID task 
* 				 the signal is sent using the Timer_Encoders_Semaphore
  * @param  None
  * @retval None
  */
 void TIM5_IRQHandler(void){
			if ((TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)){
						TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
						BaseType_t xHigherPriorityTaskWoken;
						xHigherPriorityTaskWoken = pdFALSE;
						xSemaphoreGiveFromISR( Timer_ISR_Encoders_Semaphore, &xHigherPriorityTaskWoken );//Unblock the task by releasing the semaphore.
						if (xHigherPriorityTaskWoken == pdTRUE)
									portYIELD_FROM_ISR( xHigherPriorityTaskWoken );

		
  }
}
 
void USART1_IRQHandler(void){
	static portBASE_TYPE xHigherPriorityTaskWoken;
	static char received_string[MAX_R_STRLEN];
	
	 xHigherPriorityTaskWoken = pdFALSE;	
	if( USART_GetITStatus(USART1, USART_IT_RXNE) ){
		
		static uint8_t cnt = 0; // this counter is used to determine the string length
		char c = USART1->DR; // the character from the USART1 data register is saved in t
		
		/* check if the received character is not the LF character (used to determine end of string) 
		 * or the if the maximum string length has been been reached 
		 */
		if( (c != '\n') && (cnt < MAX_R_STRLEN) ){ 
			received_string[cnt] = c;
			cnt++;
		}
		else{ // otherwise reset the character counter and print the received string
			cnt = 0;
			xQueueSendToBackFromISR(Receive_USART_queue,(void*)&received_string,&xHigherPriorityTaskWoken);
		  if (xHigherPriorityTaskWoken == pdTRUE)
			portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
		}
	}
}
	
	 

/*********************************************************************************************************
**------------------------------------------------------------------------------------------------------**
**----------------------------------------- Functions --------------------------------------------------**
																																																				**
In this section we have the Functions:           																												**
	+ FReeRtos_Init -> this function create all the semaphores, queue and mutex														**
	+ MPU_init -> Initialize the MPU6050 sensor calling the function TM_MPU6050_Init from the 						**
		TM_MPU6050 library. the sensor is initialized with the following configuration:											**
				- Device Address 0xD0																																						**
				- Gyroscope Scale range 250 º/sec																																**
				- Accelerometer scale range 2G																																	**
				- All axis enable																																								**
	+ MPU_LED_Init -> used to configure the LED used to debugging purpose (show the user if the 					**
		sensor is connect led off, led on Sensor not connected)																							**
**------------------------------------------------------------------------------------------------------**
**********************************************************************************************************/

/**
  * @brief  Create the Semaphore, queue and Mutex 
  * @param  None
  * @retval None
  */
void FreeRTOS_Init(void){
		//Semaphore
		Timer_ISR_Semaphore 				 = xSemaphoreCreateBinary();
		Timer_ISR_Encoders_Semaphore = xSemaphoreCreateBinary();
		
		//Mutex
	  Output_DistancePID_mutex		 = xSemaphoreCreateMutex();
	  BalancingRobotEncoder_mutex	 = xSemaphoreCreateMutex();		
	  s_DistancePID_mutex  				 = xSemaphoreCreateMutex();
		s_BalancePID_mutex  				 = xSemaphoreCreateMutex();
	
	  //Message queue
		Receive_USART_queue = xQueueCreate( 1, sizeof( char[MAX_R_STRLEN]));
		Angle_queue   			= xQueueCreate( 1, sizeof( MPU_Data ));
	  Encoder_queue 			= xQueueCreate( 5, sizeof( char ));
	  PWM_queue		  			= xQueueCreate( 5, sizeof(PWM));
}

/**
  * @brief  Initialize the MPU sensor 
  * @param  None
  * @retval None
  */
 int MPU_Init(void){
		GPIO_ToggleBits(GPIOD, GPIO_Pin_13);
		/* Initialize MPU6050 sensor 0, address = 0xD0, AD0 pin on sensor is low */
    if (TM_MPU6050_Init(&MPU6050_Data0, TM_MPU6050_Device_0, TM_MPU6050_Accelerometer_2G, TM_MPU6050_Gyroscope_250s) == TM_MPU6050_Result_Ok) {
				GPIO_ToggleBits(GPIOD, GPIO_Pin_13);	
				return 0;
		}	 
				return 1;
}
 
 /**
  * @brief  Configure the Debugging LED
  * @param  None
  * @retval None
  */
 void MPU_LED_Init(void){
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);	
    GPIO_InitTypeDef  gpio_struct;
		gpio_struct.GPIO_Pin = GPIO_Pin_13;
		gpio_struct.GPIO_Mode = GPIO_Mode_OUT;
		gpio_struct.GPIO_Speed = GPIO_Speed_2MHz;
		gpio_struct.GPIO_PuPd = GPIO_PuPd_NOPULL;
		gpio_struct.GPIO_OType = GPIO_OType_PP;
		GPIO_Init(GPIOD, &gpio_struct);
}
 
void Insert_List(  char* Name,TaskHandle_t* Task_Handle){
BalancingRobotTaskList.Task_Handle[BalancingRobotTaskList.i] = Task_Handle;
BalancingRobotTaskList.Task_Name[BalancingRobotTaskList.i++] = Name;
 
}

void Create_Task(TaskFunction_t pxTaskCode,const char * const pcName,const uint16_t usStackDepth,void * const pvParameters,UBaseType_t uxPriority,TaskHandle_t * const pxCreatedTask){
xTaskCreate(pxTaskCode, pcName,usStackDepth, pvParameters,uxPriority,pxCreatedTask );
Insert_List((char*)pcName,pxCreatedTask);
}

  int Index = 0, direction = 0, next = 0, init = 0;
	float Gains[3];
void PID_Tunning(float Error, float Last_Error){
		
		if(init == 0){
				init = 1;
				Gains[0] = 80.0;
				Gains[1] = 0.0;
				Gains[2] = 7.0;
		}
		if( Error < 0) 
				Error = -Error;
		if( Last_Error < 0) 
				Last_Error = -Last_Error;
		if((Error > Last_Error ) || next == 0){
				direction++;
				next = 0;
				switch(((Index%3))){
						case 0: {
									if((direction%2) == 1){
											Gains[0] += 1;
											s_BalancePID.Kp = Gains[0];
									}
									else{
											Gains[0] -= 1;
											s_BalancePID.Kp = Gains[0];
									}							
						}; break ;
						
						case 1: {
									if((direction%2) == 1){
											Gains[1] += 0.1;
												s_BalancePID.Kd = Gains[1];
									}
									else{
											Gains[1] -= 0.1;
											s_BalancePID.Kd = Gains[1];
									}
						}; break ;
						
						case 2: {
									if((direction%2) == 1){
											Gains[2] += 0.01;
											s_BalancePID.Ki = Gains[2];
									}
									else{
											Gains[2] -= 0.01;
											s_BalancePID.Ki = Gains[2];
									}
						}; break ;
						
						default: break;
			 }
			
		}
		else if(Error < Last_Error){
			next = 1;
			Index++;
		}

}



/*********************************************************************************************************
**------------------------------------------------------------------------------------------------------**
**--------------------------------------------Tasks Behavior--------------------------------------------**
																																																				**
In this section we have the Tasks:																																			**
   + PID_Distance_Function -> this task is responsable to calculate the the output of the Distance PID  **
		  according to the encoders value, the task use two global variables (DistancePID and 					  	**
		  s_BalancingRobotEncoder) 																																					**
	 + Motor_Controller -> this task is used receives the PWM output for the two motors from the Balance  **
		  PID task and configures the Motor Driver Input pins according to the Values received on the queue **
	 + PID_Controller_Function -> this is the task for the Balance PID, the task receives the Angle value ** 
			and the Gyroscope Rate from the acquisition task and calculate the PWM for the motors using the   **
			PID mathematical Equation, the output of the PID (motor PWM) is sent to the Motor Controller task ** 
			using message queue																																								**
	 + Acquisition_Function -> this task receives a signal from the timer2 ISR read the sensor            **
	    uses  the complementary filter function to calculate the Robot current angle and gyroscope rate   **
			and send thoses datas to the PID_Controller_Function Task using message queue											**
**------------------------------------------------------------------------------------------------------**
**********************************************************************************************************/
void sendPIDValues(){
	char send_string[MAX_S_STRLEN+1];
	float value;
	
	if( xSemaphoreTake( s_BalancePID_mutex, ( TickType_t ) 10 ) == pdTRUE ){
		value = s_BalancePID.Kp;
		xSemaphoreGive(s_BalancePID_mutex);
		snprintf(send_string, 10, "1%.2f-", value);
		USART_puts(USART1, send_string);
	}					
	if( xSemaphoreTake( s_BalancePID_mutex, ( TickType_t ) 10 ) == pdTRUE ){
		value = s_BalancePID.Kd;
		snprintf(send_string, 10, "2%.2f-", value);
	  USART_puts(USART1, send_string);
		xSemaphoreGive(s_BalancePID_mutex);
	}	
	if( xSemaphoreTake( s_BalancePID_mutex, ( TickType_t ) 10 ) == pdTRUE ){
		value = s_BalancePID.Ki;
		snprintf(send_string, 10, "3%.2f-", value);
		USART_puts(USART1, send_string);
		xSemaphoreGive(s_BalancePID_mutex);
	}	
	if( xSemaphoreTake( s_DistancePID_mutex, ( TickType_t ) 10 ) == pdTRUE ){
		value = s_DistancePID.Kp;
		snprintf(send_string, 10, "4%.2f-", value);
		USART_puts(USART1, send_string);
		xSemaphoreGive(s_DistancePID_mutex);
	}	
	if( xSemaphoreTake( s_DistancePID_mutex, ( TickType_t ) 10 ) == pdTRUE ){
		value = s_DistancePID.Kd;
		snprintf(send_string, 10, "5%.2f-", value);
		USART_puts(USART1, send_string);
		xSemaphoreGive(s_DistancePID_mutex);
	}	
	if( xSemaphoreTake( s_DistancePID_mutex, ( TickType_t ) 10 ) == pdTRUE ){
		value = s_DistancePID.Ki;
		snprintf(send_string, 10, "6%.2f\r\n", value);
		USART_puts(USART1, send_string);
		xSemaphoreGive(s_DistancePID_mutex);
	}	
}

void changePIDValues(char* command){
	FromUSART_ISR FromISR; 
	
	char *start = &command[1];
	FromISR.Gain = strtof(start, NULL);
	FromISR.Index = command[0];
	
	switch (FromISR.Index){
									case '1':{
												if( xSemaphoreTake( s_BalancePID_mutex, ( TickType_t ) 10 ) == pdTRUE ){
															s_BalancePID.Kp = FromISR.Gain;
															xSemaphoreGive(s_BalancePID_mutex);
												}					
									} break;
									case '2':{
												if( xSemaphoreTake( s_BalancePID_mutex, ( TickType_t ) 10 ) == pdTRUE ){
															s_BalancePID.Kd= FromISR.Gain;
															xSemaphoreGive(s_BalancePID_mutex);
												}	
									} break;
									
									case '3': {
												if( xSemaphoreTake( s_BalancePID_mutex, ( TickType_t ) 10 ) == pdTRUE ){
															s_BalancePID.Ki = FromISR.Gain;
															xSemaphoreGive(s_BalancePID_mutex);
												}	
									}break;
									case '4':{
									 	   if( xSemaphoreTake( s_DistancePID_mutex, ( TickType_t ) 10 ) == pdTRUE ){
															s_DistancePID.Kp = FromISR.Gain;
															xSemaphoreGive(s_DistancePID_mutex);
												}	
									} break;
									
									case '5':{
											if( xSemaphoreTake( s_DistancePID_mutex, ( TickType_t ) 10 ) == pdTRUE ){
															s_DistancePID.Kd = FromISR.Gain;
															xSemaphoreGive(s_DistancePID_mutex);
												}	
									} break;
									case '6':{
											if( xSemaphoreTake( s_DistancePID_mutex, ( TickType_t ) 10 ) == pdTRUE ){
															s_DistancePID.Ki = FromISR.Gain;
															xSemaphoreGive(s_DistancePID_mutex);
												}	} break;
									default: ;
								}
}
void speed(int index){
			 if (index == 0) speed_need = 0, turn_need = 0;
	     if (index == 1) speed_need =  150;
	else if (index == 2) speed_need = -150;
	else if (index == 3) turn_need =   100;
	else if (index == 4) turn_need =  -100;
	
}

void commandHandler(char* received_string){
	int i = 1;
	int count = 0;
	char command[8];
	
	if (received_string[0] == '0'){ //Received new PID values (format ex: 0155-266-30.25+; 0-received values; 1-pid parameter to change; 55-value)
		
		while(received_string[i] != '+'){ 
			if(received_string[i] != '-'){
				command[count] = received_string[i];
				count++;
			}
			else {
				command[count] = ' ';
				changePIDValues(command);
				count = 0;
			}
			i++;
		}
		sendPIDValues();
	}
	
	else if (received_string[0] == 'p'){ //App requested the PID values (sent after connection)
			sendPIDValues();
	}
	else
	
		switch (received_string[0]){
			case 's': speed(0); break;
			case 'u': speed(1); break;
			case 'd': speed(2); break;
			case 'r': speed(3); break;
			case 'l': speed(4); break;
			case 'o': {
										if(Stop_Flag == 0) Stop_Flag = 1;
										else Stop_Flag = 0;
								} break;
		}
	}


static void USART_Receive_Function(void* args){
			/** the char received in the Message queue can be the following:
								1 - Kp - Balance PID
								2 - Kd - Balance PID 
								3 - Ki - Balance PID 
								---------------------
								4 - Kp - Distance PID
								5 - Kd - Distance PID
								6 - Ki - Distance PID
			*/
			static char received_string[MAX_S_STRLEN];
	
			while(1){
					f_Read2 = 1;
					if( xQueueReceive(Receive_USART_queue,&(received_string),( TickType_t ) 10 )){
						/*if(character != '/n'){
							received_string[index] = character;
							index++;
						}
						else{
							index = 0;*/
							commandHandler(received_string);
						//}						
					}
			}
}

void PID_Distance_Function(void * args){
			//float Kp = 20.00, Ki = 4.00 ;
			s_DistancePID.Kp = 20.00, s_DistancePID.Ki = 4.00, s_DistancePID.Kd = 0.0; 
			float E_Right = 0.0, E_Left = 0.0;
			float Error = 0.00, Total_Distance = 0.00, Total_Distance_Dif = 0.0;
			int Imax = 100,Imin = -100;
	    char StopFlag = '1';
			while (1){
						if(xSemaphoreTake( Timer_ISR_Encoders_Semaphore, LONG_TIME ) == pdTRUE ){
									E_Right =  s_BalancingRobotEncoder.ENCODER_RIGHT_POSITIVE - s_BalancingRobotEncoder.ENCODER_RIGHT_NEGATIVE;
									E_Left  =  s_BalancingRobotEncoder.ENCODER_LEFT_POSITIVE  - s_BalancingRobotEncoder.ENCODER_LEFT_NEGATIVE;
									s_BalancingRobotEncoder.ENCODER_RIGHT_POSITIVE = 0; 
									s_BalancingRobotEncoder.ENCODER_RIGHT_NEGATIVE = 0;
									s_BalancingRobotEncoder.ENCODER_LEFT_NEGATIVE  = 0;
									s_BalancingRobotEncoder.ENCODER_LEFT_POSITIVE  = 0;
									// End critical state 
					
									Error = (E_Right + E_Left)*0.5;
									Total_Distance += Error;
									Total_Distance_Dif += (E_Right - E_Left);
									if (Total_Distance > Imax)
												Total_Distance = Imax;
									else if(Total_Distance < Imin) 
												Total_Distance = Imin;
									if( xSemaphoreTake( BalancingRobotEncoder_mutex, ( TickType_t ) 10 ) == pdTRUE ){
												DistancePID.Output = Error*s_DistancePID.Kp + Total_Distance*s_DistancePID.Ki;
												DistancePID.Diferential = (E_Right - E_Left);
									xSemaphoreGive(BalancingRobotEncoder_mutex);
									}
					}				
			}
}

void Motor_Controller(void *args){
	  PWM s_PWM;
		
		while(1){
				if( xQueueReceive( PWM_queue, &( s_PWM ),( TickType_t ) 10 )){
						if(s_PWM.R_Direction == 1){
								GPIO_SetBits  (GPIOE, GPIO_Pin_12);   
								GPIO_ResetBits(GPIOE, GPIO_Pin_11);  
						}
						else{
								GPIO_SetBits  (GPIOE, GPIO_Pin_11);   
								GPIO_ResetBits(GPIOE, GPIO_Pin_12);
						}
						if(s_PWM.L_Direction == 1){
								GPIO_SetBits  (GPIOE, GPIO_Pin_13);   
								GPIO_ResetBits(GPIOE, GPIO_Pin_14);
						}
						else{
								GPIO_SetBits  (GPIOE, GPIO_Pin_14);   
								GPIO_ResetBits(GPIOE, GPIO_Pin_13);
						}
						if (s_PWM.R_PWM > 255)
								   s_PWM.R_PWM = 255;
					  if (s_PWM.L_PWM > 255)
							     s_PWM.L_PWM = 255;
						UpdatePWMDutyCycle((s_PWM.R_PWM*8),1);
						UpdatePWMDutyCycle((s_PWM.L_PWM*8),2);
						//GPIO_ToggleBits(GPIOD,GPIO_Pin_10);

				}
		}
}
 
 
 //http://robotics.stackexchange.com/questions/167/what-are-good-strategies-for-tuning-pid-loops
void PID_Controller_Function(void * args){
	  PWM s_BalancingRobotPWM;
	  MPU_Data  MPU_DataPID;
		float Setpoint = 0, PWM_Input2 = 0, PWM_Right, PWM_Left,Error, Last_Error, RobotCurrentAngle = 0, Gyro_Rate; // we can use the Gyro angle on the Kd term 
		float IMAX = 500, IMIN = -500,P_in, D_in, I_in2;
		s_BalancePID.Kp = 80.0, s_BalancePID.Kd = 0.01, s_BalancePID.Ki = 4.5;
	  PID_ATune(&RobotCurrentAngle, &PWM_Input2, &AT);
	while (1){
				if( xQueueReceive( Angle_queue, &( MPU_DataPID ),( TickType_t ) 10 )){
						if( xSemaphoreTake( s_BalancePID_mutex, ( TickType_t ) 10 ) == pdTRUE ){
							if(Stop_Flag == 0){
									RobotCurrentAngle = MPU_DataPID.Accelerometer_Angle;
									Gyro_Rate = MPU_DataPID.Gyroscope_Rate;
									//PID Calculation 
								
									Error = Setpoint - RobotCurrentAngle;
									
									int a = 0;///PID_ATune_Run(&AT);
									if(a != 0) {
												s_BalancePID.Kp = PID_ATune_GetKp(&AT);
				  					    s_BalancePID.Kd = PID_ATune_GetKd(&AT);
    										s_BalancePID.Ki = PID_ATune_GetKi(&AT);
									}
									P_in = s_BalancePID.Kp * Error;
									D_in = s_BalancePID.Kd * Gyro_Rate; // (Error + Last_Error); 
									I_in2 += (s_BalancePID.Ki*Error);
									if (I_in2 > IMAX )      I_in2 = IMAX;
									else if (I_in2 < IMIN)  I_in2 = IMIN;
							
									xSemaphoreGive(s_BalancePID_mutex);
									PWM_Input2 = P_in + D_in + I_in2;
									PWM_Right = (PWM_Input2);
									PWM_Left  = (PWM_Input2);	
									Last_Error = Error;
						}	
						if( (xSemaphoreTake( BalancingRobotEncoder_mutex, ( TickType_t ) 10 ) == pdTRUE)){
									if(!speed_need)PWM_Input2 -= DistancePID.Output/300;						 
									OUTPUT_CONTROLLER = PWM_Input2*255/800;	
									PWM_Right = (PWM_Input2 - DistancePID.Diferential + turn_need + speed_need)*1.3;
									PWM_Left  = (PWM_Input2 + DistancePID.Diferential - turn_need + speed_need)*1;	
									xSemaphoreGive(BalancingRobotEncoder_mutex);
						}
					}
					else{
							PWM_Right = 0;
							PWM_Left  = 0;	
					
					}
						if( PWM_Right < 0){
									PWM_Right = - PWM_Right;
									s_BalancingRobotPWM.R_Direction = 0;	
									 
						}else{
									s_BalancingRobotPWM.R_Direction= 1;	
									 
						}
						if( PWM_Left < 0){
									s_BalancingRobotPWM.L_Direction = 0;
									PWM_Left = - PWM_Left;
							 			
						}else{
									s_BalancingRobotPWM.L_Direction = 1;
							 		
						}
						
						s_BalancingRobotPWM.R_PWM = ((int)(PWM_Right*1000))/1000 ;
						s_BalancingRobotPWM.L_PWM = ((int)(PWM_Left*1000))/1000  ;
						
						// send the PWM to the Motor controller task 
						if( xQueueSend( PWM_queue,( void * ) &s_BalancingRobotPWM,( TickType_t ) 10 ) != pdPASS );	
				}
		}
}


void Acquisition_Function(void * args){
		short vec[3], vec2[3];
		MPU_Data ToPID;	
	
	  Init_Kalman(&Struct_KalmanIndividualACC , 1);
	  Init_Kalman(&Struct_KalmanIndividualGyro, 0);
    Init_Kalman_Total(&Struct_KalmanTotal);

		while(1){	
				f_Read = 1;
				if(xSemaphoreTake( Timer_ISR_Semaphore, LONG_TIME ) == pdTRUE ){
								TM_MPU6050_ReadAll(&MPU6050_Data0); // 1.24ms // programa todo // 1.56ms
								vec[0] = MPU6050_Data0.Accelerometer_X;
								vec[1] = MPU6050_Data0.Accelerometer_Y;
								vec[2] = MPU6050_Data0.Accelerometer_Z;
								vec2[0]= MPU6050_Data0.Gyroscope_X;
								vec2[1]= MPU6050_Data0.Gyroscope_Y;
								vec2[2]= MPU6050_Data0.Gyroscope_Z; 
								ComplementaryFilter(vec,vec2, &ToPID.Accelerometer_Angle, &ToPID.Gyroscope_Rate,&Kalman_Angle2);
					GPIO_ToggleBits(GPIOD,GPIO_Pin_10);	
					Kalman_Angle  = CalculateAngle(vec,vec2,&Struct_KalmanTotal, &Struct_KalmanIndividualACC, &Struct_KalmanIndividualGyro);
					matrixPACC = Struct_KalmanTotal.P[0][0];
					matrixPGyro = Struct_KalmanTotal.P[1][1];
						Complimentary_Angle = 	ToPID.Accelerometer_Angle;
					ToPID.Accelerometer_Angle = Kalman_Angle;
 								if( xQueueSend( Angle_queue,( void * ) &ToPID,( TickType_t ) 10 ) != pdPASS ){
										//error
								}		
				}
		}							
}
 

void Test(void *args){
		static int i;
		while (1){
 			i++;
		}
}
  

void Update_Encoder(void* args){
	  char Auxiliar[10];
	  int i = 0,j;
		while (1){
				if( xQueueReceive( Encoder_queue, &( Auxiliar[i%10] ),( TickType_t ) 10 )){
						i++;
						if( xSemaphoreTake( BalancingRobotEncoder_mutex, ( TickType_t ) 10 ) == pdTRUE ){
									for (j = 0; j<(i%10); j++){
											switch(Auxiliar[j]){
													case '1': s_BalancingRobotEncoder.ENCODER_LEFT_NEGATIVE++; break;
													case '2': s_BalancingRobotEncoder.ENCODER_LEFT_POSITIVE++; break;
													case '3': s_BalancingRobotEncoder.ENCODER_RIGHT_NEGATIVE++;break;
													case '4': s_BalancingRobotEncoder.ENCODER_RIGHT_POSITIVE++;break;
													default : break;
											}
									}
									i = 0;
									xSemaphoreGive(BalancingRobotEncoder_mutex);
					  }
				}	
		}
}

//------------------------------------------------------------------------------------------------------
//----------------------------------------------Main----------------------------------------------------
//------------------------------------------------------------------------------------------------------



int main(void) {
		/**
	  * @brief (SystemInit)  Setup the microcontroller system
    *         Initialize the Embedded Flash Interface, the PLL and update the 
    *         SystemFrequency variable.
    */
    SystemInit();
		Initialize_Pin();
    Timer2_Init_AcquisitionTick();
		Encoder_Left_Configuration();
		Encoder_Right_Configuration();
    MPU_LED_Init();
    while(MPU_Init());
	  FreeRTOS_Init();
		TimerGPIO_Init();
		TIMER_Init();
		PWM_Init();
	  Kalman_init() ;
	
  	MotorController_GPIOInit();
    Timer5_Init_EncodersTick();
		init_USART1(9600);
		BalancingRobotTaskList.i = 0;
		now = 0;
		TaskHandle_t * pxCreatedTask;
    Create_Task((void*)PID_Controller_Function, "BalancingPIDTask",  500, ( void * ) 1, 29 , pxCreatedTask ); 	
		Create_Task((void*)PID_Distance_Function,   "DistancePIDTask",   128, ( void * ) 2, 10 , pxCreatedTask );     
		Create_Task((void*)Acquisition_Function,    "AcquisitionTask",   128, ( void * ) 3, 11 , pxCreatedTask );     
	  Create_Task((void*)Motor_Controller,        "MotorControlerTask",128, ( void * ) 4, 10 , pxCreatedTask );     
		Create_Task((void*)Update_Encoder,          "UpdateEncoderTask", 50,  ( void * ) 5, 2  , pxCreatedTask );     
		Create_Task((void*)USART_Receive_Function,  "USARTReceiveTask",  128, ( void * ) 6, 6  , pxCreatedTask ); 
		Create_Task((void*)Test,                    "TestTask",          8,   ( void * ) 7, 0  , pxCreatedTask ); 
   
   	NVIC_SetPriority(EXTI0_IRQn, 9);
		NVIC_SetPriority(EXTI3_IRQn, 9);
		NVIC_SetPriority(TIM2_IRQn,  6);
		NVIC_SetPriority(TIM5_IRQn,  5);
		NVIC_SetPriority(USART1_IRQn,6);
	  vTaskStartScheduler();

}

