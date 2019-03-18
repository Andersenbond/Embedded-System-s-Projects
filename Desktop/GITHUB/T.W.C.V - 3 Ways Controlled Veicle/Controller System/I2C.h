#ifndef _I2CCOMUNICATION_H_
#define _I2CCOMUNICATION_H_

#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdint.h>


class CI2CCommunication{
    public:
	CI2CCommunication(void);
	CI2CCommunication(char* I2CPort);
	~CI2CCommunication(void);
	int init(void);
	int getFiledescriptor(void);
	int writeAccelerometer(char * buf, int len);
	int readAccelerometer(short &x , short &y, short &z);
	char* getI2CPort(void);
	int getI2Cfp(void);
	void setI2Cfp (int fp);
    private:
	int fileDescriptor;
	char* I2CPort;
};


#endif
