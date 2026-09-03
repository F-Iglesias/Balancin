#ifndef _Encoder_h_
#define _Encoder_h_

#include <Arduino.h>


class Encoder {

public:
    Encoder(int pinA, int pinB);  
    void begin();

    int getTurning();
    void update(float deltaTime_ms);
    float getPosition();
    float getSpeed();
    float resetPosition();
    int counter = 0;
    
private:
    int pinA, pinB;
    bool A, B;
    int position = 0;


    //Variables for speed calculation
    float sampleTime_ms = 200;
    float n_samples = 0;
    float time_ms = 0;
    
    float sum_t = 0, sum_counter = 0, sum_t2 = 0, sum_tcounter = 0;
    float speed = 0; //speed on ticks per second
};


#endif