#include "mpu.h"

#define SAMPLE_RATE (100)

void IMU::init()
{
    // Try to initialize!
    if (!mpu.begin())
    {
        Serial.println("Failed to find MPU6050 chip");
        while (1)
        {
            delay(10);
        }
    }
    Serial.println("MPU6050 Found!");

    mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
    Serial.print("Accelerometer range set to: ");
    switch (mpu.getAccelerometerRange())
    {
    case MPU6050_RANGE_2_G:
        Serial.println("+-2G");
        break;
    case MPU6050_RANGE_4_G:
        Serial.println("+-4G");
        break;
    case MPU6050_RANGE_8_G:
        Serial.println("+-8G");
        break;
    case MPU6050_RANGE_16_G:
        Serial.println("+-16G");
        break;
    }
    mpu.setGyroRange(MPU6050_RANGE_1000_DEG);
    Serial.print("Gyro range set to: ");
    switch (mpu.getGyroRange())
    {
    case MPU6050_RANGE_250_DEG:
        Serial.println("+- 250 deg/s");
        break;
    case MPU6050_RANGE_500_DEG:
        Serial.println("+- 500 deg/s");
        break;
    case MPU6050_RANGE_1000_DEG:
        Serial.println("+- 1000 deg/s");
        break;
    case MPU6050_RANGE_2000_DEG:
        Serial.println("+- 2000 deg/s");
        break;
    }

    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
    Serial.print("Filter bandwidth set to: ");
    switch (mpu.getFilterBandwidth())
    {
    case MPU6050_BAND_260_HZ:
        Serial.println("260 Hz");
        break;
    case MPU6050_BAND_184_HZ:
        Serial.println("184 Hz");
        break;
    case MPU6050_BAND_94_HZ:
        Serial.println("94 Hz");
        break;
    case MPU6050_BAND_44_HZ:
        Serial.println("44 Hz");
        break;
    case MPU6050_BAND_21_HZ:
        Serial.println("21 Hz");
        break;
    case MPU6050_BAND_10_HZ:
        Serial.println("10 Hz");
        break;
    case MPU6050_BAND_5_HZ:
        Serial.println("5 Hz");
        break;
    }

    // mpu.setHighPassFilter(MPU6050_HIGHPASS_0_63_HZ);
    Serial.print("High pass filter set to: ");
    switch (mpu.getHighPassFilter())
    {
    case MPU6050_HIGHPASS_0_63_HZ:
        Serial.println("0.63 Hz");
        break;
    case MPU6050_HIGHPASS_1_25_HZ:
        Serial.println("1.25 Hz");
        break;
    case MPU6050_HIGHPASS_2_5_HZ:
        Serial.println("2.5 Hz");
        break;
    case MPU6050_HIGHPASS_5_HZ:
        Serial.println("5 Hz");
        break;
    case MPU6050_HIGHPASS_DISABLE:
        Serial.println("Disabled");
        break;
    case MPU6050_HIGHPASS_UNUSED:
        Serial.println("Unused");
        break;
    }

    calibrate();
    FusionOffsetInitialise(&offset, SAMPLE_RATE);
    FusionAhrsInitialise(&ahrs);
    // debug();
}

void IMU::init_motion_detection()
{
    // setup motion detection
    mpu.setHighPassFilter(MPU6050_HIGHPASS_0_63_HZ);
    mpu.setMotionDetectionThreshold(1);
    mpu.setMotionDetectionDuration(20);
    mpu.setInterruptPinLatch(true); // Keep it latched.  Will turn off when reinitialized.
    mpu.setInterruptPinPolarity(true);
    mpu.setMotionInterrupt(true);
}

void IMU::calibrate()
{
    mpu.getEvent(&a, &g, &temp);
    delay(100);
    mpu.getEvent(&a, &g, &temp);

    g_cal = {-g.gyro.x, -g.gyro.y, -g.gyro.z};
    a_cal = {-a.acceleration.x, -a.acceleration.y, -(a.acceleration.z - 9.8f)}; // assume z-axis is pointing up

    Serial.printf("Calibration data: gX %0.3f, gY %0.3f, gZ %0.3f\n", g_cal.axis.x, g_cal.axis.y, g_cal.axis.z);
    Serial.printf("Calibration data: aX %0.3f, aY %0.3f, aZ %0.3f\n", a_cal.axis.x, a_cal.axis.y, a_cal.axis.z);

    g_cal_deg = {rad2deg(g_cal.axis.x), rad2deg(g_cal.axis.y), rad2deg(g_cal.axis.z)};
    a_cal_g = {mps2g(a_cal.axis.x), mps2g(a_cal.axis.y), mps2g(a_cal.axis.z)};
}

