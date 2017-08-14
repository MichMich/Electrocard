#include "Screens.h"
#include "TinyOLED.h"
#include "img_xonay.h"
#include "System.h"

void Screens::systemInfo()
{
  TinyOLED.setpos(0, 0);
  TinyOLED.output_string("SW: ");
  TinyOLED.output_string(__DATE__);
  TinyOLED.output_string(" ");
  TinyOLED.output_string(__TIME__);

  TinyOLED.setpos(0, 1);
  TinyOLED.output_string("HW: Electrocard v0.1\n");

  char battery[10];
  sprintf(battery, "%lu", System.readVcc());
  TinyOLED.output_string("Battery: ");
  TinyOLED.output_string(battery);
  TinyOLED.output_string(" mV \n");

  char mem[10];
  sprintf(mem, "%u", System.getFreeRAM());
  TinyOLED.output_string("Free RAM: ");
  TinyOLED.output_string(mem);
  TinyOLED.output_string(" bytes \n");
}

void Screens::showScreen(unsigned char screen)
{
  switch (screen) {
    case 0:
      TinyOLED.draw_bmp(0, 0, img_width, img_height, img_c_bits);
    break;

    case 1:
      TinyOLED.draw_bmp(0, 0, img_width, img_height, img_b_bits);
    break;

    case 2:
      TinyOLED.draw_bmp(0, 0, img_width, img_height, img_a_bits);
    break;

    case 3:
        systemInfo();
        delay(20);
    break;
  }
}
