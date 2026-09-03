#ifndef PID_H
#define PID_h

#include <Arduino.h>
#include <utils.h>

class PID {
public:
    PID();
    PID(float Kp, float Ki, float Kd);

    void Update(float newE, float newDE, float dt); //dt is used to calculate IE
    void SetPID(float Kp, float Ki, float Kd);
    void SetOffset(float offset);
    float out = 0;
    float volatile Kp, Kd, Ki; //PID constants

    float offset = 0;
    float E; //Error
    float IE; //Integral of error
    float dE; //dE //derivative of error


};

#endif