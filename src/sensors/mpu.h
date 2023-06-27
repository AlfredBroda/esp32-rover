
#ifndef MPU_H
#define MPU_H

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <Fusion/Fusion.h>
#include <stdbool.h>
#include <stdio.h>

class IMU
{
public:
    IMU();
    ~IMU();
    void init();
    void init_motion_detection();
    void calibrate();
    void debug();
    void loop();
    int getHeading();
private:
    Adafruit_MPU6050 mpu;
    FusionAhrs ahrs;
    sensors_event_t a, g, temp;
    float heading;
    float rad2deg(float rad);
    float mps2g(float mps);
    unsigned long lastTime;
    FusionEuler euler; // [Roll, Pitch, Yaw] Euler angle container
    FusionVector a_cal; // accelerometer calibration data
    FusionVector g_cal; // gyroscope calibration data
};

#endif // MPU_H