void IMU::debug()
{
    mpu.getEvent(&a, &g, &temp);

    /* Print out the values, calibrated */
    Serial.print("Acceleration X: ");
    Serial.print(a.acceleration.x + a_cal.axis.x);
    Serial.print(", Y: ");
    Serial.print(a.acceleration.y + a_cal.axis.y);
    Serial.print(", Z: ");
    Serial.print(a.acceleration.z + a_cal.axis.z);
    Serial.println(" m/s^2");

    Serial.print("Rotation X: ");
    Serial.print(g.gyro.x + g_cal.axis.x);
    Serial.print(", Y: ");
    Serial.print(g.gyro.y + g_cal.axis.y);
    Serial.print(", Z: ");
    Serial.print(g.gyro.z + g_cal.axis.z);
    Serial.println(" rad/s");

    Serial.print("Temperature: ");
    Serial.print(temp.temperature);
    Serial.println(" degC");

    Serial.println("");
    delay(500);
}

float IMU::rad2deg(float radians)
{
    return ((radians * 4068) / 71);
}

float IMU::mps2g(float mps)
{
    return (mps / 9.80665);
}

const FusionMatrix gyroscopeMisalignment = {1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f};
const FusionVector gyroscopeSensitivity = {1.0f, 1.0f, 1.0f};
const FusionMatrix accelerometerMisalignment = {1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f};
const FusionVector accelerometerSensitivity = {1.0f, 1.0f, 1.0f};

void IMU::loop()
{
    /* Get new sensor events with the readings */
    mpu.getEvent(&a, &g, &temp);

    // Serial.printf("gX %0.2f, gY %0.2f, gZ %0.2f\n", g.gyro.x, g.gyro.y, g.gyro.z);

    unsigned long now = millis();
    unsigned long timeElapsed = now - lastTime;
    lastTime = now;
    float elapsedSeconds = timeElapsed / 1000.0; // This should be close to SAMPLE_RATE

    float gx = rad2deg(g.gyro.x);
    float gy = rad2deg(g.gyro.y);
    float gz = rad2deg(g.gyro.z);

    float ax = mps2g(a.acceleration.x);
    float ay = mps2g(a.acceleration.y);
    float az = mps2g(a.acceleration.z);

    FusionVector gyroscope = {gx, gy, gz};     // actual gyroscope data in degrees/s
    FusionVector accelerometer = {ax, ay, az}; // actual accelerometer data in g

    // Update calibration data
    gyroscope = FusionCalibrationInertial(gyroscope, gyroscopeMisalignment, gyroscopeSensitivity, g_cal_deg);
    accelerometer = FusionCalibrationInertial(accelerometer, accelerometerMisalignment, accelerometerSensitivity, a_cal_g);

    // Update gyroscope offset correction algorithm
    gyroscope = FusionOffsetUpdate(&offset, gyroscope);

    FusionAhrsUpdateNoMagnetometer(&ahrs, gyroscope, accelerometer, elapsedSeconds);

    euler = FusionQuaternionToEuler(FusionAhrsGetQuaternion(&ahrs));

    // Serial.printf("Roll %0.2f, Pitch %0.2f, Yaw %0.2f, time %0.3fs\n", euler.angle.roll, euler.angle.pitch, euler.angle.yaw, elapsedSeconds);
    heading = euler.angle.yaw;

    delay(SAMPLE_RATE);
}

int IMU::getHeading()
{
    Serial.printf("Roll %0.2f, Pitch %0.2f, Yaw %0.2f\n", euler.angle.roll, euler.angle.pitch, euler.angle.yaw);
    return heading;
}

IMU::IMU()
{
}

IMU::~IMU()
{
}
