#ifndef __KALMAN_FILTER_H_
#define __KALMAN_FILTER_H_

 // Kalman filter variables 
    
	 
	void Kalman_init(void);
	double getAngle(double newAngle, double newRate, double dt);
	void setRmeasure(double newR_measure);
	void setQangle(double newQ_angle) ;
	void setQbias(double newQ_bias);
	void setAngle(double newAngle); 
	double getRmeasure(void);
	double getQangle(void);
	double getQbias(void);
	double getRate(void); 
	
#endif
