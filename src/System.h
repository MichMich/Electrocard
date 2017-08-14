#include <elapsedMillis.h>

class System
{
  private:
    void sleepCPU();

  public:
    constexpr System() = default;
    void sleep();
    long readVcc();
    int getFreeRAM();
};

extern System System;
