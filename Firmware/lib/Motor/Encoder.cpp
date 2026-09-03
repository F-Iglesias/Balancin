#include <Encoder.h>


Encoder::Encoder(int pinA, int pinB)
{
    this->pinA = pinA;
    this->pinB = pinB;
}





void Encoder::begin()
{
    pinMode(pinA, INPUT);
    pinMode(pinB, INPUT);
    A = digitalRead(pinA);
    B = digitalRead(pinB);
}

int Encoder::getTurning() {
    int turning = 0;
    bool newA = digitalRead(pinA);
    bool newB = digitalRead(pinB);
    if (newA != A)
    {
        A = newA;
        turning += (A==B)? 1: -1;
    }
    else if (newB != B)
    {
        B = newB;
        turning += (A==B)? -1: 1;
    }
    return turning;
}

void Encoder::update(float deltaTime_ms)
{
    int turning = getTurning();
    position += turning;
    if (time_ms <= sampleTime_ms) {
        n_samples ++;
        time_ms += deltaTime_ms;
        counter += turning;
        
        sum_t += time_ms;
        sum_t2 += time_ms * time_ms;
        sum_counter += counter;
        sum_tcounter += time_ms * counter;
    }
    else
    {
        n_samples = 0;
        time_ms = 0;
        counter = 0;

        sum_t = 0;
        sum_t2 = 0;
        sum_counter = 0;
        sum_tcounter = 0;

        speed = (n_samples * sum_tcounter - sum_t*sum_counter) / (n_samples*sum_t2 - sum_t*sum_t ) * 1000;
    }

}




