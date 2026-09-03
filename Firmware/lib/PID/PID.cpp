#include <PID.h>

PID::PID() {
    E = dE = IE = 0;
    Kp = Ki = Kd = 0;
};

PID::PID(float Kp, float Ki, float Kd) {
    E = dE = IE = 0;
    Kp = Ki = Kd = 0;
    this->Kp = Kp;
    this->Ki = Ki;
    this->Kd = Kd;
}


void PID::SetOffset(float offset) { this->offset = offset; };

void PID::SetPID(float Kp, float Ki, float Kd) {
    this->Kp = Kp;
    this->Ki = Ki;
    this->Kd = Kd;
}


void PID::Update(float newE, float newDE, float dt) {

    E = newE - offset;
    dE = newDE;
    
    IE += E*dt;
    
    //Clamping IE
    if (IE<-100) {
        IE = -100;
    }
    else if (IE>100) {
        IE = 100;
    }

    out = Kp*E + + Ki * IE + Kd*dE;
};

