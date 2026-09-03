#include <MotorCalibration.h>

void LeastSquareSpeed(Encoder& enc, float& speed, float deltaTime_ms, int nsamples)
{
    for (int i = 0; i < nsamples; i++)
    {
        uint32_t timer = micros();
            enc.Update();

        while (timer - micros() < deltaTime_ms * 1000)
    }
}