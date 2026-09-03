#include <Arduino.h>

TaskHandle_t loop2Handle;
void loop2(void* pvParameters);

#define LED1 32
#define LED2 33

void setup()
{
    Serial.begin(115200);
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    //Tasks
    //The main loop runs on core 1, so we use core 0 for the second loop
    xTaskCreatePinnedToCore(loop2, "loop2", 10000, NULL, 1, &loop2Handle, 0);
    delay(500);
}


void loop()
{
    digitalWrite(LED1, HIGH);
    delay(500);
    digitalWrite(LED1, LOW);
    delay(500);
}

void loop2(void* pvParameters)
{
    while(1)
    {
        digitalWrite(LED2, HIGH);
        delay(200);
        digitalWrite(LED2, LOW);
        delay(200);
    }
}