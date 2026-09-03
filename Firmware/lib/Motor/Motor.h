#ifndef Motor_h
#define Motor_h
#include <Arduino.h>

class Motor
{
public:
    Motor();
    Motor(int pin1, int pin2, int ch1, int ch2);

    void Setup();

    void Invert();
    void SetSpeed(float speed);

private:
    int pin1, pin2, ch1, ch2;
    float scale = 100;
    int freq = 30000;
    int res = 1023;
    int res_bits = 10;
    int sgn = 1;
};

#endif