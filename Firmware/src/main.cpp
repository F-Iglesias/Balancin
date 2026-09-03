#include <Arduino.h>
#include <MPU6050.h>
#include <PID.h>
#include <Motor.h>
#include <Encoder.h>
#include <tuningTools.h>

TaskHandle_t loop2Handle;
void loop2(void* pvParameters);

MPU6050 imu;
PID pid;

Motor m1(19, 18, 0, 1);
Motor m2(5, 17, 2, 3);
Encoder enc1(12, 14);
bool volatile enable = 0;
float volatile ks, ka, kyaw = 0;
float volatile setpoint = 0;

void setup()
{
    
    m1.Setup();
    m2.Setup();
    
    Serial.begin(115200);
    SerialBT.begin("esepe32");
 
    imu.Setup();
 
    //Tasks
    //The main loop runs on core 1, so that we use core 0 for the second loop
    xTaskCreatePinnedToCore(loop2, "loop2", 10000, NULL, 1, &loop2Handle, 0);
    delay(500);
}


void loop()
{
#pragma region communication
  if (SerialBT.available()) {
      String text = SerialBT.readStringUntil('\n');
  //SerialBT añade un espacio al texto, así que es imposible que esté vacío
      if (includedIn("set", text)) { 
        float val = getValue(text);
        if (includedIn("pid", text)) {
             if (includedIn("kp", text)) { 
            pid.Kp = val;
             }
            else if (includedIn("ki", text)) {
            pid.IE = 0;
            pid.Ki = val;
            }
             else if (includedIn("kd", text)) {
            pid.Kd = val;
             }
            else if (includedIn("setpoint", text)) {
                setpoint = val;
            }
            else if(includedIn("enable", text)) {
                enable = (bool)val;
            }
         }
         else if (includedIn("ks", text)) {
            ks = val;
         }
         else if (includedIn("ka", text)) {
            ka = val;
         }
         else if(includedIn("kyaw", text)) {
            kyaw = val;
         }
         else if (includedIn("reset counter", text)) {
            enc1.counter = 0;
         }
      }
      else if (includedIn("get", text)) {
          SerialBT.print("angle = "); SerialBT.println(imu.angle[0]);
          SerialBT.println();
          SerialBT.print("E = "); SerialBT.println(pid.E);
          SerialBT.print("IE = "); SerialBT.println(pid.IE);
          SerialBT.print("DE = "); SerialBT.println(pid.dE);
          SerialBT.println();
          SerialBT.print("Kp = "); SerialBT.println(pid.Kp);
          SerialBT.print("Ki = "); SerialBT.println(pid.Ki);
          SerialBT.print("Kd = "); SerialBT.println(pid.Kd);
          SerialBT.print("ka = "); SerialBT.println(ka);
          SerialBT.print("ks = "); SerialBT.println(ks);
          SerialBT.print("kway = "); SerialBT.println(kyaw);
          SerialBT.println();
          SerialBT.print("PID = "); SerialBT.println(pid.out);
          SerialBT.println();
          SerialBT.print("offset = "); SerialBT.println(pid.offset);
          SerialBT.print("setpoint ="); SerialBT.println(setpoint);
          SerialBT.print("enable = "); SerialBT.println(enable);
          SerialBT.print("counter = "); SerialBT.println(enc1.counter);
          SerialBT.print("speed = "); SerialBT.println(enc1.speed);

      }

    }
#pragma endregion
}
  


void loop2(void* pvParameters)
{
    const uint32_t deltaTime = 2000; //
    const float dt = deltaTime/1000000.f;
    uint32_t timer;
    int sgn = 1;
    while(1)
    {
        timer = micros();

#pragma region logic     
        imu.Update(dt);
        enc1.update(dt*1000);
        pid.offset = ka*enc1.counter + ks*enc1.speed + setpoint;
        pid.Update(imu.angle[0], imu.rate[0], dt);
        m1.SetSpeed((pid.out + kyaw*imu.rate[2])*enable);
        m2.SetSpeed((pid.out*enable-kyaw*imu.rate[2])*enable);
        
#pragma endregion
        while (micros() - timer < deltaTime);

    }
}