#pragma once

#include "SparkFun_Ublox_Arduino_Library.h"

class GPS
{
public:
    void init();
    void loop();

    long getLatitude();
    long getLongitude();
    long getAltitude();
};
