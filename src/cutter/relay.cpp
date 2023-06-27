
#include "cutter.h"

#ifdef CUTTER_RELAY

#include "Arduino.h"
 
Cutter::Cutter(int pin)
{
  this->pin = pin;
  pinMode(this->pin, OUTPUT);
  digitalWrite(this->pin, LOW);
}

void Cutter::init()
{
}

void Cutter::start()
{
  digitalWrite(this->pin, HIGH);
}

void Cutter::stop()
{
  digitalWrite(this->pin, LOW);
}
#endif
