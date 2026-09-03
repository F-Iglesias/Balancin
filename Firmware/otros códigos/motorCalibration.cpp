#include <Arduino.h>
#include <Encoder.h>
#include <Motor.h>
#include <tuningTools.h>

Motor motor(19, 18, 0, 1);
Encoder encoder(12, 14);
float deltaTime_ms = 1;
float n_samples = 50;

float GetSpeed(float deltaTime_ms, int n_samples);

void setup()
{
    SerialBT.begin("esepe32");
    motor.Setup();
    encoder.Setup();

}

void loop()
{
    if (SerialBT.available())
    {
        String text = SerialBT.readStringUntil('\n');
        float val = getValue(text);
        if (includedIn("get", text)) {
            if (includedIn("speed", text)) {
                encoder.counter = 0;
                float speed = GetSpeed(deltaTime_ms, n_samples);
                SerialBT.print("speed = "); SerialBT.println(speed);
                SerialBT.print("counter = "); SerialBT.println(encoder.counter);
            }
            else if (includedIn("data", text)) {
                SerialBT.print("dt(ms) = "); SerialBT.println(deltaTime_ms);
                SerialBT.print("n_samples = "); SerialBT.println(n_samples);
            }
            else if (includedIn("points", text)) {
            
                for (int i = 0; i < 10; i++)
                {
                    motor.SetSpeed(100 - 10*i);
                    encoder.counter = 0;
                    float speed = GetSpeed(deltaTime_ms, n_samples);
                    SerialBT.print(i); SerialBT.print("th iteration: duty = ");
                    SerialBT.print(100-10*i);
                    SerialBT.print("    speed = "); SerialBT.println(speed);
                }
            }
        }
        else if (includedIn("set", text))
        {
            if (includedIn("dt", text))
            {
                deltaTime_ms = val;
                SerialBT.print("deltaTime_ms set to "); SerialBT.println(val);
            }
            else if(includedIn("n", text)) {
                n_samples = val;    
                SerialBT.print("n_samples set to "); SerialBT.println(val);
            }
            else if (includedIn("motor duty", text)) {
                motor.SetSpeed(val);
            }
        }
        SerialBT.println();


    }
    delay(10);
}

float GetSpeed(float deltaTime_ms, int n_samples)
{

    float time_ms = 0;
    float angle;
    float ith_sample = 1;

    float sum_t = 0, sum_t2 = 0, sum_a = 0, sum_at = 0;
    SerialBT.print("    Estimated time = "); SerialBT.println(deltaTime_ms * n_samples);
    while (time_ms <= deltaTime_ms * n_samples)
    {
        uint32_t timer = micros();
        encoder.UpdateCounter();
        angle = encoder.counter;

        if (time_ms >= ith_sample*deltaTime_ms && time_ms < (ith_sample+1)*deltaTime_ms)
        {
            sum_t += time_ms;
            sum_t2 += time_ms*time_ms;
            sum_a += angle;
            sum_at += angle*time_ms;

            ith_sample++;    
        }

        while (micros()-timer < 200);
        time_ms += 0.2;
    }


    float speed = (n_samples * sum_at - sum_t*sum_a) / (n_samples*sum_t2 - sum_t*sum_t ) * 1000;
    return speed;
}