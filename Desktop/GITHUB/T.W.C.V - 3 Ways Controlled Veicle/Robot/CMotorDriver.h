#ifndef MotorDriver_CLASS_H
#define MotorDriver_CLASS_H

using namespace std;

class CMotorDriver
{
public:
CMotorDriver (void);
~ CMotorDriver (void);
void init(int IN1, int IN2, int IN3, int IN4);
void Move_UP();
void Move_DOWN();
void Move_LEFT();
void Move_RIGHT();
void STOP();
private:
int C_IN1,C_IN2,C_IN3,C_IN4;
    
};

#endif
