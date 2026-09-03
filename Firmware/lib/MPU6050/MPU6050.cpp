#include <MPU6050.h>



void MPU6050::Setup() {

#pragma region i2c_setup
    Wire.begin(I2C_SDA, I2C_SCL, 400000); //sda, scl, clock speed;
    Wire.beginTransmission(I2C_ADD); //MPU address

    //We set the PWR_MGMT_1 register of MPU6050 to 0.This wakes the MPU
    Wire.write(0x6B);
    Wire.write(0);
    Wire.endTransmission(true);
#pragma endregion

#pragma region calibration

    //To do the calibration we need to place the IMU horizontally

    //The number of samples we are averaging
    const int nsamples = 1000;

    //The real values when the IMU is placed horizontally
    const float realAcc[3] = {0, 0, 1};
    const float realGyro[3] = {0, 0, 0}; 

    float acc[3], gyro[3];

    //We measure the offsets and then take the average value
    //In general offset = real value - measured value
    for (int i = 0; i < nsamples; i++)
    {
        I2CRead(acc, gyro);
        for (int i = 0; i < 3; i++) {
            accOffset[i] += realAcc[i] - acc[i];
            gyroOffset[i] += realGyro[i] - gyro[i];
        }
        delay(4);
    }
    for (int i = 0; i < 3; i++) {
        accOffset[i] *= 1.0f/nsamples;
        gyroOffset[i] *= 1.0f/nsamples;
    }

#pragma endregion

    kalmanRoll.SetAngle(0);
    kalmanPitch.SetAngle(0);


};

void MPU6050::I2CRead(float (&acc)[3], float (&gyro)[3]) { //0, 1, 2, represent x, y, z 

    Wire.beginTransmission(0x68);
    Wire.write(0x1A);
    Wire.write(0x05);
    Wire.endTransmission();
    Wire.beginTransmission(0x68);
    Wire.write(0x1C);
    Wire.write(0x10);
    Wire.endTransmission();
    Wire.beginTransmission(0x68);
    Wire.write(0x3B);
    Wire.endTransmission(); 
    Wire.requestFrom(0x68,6);

    for (int i = 0; i < 3; i++) {
        acc[i] = (int16_t)(Wire.read() << 8 | Wire.read())/4096.f; //acceleration measured in g units
    }
   
    Wire.beginTransmission(0x68);
    Wire.write(0x1B); 
    Wire.write(0x8);
    Wire.endTransmission();     
    Wire.beginTransmission(0x68);
    Wire.write(0x43);
    Wire.endTransmission();
    Wire.requestFrom(0x68,6);

    for (int i = 0; i < 3; i++) {
        gyro[i] = (int16_t)(Wire.read() << 8 | Wire.read())/65.f; //rate in degrees per second 
    }
};

void MPU6050::I2CReadWithOffsets(float (&acc)[3], float (&gyro)[3])
{
    float acc2[3], gyro2[3];
    I2CRead(acc2, gyro2);

/***** Applying offsets *****/
    for (int i = 0; i < 3; i++) {
        acc[i] = acc2[i] +  accOffset[i];
        gyro[i] = gyro2[i] + gyroOffset[i];
    }

}


void MPU6050::Update(float dt) {

/***** Getting i2c readings *****/

    float acc[3], gyro[3];
    I2CRead(acc, gyro);

/***** Applying offsets *****/
    for (int i = 0; i < 3; i++) {
        acc[i] += accOffset[i];
        gyro[i] += gyroOffset[i];
    }

/***** Updating rates and angles *****/

    //Accelerometer angles
    float accAngleRoll = atan(acc[1]/sqrt(acc[0]*acc[0] + acc[2]*acc[2])) * 180/PI;
    float accAnglePitch = atan(-acc[0]/sqrt(acc[1]*acc[1] + acc[2]*acc[2])) * 180/PI;

    //Updating kalman filter
    kalmanRoll.Update(accAngleRoll, gyro[0], dt);
    kalmanPitch.Update(accAnglePitch, gyro[1], dt);

    //Setting rates and angles
    rate[0] = kalmanRoll.GetRate();
    rate[1] = kalmanPitch.GetRate();
    rate[2] = gyro[2];

    angle[0] = kalmanRoll.GetAngle();
    angle[1] = kalmanPitch.GetAngle();
    angle[2] += rate[2] * dt;
}
