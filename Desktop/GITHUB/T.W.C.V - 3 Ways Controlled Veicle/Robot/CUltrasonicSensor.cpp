#include <wiringPi.h>
#include "CUltrasonicSensor.h"

using namespace std;

CUltrasonicSensor::CUltrasonicSensor (void){}

CUltrasonicSensor::~ CUltrasonicSensor (void){}

int CUltrasonicSensor::Get_Distance_cm(int ECHO_PIN, int TRIG_PIN)
{
	pinMode(TRIG_PIN, OUTPUT);
        pinMode(ECHO_PIN, INPUT);
        
	//TRIG pin must start LOW
        digitalWrite(TRIG_PIN, LOW);
        delay(30);

	//Send trig pulse
        digitalWrite(TRIG_PIN, HIGH);
        delayMicroseconds(20);
        digitalWrite(TRIG_PIN, LOW);
 
        //Wait for echo start
        while(digitalRead(ECHO_PIN) == LOW);
 
        //Wait for echo end
        long startTime = micros();
        while(digitalRead(ECHO_PIN) == HIGH);
        long travelTime = micros() - startTime;
 
        //Get distance in cm
        int distance = travelTime / 58;
 
        return distance;

}





