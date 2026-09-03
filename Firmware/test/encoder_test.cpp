#include <Arduino.h>
#include <Motor.h>
#include <Encoder.h>
#include <tuningTools.h>


int encA = 12, encB = 14;
int counter = 0;


TaskHandle_t controlStage;
void controlStageLoop(void* pvParameters);

void setup()
{
    Serial.begin(115200);
    pinMode(encA, INPUT);
    pinMode(encB, INPUT);
    xTaskCreatePinnedToCore(controlStageLoop, "controlStage", 100000, NULL, 1, &controlStage, 0);               
    delay(500); 
}


void controlStageLoop(void* pvParameters)
{
    int A = 0, B = 0;
    while(1)
    {

        int newA = digitalRead(encA);
        int newB = digitalRead(encB);
        if (newA!=A)
        {
            A = newA;
            if (A==B)
                counter++;
            else 
                counter--;
        }
        else if(newB !=B)
        {
            B = newB;
            if (A==B)
                counter--;
            else
                counter++;
        }

    
        delay(1);
    }
}


void loop()
{
    Serial.println(counter);
    delay(10);
}