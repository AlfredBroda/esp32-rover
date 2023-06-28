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
        int a1;
        int a2;
        int b1;
        int b2;

        MotorPreset() {
            a1 = 0;
            a2 = 0;
            b1 = 0;
            b2 = 0;
        }
        
        MotorPreset(int ia1, int ia2, int ib1, int ib2) {
            a1 = ia1;
            a2 = ia2;
            b1 = ib1;
            b2 = ib2;
        }
};

const MotorPreset M_FORWARD = {255, 0, 255, 0};
const MotorPreset M_BACKWARD = {0, 160, 0, 160};
const MotorPreset M_LEFT = {180, 0, 255, 0};
const MotorPreset M_LEFT_REVERSE = {0, 100, 0, 255};
const MotorPreset M_RIGHT = {255, 0, 180, 0};
const MotorPreset M_RIGHT_REVERSE = {0, 255, 0, 100};
const MotorPreset M_LEFT_SPOT = {0, 100, 100, 0};
const MotorPreset M_RIGHT_SPOT = {100, 0, 0, 100};
const MotorPreset M_STOP = {0, 0, 0, 0};

#endif // STRUCTS_H
