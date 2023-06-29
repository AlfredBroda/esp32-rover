#include "tracker.h"

void Tracker::followHeading()
{
    const int currentHeading = imu.getHeading();
    int deviation = (360 + currentHeading) - (heading + 360);
    if (deviation > maxDeviation)
    {
        if (currentState == FORWARD)
        {
            if (deviation > 0)
            {
                if (currentPreset.getType() != MS_RIGHT)
                {
                    currentAction = "Turn Right " + String(heading);
                    setMotorState(M_RGHT_FW);
                }
            }
            else
            {
                if (currentPreset.getType() != MS_LEFT)
                {
                    currentAction = "Turn Left " + String(heading);
                    setMotorState(M_LEFT_FW);
                }
            }
        }
        else if (currentState == BACKWARD)
        {
            if (deviation > 0)
            {
                if (currentPreset.getType() != MS_LEFT_REVERSE)
                {
                    currentAction = "Rev Left " + String(heading);
                    setMotorState(M_LFT_REV);
                }
            }
            else
            {
                if (currentPreset.getType() != MS_RIGHT_REVERSE)
                {
                    currentAction = "Rev Right " + String(heading);
                    setMotorState(M_RGT_REV);
                }
            }
        }
    }
    else
    {
        if ((currentState == FORWARD) && (currentPreset.getType() != MS_FORWARD))
        {
            currentAction = "Straight " + String(heading);
            setMotorState(M_FORWARD);
        }
        else if ((currentState == BACKWARD) && (currentPreset.getType() != MS_BACKWARD))
        {
            currentAction = "Rev Straight " + String(heading);
            setMotorState(M_REVERSE);
        }
    }
}

String Tracker::getCurrentAction()
{
    return currentAction;
}

void Tracker::saveHeading()
{
    heading = imu.getHeading();
}

int Tracker::getActualHeading()
{
    return imu.getHeading();
}

void Tracker::setHeading(int newHeading)
{
    heading = newHeading;
}

void Tracker::emergencyStop()
{
    Serial.println("Emergency STOP");
    setMotorState(M_STOP);
    currentState = STOP;
    currentAction = "STOP";

    // recalibrate IMU
    delay(5000);
    imu.mpuCalibrate();
}

void Tracker::setMaxDeviation(int newMaxDeviation)
{
    maxDeviation = newMaxDeviation;
}

void Tracker::setState(state_t newState)
{
    currentState = newState;
    switch (newState)
    {
    case FORWARD:
        saveHeading();
        setMotorState(M_FORWARD);
        currentAction = "Forward";
        break;
    case BACKWARD:
        saveHeading();
        setMotorState(M_REVERSE);
        currentAction = "Reverse";
        break;
    case LEFT:
        setMotorState(M_LFT_SPT);
        currentAction = "Pivot Left";
        break;
    case RIGHT:
        setMotorState(M_RGT_SPT);
        currentAction = "Pivot Right";
        break;
    case FORWARD_LEFT:
        setMotorState(M_LFT_ARC);
        currentAction = "Arc Left";
        break;
    case FORWARD_RIGHT:
        setMotorState(M_RGT_ARC);
        currentAction = "Arc Right";
        break;
    case STOP:
        setMotorState(M_STOP);
        currentAction = "Stop";
        break;
    }
}

state_t Tracker::getState()
{
    return currentState;
}

MotorPreset Tracker::getCurrentPreset()
{
    return currentPreset;
}

int Tracker::getHeading()
{
    return heading;
}

void Tracker::setMotorState(MotorPreset preset)
{
    currentPreset = preset;
    analogWrite(ma1, preset.a1);
    analogWrite(ma2, preset.a2);
    analogWrite(mb1, preset.b1);
    analogWrite(mb2, preset.b2);
}

Tracker::Tracker(int pin1, int pin2, int pin3, int pin4)
{
    heading = 0;
    maxDeviation = 5;
    currentState = STOP;
    currentPreset = M_STOP;

    ma1 = pin1;
    ma2 = pin2;
    mb1 = pin3;
    mb2 = pin4;
}

void Tracker::init()
{
    imu.init();

    saveHeading();
    setMotorState(M_STOP);
    currentAction = "Init";
}

void Tracker::loop()
{
    imu.loop();

    if (currentState == FORWARD || currentState == BACKWARD)
    {
        followHeading();
    } else if (currentState == FORWARD_LEFT) {
        setMotorState(currentPreset);
    }
}

String Tracker::getStateString()
{
    switch (currentState)
    {
    case FORWARD:
        return "FORWARD";
    case BACKWARD:
        return "BACKWARD";
    case LEFT:
        return "LEFT";
    case RIGHT:
        return "RIGHT";
    case STOP:
        return "STOP";
    }
    return "UNKNOWN";
}
