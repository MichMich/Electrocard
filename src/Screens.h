/*
 * Electrocard. - An OLED powered business card. 
 *
 * Designed an programmed by Michael Teeuw
 * For more info, check: http://michaelteeuw.nl/tagged/electrocard
 *
 */

#include <elapsedMillis.h>

class Screens
{
  private:
    void systemInfo();
  public:
    constexpr Screens() = default;
    void showScreen(unsigned char screen);
};


extern Screens Screens;
