#include <Motor.h>


Motor::Motor()
{

}
Motor::Motor(int pin1, int pin2, int ch1, int ch2)
{
    this->pin1 = pin1;
    this->pin2 = pin2;
    this->ch1 = ch1;
    this->ch2 = ch2;

}

void Motor::Setup()
{
    
    //Set motor pins as output
    pinMode(pin1, OUTPUT);
    pinMode(pin2, OUTPUT);  

    //Set freq and resolution
    ledcSetup(ch1, freq, res_bits);
    ledcSetup(ch2, freq, res_bits);

    //Attach PWM channels to motor pins:
    ledcAttachPin(pin1, ch1);
    ledcAttachPin(pin2, ch2);

    //Set all channels to zero:
    ledcWrite(ch1, 0);
    ledcWrite(ch2, 0);
}

void Motor::Invert() { sgn *= -1; }; 

void Motor::SetSpeed(float speed) {
    
    
    int speed_bits = abs(speed/scale *  (float)res);

    if (speed_bits > res) //clamp speed_bits
        speed_bits = res;

    if (speed*sgn >= 0)
    {
        ledcWrite(ch2, 0);
        delayMicroseconds(50); //This guarantees that pin1 and pin2 will not be high at the same time
        ledcWrite(ch1, speed_bits);
    }
    else {
        ledcWrite(ch1, 0);
        delayMicroseconds(50); 
        ledcWrite(ch2, speed_bits);
    }
}