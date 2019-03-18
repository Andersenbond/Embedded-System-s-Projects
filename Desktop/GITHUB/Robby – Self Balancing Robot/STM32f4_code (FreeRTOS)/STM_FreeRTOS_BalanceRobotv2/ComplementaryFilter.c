#include "ComplementaryFilter.h"
#include "KalmanCA.h"

float ComplementaryFilter(short accData[3], short gyrData[3], float *Angle_Average, float *Gyro, float *Kalman_Angle2){
	 float Gyro_X = 0.00, X_Angle_1 = 0.00,Y_Angle_1  = 0.00,X_Angle_2 = 0.00, Y_Angle_2  = 0.00;
	 static float X_Angle_3 = 0.00;
	 static float Y_Angle_3 = 0.00;
	//Gyro rate
	 Gyro_X = gyrData[0] / GYROSCOPE_SENSITIVITY; // º/s datasheet pagina 12
	float Gyro_Y = gyrData[1] / GYROSCOPE_SENSITIVITY; // º/s datasheet pagina 12
	 
	//using only the ACC    
    X_Angle_1 = atan2((float)accData[1], (float)accData[2]) * 180 / PI;
    Y_Angle_1 = atan((float)-accData[0] / sqrt((float)accData[1]+(float)accData[2]*(float)accData[2])) * 180 / PI;
     
 
	
	double roll  = atan2(accData[1], accData[2]) * RAD_TO_DEG;
  double pitch = atan(-accData[0] / sqrt(accData[1] * accData[1] + accData[2] * accData[2])) * RAD_TO_DEG;

  double roll2  = atan(accData[1] / sqrt(accData[0] * accData[0] + accData[2] * accData[2])) * RAD_TO_DEG;
  double pitch2 = atan2(-accData[0], accData[2]) * RAD_TO_DEG;
	
	
	
	 //using a low pass filter on the acc
    X_Angle_2 = X_Angle_2*0.75 + 0.25*X_Angle_1;
    Y_Angle_2 = Y_Angle_2*0.75 + 0.25*X_Angle_1; 
	
	//complementary filter 
	float a = 0.98*(X_Angle_3 + Gyro_X * 0.011);
	float b = 0.02*(X_Angle_1);
	X_Angle_3 = a + b;
		
	float c = 0.98*(Y_Angle_3 + Gyro_Y * 0.011);
	float d = 0.02*(Y_Angle_1);
	Y_Angle_3 = c + d;
	
	
	//Kalman Filter
	float aux  = (float)getAngle(X_Angle_1,Gyro_X ,0.011);
	float aux2 = (float)getAngle(Y_Angle_1,Gyro_Y,0.011);
	 *Kalman_Angle2 = aux;
	 *Angle_Average =   X_Angle_3;//(2*aux );//+ X_Angle_3 + X_Angle_1)/3;
	//*Angle_Average = (aux2 + Y_Angle_3 + Y_Angle_1)/3;
	*Gyro = Gyro_X;
	
}







 