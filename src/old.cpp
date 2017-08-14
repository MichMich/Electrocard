/*
void selfTest()
{
  TinyOLED.setpos(0, 0);
  TinyOLED.output_string((PINB & (1 << 0)) ? "0: TRUE " : "0: FALSE");

  TinyOLED.setpos(0, 1);
  TinyOLED.output_string((PINB & (1 << 1)) ? "1: TRUE " : "1: FALSE");

  TinyOLED.setpos(0, 2);
  TinyOLED.output_string((PINB & (1 << 2)) ? "2: TRUE " : "2: FALSE");

  TinyOLED.setpos(0, 3);
  char s[20];
  sprintf(s, "%lu", millis());
  TinyOLED.output_string(s);
}
*/

/*void showSystemDetails()
{
  TinyOLED.clear();
  TinyOLED.setpos(0, 0);
  TinyOLED.output_string("SW: ");
  TinyOLED.output_string(__DATE__);
  TinyOLED.output_string(" ");
  TinyOLED.output_string(__TIME__);

  TinyOLED.setpos(0, 1);
  TinyOLED.output_string("HW: Electrocard v0.1\n");

  char battery[10];
  sprintf(battery, "%u", sys.readVcc());
  TinyOLED.output_string("Battery: ");
  TinyOLED.output_string(battery);
  TinyOLED.output_string(" mV\n");

  char mem[10];
  sprintf(mem, "%u", sys.getFreeRAM());
  TinyOLED.output_string("Free RAM: ");
  TinyOLED.output_string(mem);
  TinyOLED.output_string(" bytes\n");
}
*/
