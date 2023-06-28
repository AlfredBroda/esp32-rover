#ifndef STRUCTS_H
#define STRUCTS_H

typedef enum {
    STOP,
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
} state_t;

class MotorPreset {
    public:
        bool a1;
        bool a2;
        bool b1;
        bool b2;

        MotorPreset() {
            a1 = 0;
            a2 = 0;
            b1 = 0;
            b2 = 0;
        }
        
        MotorPreset(bool ia1, bool ia2, bool ib1, bool ib2) {
            a1 = ia1;
            a2 = ia2;
            b1 = ib1;
            b2 = ib2;
        }
};

const MotorPreset M_FORWARD = {1, 0, 1, 0};
const MotorPreset M_BACKWARD = {0, 1, 0, 1};
const MotorPreset M_LEFT = {0, 0, 1, 0};
const MotorPreset M_RIGHT = {0, 1, 0, 0};
const MotorPreset M_LEFT_SPOT = {0, 1, 1, 0};
const MotorPreset M_RIGHT_SPOT = {1, 0, 0, 1};
const MotorPreset M_STOP = {0, 0, 0, 0};

#endif // STRUCTS_H
