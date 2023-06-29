#ifndef STRUCTS_H
#define STRUCTS_H

typedef enum
{
    STOP,
    FORWARD,
    FORWARD_LEFT,
    FORWARD_RIGHT,
    BACKWARD,
    LEFT,
    RIGHT
} state_t;

typedef enum {
    MS_STOP,
    MS_FORWARD,
    MS_BACKWARD,
    MS_LEFT,
    MS_LEFT_ARC,
    MS_LEFT_SPOT,
    MS_LEFT_REVERSE,
    MS_RIGHT,
    MS_RIGHT_ARC,
    MS_RIGHT_SPOT,
    MS_RIGHT_REVERSE
} motor_state_t;

class MotorPreset
{
public:
    int a1, a2, b1, b2;
    MotorPreset()
    {
        a1 = 0;
        a2 = 0;
        b1 = 0;
        b2 = 0;
    }
    MotorPreset(int ia1, int ia2, int ib1, int ib2, motor_state_t type)
    {
        a1 = ia1;
        a2 = ia2;
        b1 = ib1;
        b2 = ib2;
        this->type = type;
    }
    motor_state_t getType();
private:
    motor_state_t type;
};

const int SPD_H = 230;
const int SPD_M = 180;
const int SPD_L = 140;
const int SPD_0 = 0;

const MotorPreset M_FORWARD = {SPD_H, SPD_0, SPD_H, SPD_0, MS_FORWARD};

const MotorPreset M_LEFT_FW = {SPD_L, SPD_0, SPD_H, SPD_0, MS_LEFT};
const MotorPreset M_LFT_ARC = {SPD_0, SPD_L, SPD_M, SPD_0, MS_LEFT_ARC};
const MotorPreset M_LFT_SPT = {SPD_0, SPD_L, SPD_L, SPD_0, MS_LEFT_SPOT};

const MotorPreset M_RGHT_FW = {SPD_H, SPD_0, SPD_L, SPD_0, MS_RIGHT};
const MotorPreset M_RGT_ARC = {SPD_M, SPD_0, SPD_0, SPD_L, MS_RIGHT_ARC};
const MotorPreset M_RGT_SPT = {SPD_L, SPD_0, SPD_0, SPD_L, MS_RIGHT_SPOT};

const MotorPreset M_REVERSE = {SPD_0, SPD_H, SPD_0, SPD_H, MS_BACKWARD};
const MotorPreset M_LFT_REV = {SPD_0, SPD_L, SPD_0, SPD_H, MS_LEFT_REVERSE};
const MotorPreset M_RGT_REV = {SPD_0, SPD_H, SPD_0, SPD_L, MS_RIGHT_REVERSE};

const MotorPreset M_STOP = {SPD_0, SPD_0, SPD_0, SPD_0, MS_STOP};

#endif // STRUCTS_H
