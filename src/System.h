/*
 * Electrocard. - An OLED powered business card. 
 *
 * Designed an programmed by Michael Teeuw
 * For more info, check: http://michaelteeuw.nl/tagged/electrocard
 *
 */

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
