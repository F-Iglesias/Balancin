#ifndef MPU6050_h
#define MPU6050_h
#include <Arduino.h>
#include <Wire.h>
#include <Kalman.h>


class MPU6050 {
public:

    //This function initializes i2c communication and  
    void Setup(); 

    //updates the output values. dt must be on miliseconds
    void Update(float dt);

    //Roll, pitch and yaw are represented by 0, 1, 2, respectively
    float angle[3] = {0, 0, 0}, rate[3] = {0, 0, 0};
    //Sets the readings from the IMU    
    void I2CRead(float (&acc)[3], float (&gyro)[3]); 

    void I2CReadWithOffsets(float (&acc)[3], float (&gyro)[3]);
private:

    int I2C_SDA = 32, I2C_SCL = 33, I2C_ADD = 0x68; //I2C pins and adress for the MPU6050. These are used on I2CRead

    //imu readings offsets
    float accOffset[3] = {0, 0, 0}, gyroOffset[3] = {0, 0, 0};

    //We use kalman filter to estimate the roll and pitch angles. The yaw angle is estimated via integration
    Kalman kalmanRoll, kalmanPitch;

};
#endif