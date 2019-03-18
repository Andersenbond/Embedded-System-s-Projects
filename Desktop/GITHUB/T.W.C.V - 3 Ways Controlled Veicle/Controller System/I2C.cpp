#include "I2C.h"

CI2CCommunication::CI2CCommunication(void){
	this->I2CPort = "/dev/i2c-1";            			// Name of the  i2c port on our raspberry pi in /dev
   
}

CI2CCommunication::CI2CCommunication(char* I2CPort){
	this->I2CPort = I2CPort;
}

CI2CCommunication::~CI2CCommunication(void){
	//if ((fileDescriptor == release(I2CPort, O_RDWR)))               // release port for reading and writing
      		//printf("Failed to closing i2c Device Driver\n");
   	 
}

int CI2CCommunication::writeAccelerometer(char * buf, int len){
	if (write(fileDescriptor, buf, len) != len){
		fprintf(stderr, "Can't write to device\n");
		return -1;
	}
	else 
		return 0;
}


char* CI2CCommunication::getI2CPort(void){
	return I2CPort;
}
int CI2CCommunication::getI2Cfp(void){
	return fileDescriptor;
}
void CI2CCommunication::setI2Cfp(int fp){
	fileDescriptor = fp;
}
int CI2CCommunication::readAccelerometer(short &x , short &y, short &z ){
	
	if (ioctl(fileDescriptor, I2C_SLAVE, 0x53) < 0){ // check the accelerometer sensor
		fprintf(stderr, "ADXL345 sensor not found3");       		
		return -1;
	}
	char buf[7];
   	buf[0] = 0x32;                                       
   	if(writeAccelerometer(buf,2) != 0){
       		return -2;
   	}
   	if (read(fileDescriptor, buf, 6) != 6) {                        
      		printf("Unable to read from ADXL345 Sensor\n");
      		return -3;
   	}
   	else {
		 
		x = (buf[1]<<8) |  buf[0]; 
      		y = (buf[3]<<8) |  buf[2];
       		z = (buf[5]<<8) |  buf[4]; 

  	}
   return 0;
}
 

int CI2CCommunication::init(){

   	if ((fileDescriptor = open(I2CPort, O_RDWR)) < 0) {               		// Open port for reading and writing
      		printf("Failed to open i2c Device Driver\n");
      		return -1;
   	}
   	 //printf("FileDescriptop = %d\n", fileDescriptor);
   return 0;
}

int CI2CCommunication::getFiledescriptor(void){
	return fileDescriptor;
}

