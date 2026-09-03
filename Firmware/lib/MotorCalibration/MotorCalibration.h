#pragma once
#include <Arduino.h>
#include <Encoder.h>
#include <Motor.h>


void LeastSquareSpeed(Encoder& enc, float& speed, float deltaTime_ms, int nsamples);