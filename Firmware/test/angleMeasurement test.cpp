#include <Arduino.h>
#include <MPU6050.h>


/******** This program estimates the pitch angle using the accelerometer, the gyroscope and the kalman filter ********/

TaskHandle_t loop2Handle;
void loop2(void* pvParameters);

MPU6050 imu;
bool printed = 0;

float kalmanAngle, accAngle, gyroAngle;

void setup()
{
    Serial.begin(115200);
    
    imu.Setup();
    //Tasks
    //The main loop runs on core 1, so that we use core 0 for the second loop
    xTaskCreatePinnedToCore(loop2, "loop2", 10000, NULL, 1, &loop2Handle, 0);
    delay(500);
}


void loop()
{
    if (!printed) {
        Serial.print(accAngle); Serial.print("|");
        Serial.print(gyroAngle); Serial.print("|");
        Serial.print(kalmanAngle); Serial.print("\n");
        printed = 1;
    }
}

void loop2(void* pvParameters)
{
    const uint32_t deltaTime = 4000; //
    const float dt = deltaTime/1000000.f;
    uint32_t timer;
    float acc[3], gyro[3];
    while(1)
    {
        timer = micros();

#pragma region logic

        //imu.I2CReadWithOffsets(acc, gyro);
        imu.I2CReadWithOffsets(acc, gyro);       
        imu.Update(deltaTime/1000000.f);

        accAngle = atan(-acc[0]/sqrt(acc[1]*acc[1] + acc[2]*acc[2])) * 180/PI;
        gyroAngle += gyro[1] * dt;
        kalmanAngle = imu.angle[1];

#pragma endregion
        printed = 0;
        while (micros() - timer < deltaTime);

    }
}