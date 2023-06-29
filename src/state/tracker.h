#pragma once

#include "structs.h"
#include "sensors/imu.h"
#include "cutter/cutter.h"

struct Tracker
{
private:
    int heading;
    int maxDeviation;
    state_t currentState;
    MotorPreset currentPreset;
    IMU imu;
    void setMotorSpeed(int a1, int a2, int b1, int b2);
    int ma1, ma2, mb1, mb2;
    void followHeading();
    String currentAction;
public:
    Tracker(int, int, int, int);
    void init();
    void loop();

    void setState(state_t newState);
    state_t getState();
    String getStateString();
    String getCurrentAction();

    void setMotorState(MotorPreset preset);
    MotorPreset getCurrentPreset();

    void saveHeading();
    void setHeading(int newHeading);
    int getHeading();
    int getActualHeading();

    void emergencyStop();
    void setMaxDeviation(int newMaxDeviation);
};
