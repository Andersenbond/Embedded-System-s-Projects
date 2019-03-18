#include "Auto_Tunning.h"
 
 
void PID_ATune_FinishUp(Auto_Tunning *AT){
	    *AT->output = AT->outputStart;
      //we can generate tuning parameters!
      AT->Ku = 4*(2*AT->oStep)/((AT->absMax-AT->absMin)*3.14159);
      AT->Pu = (float)(AT->peak1-AT->peak2) / 1000;
}


void PID_ATune(float* Input, float* Output, Auto_Tunning *AT){
	
	AT->input = Input;
	AT->output = Output;
	AT->controlType = 1 ; //0 to PI, 1 to PID
	AT->noiseBand = 0.5;
	AT->running = false;
	AT->oStep = 10;
	AT->lastTime = now;
	//AT.peaks[0] = 0; AT.peaks[1] = 0; AT.peaks[2] = 0; AT.peaks[3] = 0; AT.peaks[4] = 0;  
	//AT-peaks[5] = 0; AT.peaks[8] = 0; AT.peaks[6] = 0; AT.peaks[7] = 0; AT.peaks[9] = 0; 
	
}

void PID_ATune_Cancel(Auto_Tunning *AT){
	AT->running = false;
} 
 
int PID_ATune_Run(Auto_Tunning *AT ){
	
	AT->justevaled=false;
	if(AT->peakCount >9 && AT->running){
		AT->running = false;
		PID_ATune_FinishUp(AT);
		return 1;
	}
	
	if((now-AT->lastTime)<11) return false;
	AT->lastTime = now;
	double refVal = *AT->input;
	AT->justevaled=true;
	if(!AT->running){
		//initialize working variables the first time around
		AT->peakType = 0;
		AT->peakCount=0;
		AT->justchanged=false;
		AT->absMax=refVal;
		AT->absMin=refVal;
		AT->setpoint = refVal;
		AT->running = true;
		AT->outputStart = *AT->output;
		*AT->output = AT->outputStart+AT->oStep;
	}
	else{
		if(refVal>AT->absMax)AT->absMax=refVal;
		if(refVal<AT->absMin)AT->absMin=refVal;
	}
	
	//oscillate the output base on the input's relation to the setpoint
	
	     if (refVal>AT->setpoint+AT->noiseBand) *AT->output = AT->outputStart-AT->oStep;
	else if (refVal<AT->setpoint-AT->noiseBand) *AT->output = AT->outputStart+AT->oStep;
	
	
	//bool isMax=true, isMin=true;
	AT->isMax=true;AT->isMin=true;
	//id peaks
	for(int i=AT->nLookBack-1;i>=0;i--){
		float val = AT->lastInputs[i];
		if(AT->isMax) AT->isMax = refVal>val;
		if(AT->isMin) AT->isMin = refVal<val;
		AT->lastInputs[i+1] = AT->lastInputs[i];
	}
	AT->lastInputs[0] = refVal;  
	if(AT->nLookBack<9){
		//we don't want to trust the maxes or mins until the inputs array has been filled
		return 0;
	}
  
	if(AT->isMax){
		if(AT->peakType==0)AT->peakType=1;
		if(AT->peakType==-1){
			AT->peakType = 1;
			AT->justchanged=true;
			AT->peak2 = AT->peak1;
		}
		AT->peak1 = now;
		AT->peaks[AT->peakCount] = refVal;
	}
	else if(AT->isMin){
		if(AT->peakType==0)AT->peakType=-1;
		if(AT->peakType==1){
			AT->peakType=-1;
			AT->peakCount++;
			AT->justchanged=true;
		}
		if(AT->peakCount<10)AT->peaks[AT->peakCount] = refVal;
	}
  
	if(AT->justchanged && AT->peakCount>2){ 
		//we've transitioned.  check if we can autotune based on the last peaks
		float avgSeparation = (fabs(AT->peaks[AT->peakCount-1]-AT->peaks[AT->peakCount-2])+ fabs(AT->peaks[AT->peakCount-2]-AT->peaks[AT->peakCount-3]))/2;
		if( avgSeparation < 0.05*(AT->absMax-AT->absMin)){
			PID_ATune_FinishUp(AT);
			AT->running = false;
			return 1; 
		}
	}
	AT->justchanged=false;
	return 0;
}

double PID_ATune_GetKp(Auto_Tunning *AT){
	return AT->controlType==1 ? 0.6 * AT->Ku : 0.4 * AT->Ku;
}

double PID_ATune_GetKi(Auto_Tunning *AT){
	return AT->controlType==1? 1.2*AT->Ku / AT->Pu : 0.48 * AT->Ku / AT->Pu;  // Ki = Kc/Ti
}

double PID_ATune_GetKd(Auto_Tunning *AT){
	return AT->controlType==1? 0.075 * AT->Ku * AT->Pu : 0;  //Kd = Kc * Td
}
