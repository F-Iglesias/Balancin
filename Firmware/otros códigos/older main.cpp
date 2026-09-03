#include <Arduino.h>

#include <balancin.h>

#include <tuningTools.h>

Balancin balancin;

TaskHandle_t controlStage;
void controlStageLoop(void* pvParameters);
void tuning();

void setup()
{
  Serial.begin(115200);
  SerialBT.begin("ESP32");  

  balancin.Setup();

  //******Tasks*******
  xTaskCreatePinnedToCore(controlStageLoop, "controlStage", 100000, NULL, 1, &controlStage, 0);               
  delay(500); 

}

// Core 1
void controlStageLoop(void* pvParameters)
{
  //loop
  while (1)
  {
    uint32_t loopTimer = micros(); 
    balancin.Update();
    //balancin.Print();
    while ((micros() - loopTimer)<4); //Ensures the loop lasts at least 4ms
  }
}


// Core 0 loop
void loop()
{
  tuning();
  delay(1);


}
void tuning() {

  if (SerialBT.available()) {
      String text = SerialBT.readStringUntil('\n');
  //SerialBT adds an space to text, so it's impossible for it to be empty
      if (includedIn("set", text)) { //Sets integral to zero.
        float val = getValue(text);
        if (includedIn("pidm", text)) {
          if (includedIn("kp", text)) {
            balancin.pidM.SetKp(val);
          }
          else if (includedIn("ki", text)) {
            balancin.pidM.ResetIE();
            balancin.pidM.SetKi(val);
          }
          else if (includedIn("kd", text)) {
            balancin.pidM.SetKd(val);
          }
          else if (includedIn("ri", text)) {
            balancin.pidM.ResetIE();
          }
          else if(includedIn("tar", text)) {
            balancin.pidM.SetTarget(val);
          }
          else if(includedIn("iemax", text)) {
            balancin.pidM.SetIEmax(val);
          }
          else if (includedIn("pmax", text)) {
            balancin.pidM.SetPmax(val);
          }
          else if (includedIn("imax", text)) {
            balancin.pidM.SetImax(val);
          }
          else if (includedIn("dmax", text)) {
            balancin.pidM.SetDmax(val);
          }
          else if (includedIn("threshold", text)) {
            balancin.pidM.SetThreshold(val);
          }
        }
        else if(includedIn("pidyaw", text)) {
          if (includedIn("kp", text)) {
            balancin.pidYaw.SetKp(val);
          }
          else if (includedIn("ki", text)) {
            balancin.pidYaw.ResetIE();
            balancin.pidYaw.SetKi(val);
          }
          else if (includedIn("kd", text)) {
            balancin.pidYaw.SetKd(val);
          }
          else if (includedIn("ri", text)) {
            balancin.pidYaw.ResetIE();
          }
          else if(includedIn("tar", text)) {
            balancin.pidYaw.SetTarget(val);
          }
          else if(includedIn("iemax", text)) {
            balancin.pidYaw.SetIEmax(val);
          }
          else if (includedIn("pmax", text)) {
            balancin.pidYaw.SetPmax(val);
          }
          else if (includedIn("imax", text)) {
            balancin.pidYaw.SetImax(val);
          }
          else if (includedIn("dmax", text)) {
            balancin.pidYaw.SetDmax(val);
          }
          else if (includedIn("threshold", text)) {
            balancin.pidYaw.SetThreshold(val);
          }
          else if(includedIn("inv", text)) {
            balancin.pidYaw.Invert();
          }
        }

        else if(includedIn("m1", text)) {
          balancin.m1.Invert();
        }
        else if(includedIn("m2", text)) {
          balancin.m2.Invert();
        }

      }
      else if (includedIn("get", text)) {
        if (includedIn("pidm", text)) {
          SerialBT.print("kalmanRoll = "); SerialBT.println(balancin.mpu.GetRoll());
          SerialBT.println();
          SerialBT.print("E = "); SerialBT.println(balancin.pidM.GetE());
          SerialBT.print("IE = "); SerialBT.println(balancin.pidM.GetIE());
          SerialBT.print("DE = "); SerialBT.println(balancin.pidM.GetDE());
          SerialBT.println();
          SerialBT.print("Kp = "); SerialBT.println(balancin.pidM.GetKp());
          SerialBT.print("Ki = "); SerialBT.println(balancin.pidM.GetKi());
          SerialBT.print("Kd = "); SerialBT.println(balancin.pidM.GetKd());
          SerialBT.println();
          SerialBT.print("P = "); SerialBT.println(balancin.pidM.GetP());
          SerialBT.print("I = "); SerialBT.println(balancin.pidM.GetI());
          SerialBT.print("D = "); SerialBT.println(balancin.pidM.GetD());
          SerialBT.print("out = "); SerialBT.println(balancin.pidM.Out());
          SerialBT.println();
          SerialBT.print("Target = "); SerialBT.println(balancin.pidM.GetTarget());
          SerialBT.print("threshold = "); SerialBT.println(balancin.pidM.GetThreshold());
      }
      else if (includedIn("pidyaw", text)) {
        SerialBT.print("kalmanPitch = "); SerialBT.println(balancin.mpu.GetYaw());
        SerialBT.println();
        SerialBT.print("E = "); SerialBT.println(balancin.pidYaw.GetE());
        SerialBT.print("IE = "); SerialBT.println(balancin.pidYaw.GetIE());
        SerialBT.print("DE = "); SerialBT.println(balancin.pidYaw.GetDE());
        SerialBT.println();
        SerialBT.print("Kp = "); SerialBT.println(balancin.pidYaw.GetKp());
        SerialBT.print("Ki = "); SerialBT.println(balancin.pidYaw.GetKi());
        SerialBT.print("Kd = "); SerialBT.println(balancin.pidYaw.GetKd());
        SerialBT.println();
        SerialBT.print("P = "); SerialBT.println(balancin.pidYaw.GetP());
        SerialBT.print("I = "); SerialBT.println(balancin.pidYaw.GetI());
        SerialBT.print("D = "); SerialBT.println(balancin.pidYaw.GetD());
        SerialBT.print("out = "); SerialBT.println(balancin.pidYaw.Out());
        SerialBT.println();
        SerialBT.print("Target = "); SerialBT.println(balancin.pidYaw.GetTarget());
        SerialBT.print("threshold = "); SerialBT.println(balancin.pidYaw.GetThreshold());
    }


  }
  
}
}
