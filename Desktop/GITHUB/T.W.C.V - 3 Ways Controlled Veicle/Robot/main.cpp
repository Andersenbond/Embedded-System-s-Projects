#include "PracticalSocket.h"  // For Socket, ServerSocket, and SocketException
#include <iostream>           // For cout, cerr
#include <cstdlib>            // For atoi()  
#include <pthread.h>          // For POSIX threads  
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>
#include <errno.h>
#include "CUltrasonicSensor.h"
#include "CMotorDriver.h"
#include <wiringPi.h>

// GPIO Pins 
#define IR_PIN 11 
#define IN1 9
#define IN2 22
#define IN3 10
#define IN4 17
#define ECHO_PIN 18  
#define TRIG_PIN 4
#define Buzzer 23

TCPSocket* App=new TCPSocket(0);

bool front_obstacle=0, back_obstacle=0;                bool last=0;

const int RCVBUFSIZE = 32; 

void HandleTCPClient(TCPSocket *sock);     // TCP client handling function
void *ThreadMain(void *arg);               // Main program of a thread  
void *USThread(void *arg);
void *SendUSThread(void *arg); 


sched_param param; 

CMotorDriver MotorDriver;

//unsigned long last_interrupt_time = 0;

//_______________________________________________________________________________IR_INTERRUPT_________________________________________________

void myInterrupt(void) {
					
if(digitalRead(IR_PIN)==0) //entrou no obstaculo frontal
{
front_obstacle=1;
if(last)MotorDriver.STOP();
digitalWrite(Buzzer, HIGH);
try{App->send(1);} catch (SocketException &e) {}
}

else //saiu do obstaculo frontal
{
front_obstacle=0;
digitalWrite(Buzzer, LOW);
try{App->send(2);} catch (SocketException &e) {}
}
								
}

//________________________________________________________________________________main_______________________________________________________

int main(int argc, char *argv[]) {  

  if (wiringPiSetupGpio () < 0) {                                                 //wiringPiSetupGpio () for GPIO PIN SETUP
  	fprintf (stderr, "Unable to setup wiringPi: %s\n", strerror (errno));
  	return 1;
  } 

  MotorDriver.init(IN1,IN2,IN3,IN4);                            

  if ( wiringPiISR (IR_PIN, INT_EDGE_BOTH, &myInterrupt) < 0 ) {           //Setup ISR for InfraRed Sensor Pin
  	fprintf (stderr, "Unable to setup ISR: %s\n", strerror (errno));
        return 1;
  }

  if (argc != 2) {                                                            // Test for correct number of arguments 
	cerr << "Usage: " << argv[0] << " <Server Port> " << endl; 
	exit(1);
  }  
  
  pinMode(IR_PIN, INPUT);
  unsigned short echoServPort = atoi(argv[1]);                                // First arg:  local port  

  pthread_t USthreadID, SendUSThreadID; 
  pthread_attr_t UStattr, SendUStattr;
  
  pthread_attr_init (&UStattr);
  pthread_attr_getschedparam (&UStattr, &param);
  param.sched_priority = 2;
  pthread_attr_setschedparam (&UStattr, &param);
  if (pthread_create (&USthreadID, &UStattr, USThread, NULL) != 0) {cerr << "Unable to create thread" << endl;exit(1);}
 
  pthread_attr_init (&SendUStattr);
  pthread_attr_getschedparam (&SendUStattr, &param);
  param.sched_priority = 3;
  pthread_attr_setschedparam (&SendUStattr, &param);
  if (pthread_create(&SendUSThreadID, &SendUStattr, SendUSThread, NULL) != 0) {cerr << "Unable to create thread" << endl;exit(1);}

  try {
    TCPServerSocket servSock(echoServPort);   // Socket descriptor for server  
  
    for (;;) {      // Run forever  
      
      TCPSocket *clntSock = servSock.accept();   // Create separate memory for client argument  
      
      // Create client thread  
      pthread_t threadID;              // Thread ID from pthread_create()  
      pthread_attr_t tattr;
      pthread_attr_init (&tattr);
      pthread_attr_getschedparam (&tattr, &param);
      param.sched_priority = 1;
      pthread_attr_setschedparam (&tattr, &param);
      if (pthread_create(&threadID, &tattr, ThreadMain, (void *) clntSock) != 0) {cerr << "Unable to create thread" << endl;exit(1);}
    }
  } catch (SocketException &e) {
    cerr << e.what() << endl;
    exit(1);
  }
  // NOT REACHED
  return 0;
}

