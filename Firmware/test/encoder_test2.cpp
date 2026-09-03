#include <Arduino.h>

int volatile enc1A = 12;
int volatile enc1B = 14;
//12. 14, 26, 27
int volatile counter = 0;

void enc1AInterrupt()
{
    bool A = digitalRead(enc1A);
    bool B = digitalRead(enc1B);
    
    if (A==B) 
        counter--;

    else
        counter++;
}



void setup()
{
    Serial.begin(115200);
    pinMode(12, INPUT);
    pinMode(14, INPUT);
    attachInterrupt(digitalPinToInterrupt(enc1A), enc1AInterrupt, CHANGE);

}


void loop()
{
    Serial.println(counter);
    delay(10);
}

