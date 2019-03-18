#ifndef __COMPLEMENTARY_FILTER_H_
#define __COMPLEMENTARY_FILTER_H_


#include "stm32f4xx.h"
#include <math.h>
#include <stdlib.h>
#include "Kalman.h"
#define RAD_TO_DEG 57.2957795
#define ACCELEROMETER_SENSITIVITY 8192.0
#define GYROSCOPE_SENSITIVITY  131
#define PI 3.14159265359	    
 
#define dt 0.011							// 11 ms sample rate!    
 

float ComplementaryFilter(short accData[3], short gyrData[3], float *Angle_Average, float *Gyro,float *Kalman_Angle2);

#endif
