#include <wiringPi.h>
#include "CMotorDriver.h"

using namespace std;

CMotorDriver::CMotorDriver (void){}

CMotorDriver::~ CMotorDriver (void){}

void CMotorDriver::init(int IN1, int IN2, int IN3, int IN4)
{
 pinMode(IN1, OUTPUT); 
 pinMode(IN2, OUTPUT);
 pinMode(IN3, OUTPUT);
 pinMode(IN4, OUTPUT);

 C_IN1=IN1; 
 C_IN2=IN2; 
 C_IN3=IN3; 
 C_IN4=IN4;	          
}

void CMotorDriver::STOP()
{
digitalWrite (C_IN1, LOW) ;
digitalWrite (C_IN2, LOW) ;
digitalWrite (C_IN3, LOW) ;
digitalWrite (C_IN4, LOW) ;
}

void CMotorDriver::Move_UP()
{
digitalWrite (C_IN2, HIGH) ;    
digitalWrite (C_IN3, HIGH) ;	 
digitalWrite (C_IN1, LOW) ;
digitalWrite (C_IN4, LOW) ;
}

void CMotorDriver::Move_DOWN()
{
digitalWrite (C_IN4, HIGH) ; 
digitalWrite (C_IN1, HIGH) ;       
digitalWrite (C_IN2, LOW) ;
digitalWrite (C_IN3, LOW) ;
}

void CMotorDriver::Move_LEFT()
{
digitalWrite (C_IN3, HIGH) ; 
digitalWrite (C_IN1, HIGH) ;     
digitalWrite (C_IN2, LOW) ;
digitalWrite (C_IN4, LOW) ;
}

void CMotorDriver::Move_RIGHT()
{
digitalWrite (C_IN4, HIGH) ; 
digitalWrite (C_IN2, HIGH) ;       
digitalWrite (C_IN1, LOW) ;
digitalWrite (C_IN3, LOW) ;
}




