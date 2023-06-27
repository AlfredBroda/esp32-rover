
#include "cutter.h"

#ifdef CUTTER_DSHOT

#include <DShotRMT.h>

// Initialize the motor
const auto DSHOT_MODE = DSHOT300;

DShotRMT cutter_actual = DShotRMT(15, RMT_CHANNEL_0);

Cutter::Cutter(int pin)
{
  this->pin = pin;
  DShotRMT cutter_actual = DShotRMT(this->pin, RMT_CHANNEL_0);
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
