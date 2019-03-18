#ifndef UltrasonicSensor_CLASS_H
#define UltrasonicSensor_CLASS_H

using namespace std;

class CUltrasonicSensor
{
public:
CUltrasonicSensor (void);
~ CUltrasonicSensor (void);
int Get_Distance_cm(int ECHO_PIN, int TRIG_PIN);
private:
    
};

#endif
