#ifndef CUTTER_H 
#define CUTTER_H

class Cutter
{
  public:
    Cutter(int pin);
    void init();
    void start();
    void stop();
  private:
    int pin;
};

#endif // CUTTER_H
