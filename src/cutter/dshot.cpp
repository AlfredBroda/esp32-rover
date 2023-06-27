
#include "cutter.h"

#ifdef CUTTER_DSHOT

#include <DShotESC.h>

// Initialize the motor
const auto DSHOT_MODE = DSHOT300;

DShotESC cutter_actual = DShotESC(15, RMT_CHANNEL_0);

Cutter::Cutter(int pin)
{
  this->pin = pin;
  DShotESC cutter_actual = DShotESC(this->pin, RMT_CHANNEL_0);
}

void Cutter::init()
{
  cutter_actual.begin(DSHOT_MODE);
  cutter_actual.sendThrottleValue(DSHOT_THROTTLE_MIN);
}

void Cutter::start()
{
  cutter_actual.sendThrottleValue(DSHOT_THROTTLE_MAX);
}

void Cutter::stop()
{
  cutter_actual.sendThrottleValue(DSHOT_THROTTLE_MIN);
}
#endif
