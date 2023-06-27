
#ifndef MPU_H
#define MPU_H

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

class IMU
{
private:
    Adafruit_MPU6050 mpu;
    sensors_event_t a, g, temp;
public:
    IMU();
    ~IMU();
    void init();
    void init_motion_detection();
    void debug();
    void detect_motion();
};

#endif // MPU_H
