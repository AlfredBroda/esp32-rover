#ifndef STRUCTS_H
#define STRUCTS_H

typedef enum
{
    STOP,
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
} state_t;

typedef enum {
    MS_STOP,
    MS_FORWARD,
    MS_BACKWARD,
    MS_LEFT,
    MS_LEFT_SPOT,
    MS_LEFT_REVERSE,
    MS_RIGHT,
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

const MotorPreset M_FORWARD = {255, 0, 255, 0, MS_FORWARD};
const MotorPreset M_BACKWARD = {0, 160, 0, 160, MS_BACKWARD};
const MotorPreset M_LEFT = {180, 0, 255, 0, MS_LEFT};
const MotorPreset M_LEFT_REVERSE = {0, 100, 0, 255, MS_LEFT_REVERSE};
const MotorPreset M_RIGHT = {255, 0, 180, 0, MS_RIGHT};
const MotorPreset M_RIGHT_REVERSE = {0, 255, 0, 100, MS_RIGHT_REVERSE};
const MotorPreset M_LEFT_SPOT = {0, 100, 100, 0, MS_LEFT_SPOT};
const MotorPreset M_RIGHT_SPOT = {100, 0, 0, 100, MS_RIGHT_SPOT};
const MotorPreset M_STOP = {0, 0, 0, 0, MS_STOP};

#endif // STRUCTS_H
