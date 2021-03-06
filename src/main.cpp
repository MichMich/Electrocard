/*
 * Electrocard. - An OLED powered business card.
 *
 * Designed an programmed by Michael Teeuw
 * For more info, check: http://michaelteeuw.nl/tagged/electrocard
 *
 */

#include <Arduino.h>
#include <util/delay.h>

#include "TinyOLED.h"

#include "Tetris.h"
#include "System.h"
#include "clickButton.h"
#include <elapsedMillis.h>
#include "Screens.h"

#define STANDBY_TIME 30000
#define STANDBY_TIME_LONG 60000

typedef enum {SCREENS, TETRIS, SYSTEM_INFO} mode_type;
mode_type mode = SCREENS;

Tetris tetris = Tetris();
elapsedMillis sleepTimer;
ClickButton buttons[3] = {
  ClickButton(0, LOW, CLICKBTN_PULLUP),
  ClickButton(1, LOW, CLICKBTN_PULLUP),
  ClickButton(2, LOW, CLICKBTN_PULLUP)
};

void setup()
{
  _delay_ms(100);

  // pinMode(0, INPUT_PULLUP);
  // pinMode(1, INPUT_PULLUP);
  // pinMode(2, INPUT_PULLUP);
  DDRB = 0x00;                //PC as input
  PORTB |= 0b00000111;

  TinyOLED.init();
	TinyOLED.setfont(bitmap_font_small_6bit);

  for (byte i = 0; i < 3; i++) {
    buttons[i].debounceTime   = 20;   // Debounce timer in ms
    buttons[i].multiclickTime = 1;  // Time limit for multi clicks
    buttons[i].longClickTime  = 500; // time until "held-down clicks" register
  }

  // Begin with start image
  mode = SCREENS;
  Screens.showScreen(2);
}

void sleep() {
  System.sleep();
  sleepTimer = 0;
  mode = SCREENS;
  Screens.showScreen(2);
}

void loop()
{
  // Update all buttons
  for (byte i = 0; i < 3; i++) {
    buttons[i].Update();

    if (buttons[i].clicks > 0 && (mode != TETRIS || tetris.gameOver)) {
      mode = SCREENS;
      Screens.showScreen(i);
    }

    if (mode == TETRIS) {
      if (buttons[i].clicks > 0) tetris.buttonPressed(i);
      if (buttons[i].clicks < 0) tetris.buttonHold(i);
      if (buttons[i].released) tetris.buttonRelease(i);
    }
  }

  if (buttons[0].clicks < 0 && (mode != TETRIS || tetris.gameOver)) {
    // Set the longpress for button 2 longer to prevent accidental exiting.
    buttons[2].longClickTime  = 2000;
    tetris.start();
    mode = TETRIS;
  }

  if (buttons[1].clicks < 0 && (mode != TETRIS || tetris.gameOver)) {
    TinyOLED.clear();
    mode = SYSTEM_INFO;
  }

  // If we aren't running Tetris, and button 2 is long pressed, go to virtual standby mode.
  if (buttons[2].clicks < 0 && (mode != TETRIS || tetris.gameOver)) {
    //Pretend we go off by clearing the display.
    mode = SCREENS;
    TinyOLED.clear();

    /**
     * Let's just pretend we go to sleep, and let the auto power-off do the rest.
     * This way we won't get any interference from the button being pressed when going to sleep.
     * *

      // Debounce: wait until the button is released for approx 250ms.
      unsigned char debounce = 0;
      while (debounce < 250) {
        debounce++;
        for (int i = 0; i < 2500; i++) {
          if (!(PINB & (1 << 2))) debounce = 0;
        }
      }

      //Good to go! Go to sleep.
      sleep();
    */
  }

  // If we ARE running Tetris, and button 2 is long pressed, exit the game by running startup again.
  if (buttons[2].clicks < 0 && !(mode != TETRIS || tetris.gameOver)) {
    setup();
  }

  switch (mode) {
    case TETRIS:
      if (!tetris.gameOver) {
        tetris.update();
      } else {
        TinyOLED.setpos(0, 0);
        char scoreText[30];
        sprintf(scoreText, "GAME OVER!\nScore: %d", tetris.playerScore);
        TinyOLED.output_string(scoreText);
      }
    break;
    case SYSTEM_INFO:
      Screens.showScreen(3);
    break;
    default:

    break;
  }

  // Sleep timer reset
  if (buttons[0].clicks != 0 || buttons[1].clicks != 0 || buttons[2].clicks != 0) {
    sleepTimer = 0;
  }
  // Sleep timer initiate
  if (sleepTimer >= ((mode != TETRIS) ? STANDBY_TIME : STANDBY_TIME_LONG)) {
    sleep();
  }

}
