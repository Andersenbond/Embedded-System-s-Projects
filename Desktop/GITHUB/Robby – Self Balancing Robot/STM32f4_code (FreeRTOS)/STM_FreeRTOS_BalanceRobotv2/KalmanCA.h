#ifndef KALMAN_CA_H
#define KALMAN_CA_H

#include <stdio.h>
#include <math.h>
#define dt 0.011
#define RAD_TO_DEG 57.2957795
#define GYROSCOPE_SENSITIVITY  131
#define PI 3.14159265359


typedef struct{
int	A, H;
float Data, Q ,R ,X ,P,K ,P_, X_;	
} KAl_Individual;

typedef struct {
float A[2][2];
int   H[2][2];
float B[2][1], U; 
float Data[2][1], Q[2][2] ,R[2][2] ,X[2][1] ,P[2][2], K[2][1], X_[2][1], P_[2][2];	
} KAl_Total;

void Init_Kalman(KAl_Individual *Temp, int i);
void Init_Kalman_Total(KAl_Total *Temp);
float Calcula_Kalman_Individual(float Data, KAl_Individual *Temp);
float Calcula_Kalman_Total(float GyroData, float AccData, KAl_Total *Temp);
float CalculateAngle(short AccDATA[3],short GyroDATA[3],KAl_Total *Total, KAl_Individual *Acc, KAl_Individual   *Gyro );

#endif
