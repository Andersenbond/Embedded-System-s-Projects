#ifndef AUTO_TUNNING_
#define AUTO_TUNNING_
#include <stdio.h>
#include <stdlib.h>

#define bool int
#define true 1
#define false 0

typedef struct {
	bool isMax, isMin;
	float *input, *output;
	float setpoint;
	float noiseBand;
	int controlType;
	bool running;
	unsigned long peak1, peak2, lastTime;
	int sampleTime;
	int nLookBack;
	int peakType;
	float lastInputs[101];
  float peaks[10];
	int peakCount;
	bool justchanged;
	bool justevaled;
	float absMax, absMin;
	float oStep;
	float outputStart;
	float Ku, Pu; 
	
}Auto_Tunning;	

extern long int now;

void PID_ATune(float* Input, float* Output, Auto_Tunning *AT);
void PID_ATune_FinishUp(Auto_Tunning *AT);
void PID_ATune_Cancel(Auto_Tunning *AT);
int PID_ATune_Run(Auto_Tunning *AT);
double PID_ATune_GetKp(Auto_Tunning *AT);
double PID_ATune_GetKi(Auto_Tunning *AT);
double PID_ATune_GetKd(Auto_Tunning *AT);


#endif
