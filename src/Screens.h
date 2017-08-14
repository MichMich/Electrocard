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
