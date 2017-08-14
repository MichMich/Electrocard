/*
 * Electrocard. - An OLED powered business card. 
 *
 * Designed an programmed by Michael Teeuw
 * For more info, check: http://michaelteeuw.nl/tagged/electrocard
 *
 */

#include <elapsedMillis.h>

class Tetris
{
  private:

    unsigned int speed = 0;
    elapsedMillis gameTimer;
    bool needsUIUpdate = false;
    unsigned char playerOffsetX = 0;
    unsigned char playerOffsetY = 0;
    bool fastDrop = false;
    unsigned int playerMatrix = 0;
    unsigned char arenaMatrix [32] = {
      /*
      0b00000000,
      0b00000000,
      0b00000000,
      0b00000000,
      0b00000000,
      0b00000000,
      0b00000000,
      0b00000000,
      0b00000000,
      0b00000000,
      0b00000000,
      0b00000000,
      0b00000000,
      0b00000000,
      0b00000000,
      0b00000000,
      0b00000000,
      0b00000000,
      0b00000000,
      0b00000000,
      0b00000000,
      0b00000000,
      0b00000000,
      0b00000000,
      0b00000000,
      0b00000000,
      0b01000000,
      0b11100000,
      0b00110010,
      0b11110011,
      0b11111011,
      0b11111011,
      */
    };

    bool bitFromPlayerMatrix(unsigned char x, unsigned char y);
    bool bitFromArenaMatrix(unsigned char x, unsigned char y);
    bool checkCollision(unsigned char offsetX, unsigned char offsetY);
    unsigned char clearLines();
    void drawPage(unsigned char index);
    void dropBlock();
    void mergeMatrix();
    void moveBlock(bool left);
    void rotateBlock(bool clockWise);
    void rotateBlockCW();
    void rotateBlockCCW();
    void setRandomPlayerMatrix();
    void spliceLine(unsigned char line);
    void trimPlayerMatrix();
    void updateUI();

  public:
    unsigned int playerScore = 0;
    bool gameOver = false;

    Tetris();
    void buttonHold(unsigned char index);
    void buttonPressed(unsigned char index);
    void buttonRelease(unsigned char index);
    void start();
    bool update();
};