//________________________________________________________________TCP client handling function_______________________________________________

void HandleTCPClient(TCPSocket *sock) {

  cout << "Handling client ";

  try   {cout << sock->getForeignAddress() << ":";} 
  catch (SocketException &e) {cerr << "Unable to get foreign address" << endl;}
  
  try   { cout << sock->getForeignPort();} 
  catch (SocketException &e) {cerr << "Unable to get foreign port" << endl;}
  
  cout << " with thread " << pthread_self() << endl;

  char Buffer[RCVBUFSIZE];
  int recvMsgSize;

  while ((recvMsgSize = sock->recv(Buffer, RCVBUFSIZE)) > 0) { // Zero means end of transmission
                                                         
  for(int i=0;i<recvMsgSize;i++) 
  {
       switch(Buffer[i])
       {
       case '0': 	    MotorDriver.STOP();						break;	//00110000 (48d) ('0') stop
       case '1':            last=1;	if(!front_obstacle) MotorDriver.Move_UP();	break;	//00110001 (49d) ('1') up
       case '2': 	    last=0;   	if(!back_obstacle)  MotorDriver.Move_DOWN(); 	break;	//00110010 (50d) ('2') down
       case '3': 	    MotorDriver.Move_RIGHT();					break;	//00110011 (51d) ('3') right
       case '4': 	    MotorDriver.Move_LEFT();					break;	//00110100 (52d) ('4') left
       case '5': 	    App = sock;                             			break;	//00110101 (53d) ('5') APP connected
       case 128 ... 255:    try{App->send(Buffer[i]);} catch (SocketException &e) {}	break;  //192-255(start)  128-191 (+5)
       }		
  }
  } 
  // Destructor closes socket
}

//______________________________________________________________________________________________________________________________________

void *ThreadMain(void *clntSock) {

  pthread_detach(pthread_self());  // Guarantees that thread resources are deallocated upon return  
  HandleTCPClient((TCPSocket *) clntSock);// Extract socket file descriptor from argument

  delete (TCPSocket *) clntSock;
  return NULL;
}
//______________________________________________________________________________________________________________________________________

void *USThread(void *arg) {

  pthread_detach(pthread_self());  // Guarantees that thread resources are deallocated upon return  
  
  CUltrasonicSensor x;  
  pinMode(Buzzer, OUTPUT);

  while(1)
  {
  	if(x.Get_Distance_cm(ECHO_PIN,TRIG_PIN)<10)      //back obstacle
  	{
        if(back_obstacle==0)MotorDriver.STOP();
	back_obstacle=1;
   	digitalWrite(Buzzer, HIGH); 
  	}
 	
	else 						//nao tem back obstacle
	{
	back_obstacle=0;
	if (front_obstacle==0) digitalWrite(Buzzer, LOW);
	}

  }

  return NULL;
}

//______________________________________________________________________________________________________________________________________

void *SendUSThread(void *arg) {

  pthread_detach(pthread_self());  // Guarantees that thread resources are deallocated upon return  
  													
  unsigned char value;    
  CUltrasonicSensor x; 
                                                          
  while(1)
	{														 
	value= x.Get_Distance_cm(ECHO_PIN,TRIG_PIN); 
	if (value>63) value=63;
	try{App->send(value+64);} catch (SocketException &e) {printf("No App\n");} 
	delay(1000);
	}
		
  return NULL;
}



