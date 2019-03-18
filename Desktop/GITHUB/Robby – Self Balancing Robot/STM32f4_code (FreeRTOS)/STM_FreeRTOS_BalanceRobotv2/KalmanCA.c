#include "KalmanCA.h"	    
 
// acc i = 1; Gyro i != 1
void Init_Kalman(KAl_Individual *Temp, int i){
	Temp->A = 1;
	Temp->H = 1;
	if( i == 1){
			Temp->Q = 0.0001;
			Temp->R = 0.0299;
	}
	else {
			Temp->Q = 0.001;
			Temp->R = 0.1317;
	}
	Temp->X = 0;
	Temp->K = 0;
	Temp->P = 0;
	Temp->P_ = 0.2;
	Temp->X_ = 0;
	
}

 

void Init_Kalman_Total(KAl_Total *Temp){	
	Temp->A[0][0] = 1; Temp->A[0][1] = dt; Temp->A[1][0] = 0; Temp->A[1][1] = 1;
	Temp->H[0][0] = 1; Temp->H[0][1] = 0;  Temp->H[1][0] = 0; Temp->H[1][1] = 0;
	Temp->Q[0][0]= 0.001; Temp->Q[0][1]= 0; Temp->Q[1][0]= 0;Temp->Q[1][1]= 0.003;
	Temp->R[0][0]= 0.0299; Temp->R[0][1]= 0; Temp->R[1][0]= 0;Temp->R[1][1]= 0.1317;
	Temp->X[0][0]= 0; Temp->X[1][0]= 0;
	Temp->K[0][0] = 0 ; Temp->K[1][0] = 0; 
	Temp->P[0][1] = 0 ; Temp->P[0][0] = 0; Temp->P[1][0] = 0 ; Temp->P[1][1] = 0;
	Temp->P_[0][0]= 2.2 ; Temp->P_[0][1] = 0; Temp->P_[1][0] = 0 ; Temp->P_[1][1] = 2.2;
	
	Temp->X_[0][0]= 0; Temp->X_[1][0]= 0;
	
}

  
   
   
float Calcula_Kalman_Individual(float Data, KAl_Individual *Temp){
    
	//Correct 
	Temp->K = Temp->P_* Temp->H / (Temp->H*Temp->P_*Temp->H + Temp->R);
	Temp->X = Temp->X_ + Temp->K *(Data -Temp->H * Temp->X_); 
	Temp->P = (1-Temp->K*Temp->H)*Temp->P_;
	
	
	//Prediction
	Temp->X_ = Temp->A * Temp->X;
	Temp->P_ = Temp->A * Temp->P * Temp->A + Temp->Q;
	// Imprimir X, P , K;
	return Temp->X;
	
}


float Calcula_Kalman_Total(float GyroData, float AccData, KAl_Total *Temp){
   
	//Correct 
	float s;
	s = Temp->P_[0][0] + Temp->R[0][0];
	Temp->K[0][0] = Temp->P_[0][0] / s;
	Temp->K[1][0] = Temp->P_[1][0] / s;
	
	Temp->X[0][0] = Temp->X_[0][0] + Temp->K[0][0]*(AccData  - Temp->X_[0][0]);
	Temp->X[1][0] = Temp->X_[1][0] + Temp->K[1][0]*(AccData  - Temp->X_[0][0]);
	
	Temp->P[0][0] = Temp->P_[0][0] - Temp->K[0][0]*Temp->P_[0][0];
	Temp->P[0][1] = Temp->P_[0][1] - Temp->K[0][0]*Temp->P_[0][1];
	Temp->P[1][0] = Temp->P_[1][0] - Temp->K[1][0]*Temp->P_[0][0];
	Temp->P[1][1] = Temp->P_[1][1] - Temp->K[1][0]*Temp->P_[0][1];
	
	
	 //Prediction
	Temp->X_[1][0] = GyroData - Temp->X[1][0];
	Temp->X_[0][0] = Temp->X[0][0] + dt * Temp->X_[1][0];
	
	Temp->P_[0][0] = Temp->P[0][0] + dt*(dt*Temp->P[1][1] - Temp->P[0][1] - Temp->P[1][0] + Temp->Q[0][0]);
	Temp->P_[0][1] = Temp->P[0][1] - dt* Temp->P[1][1];
	Temp->P_[1][0] = Temp->P[1][0] - dt* Temp->P[1][1];
	Temp->P_[1][1] = Temp->P[1][1] + Temp->Q[1][1]*dt;
	
	
	
	
	
	
	// Imprimir X, P , K;
	return Temp->X[0][0];
	
}

float CalculateAngle(short AccDATA[3],short GyroDATA[3],KAl_Total *Total, KAl_Individual *Acc, KAl_Individual   *Gyro ){
			 float AnguloX = atan2((float)AccDATA[1], (float)AccDATA[2]) * 180 / PI;
			 float GyroRateX = GyroDATA[0] / GYROSCOPE_SENSITIVITY; // º/s datasheet pagina 12
		   float AnguloACC = Calcula_Kalman_Individual(AnguloX,Acc);
			 //return AnguloACC;
		   float GyroRate = Calcula_Kalman_Individual(GyroRateX,Gyro);		 
		   float RobotAngle = Calcula_Kalman_Total(GyroRate,AnguloACC, Total);
			 return RobotAngle;

}


/*main(void) {
	KAl_Individual Gyro, Acc;
	KAl_Total Total Temp;
	InitKalman(&Acc, 1);
	InitKalman(&Gyro, 0);
	float AnguloACC, GyroRate, AnguloRobot;
	InitKalmanTotal(&Temp);
	while(1){
		//Le sensor 
		// calcula angulo 
		 float AnguloX = atan2((float)accData[1], (float)accData[2]) * 180 / PI;
		 float GyroRateX = gyrData[0] / GYROSCOPE_SENSITIVITY; // º/s datasheet pagina 12
		 AnguloACC = Calcula_Kalman_Individual(AnguloX,&Acc);
		 GyroRate = Calcula_Kalman_Individual(GyroRateX,&Gyro);		 
		 AnguloRobot= Calcula_Kalman_Total(AnguloACC,GyroRate, &Temp);
		
	}*/
	
	
	
	
//}











