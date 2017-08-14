
#include "Tetris.h"
#include "TinyOLED.h"
#include <stdlib.h>

Tetris::Tetris()
{
  start();
}

/** Private Methods **/

bool Tetris::bitFromArenaMatrix(unsigned char x, unsigned char y)
{
  return (arenaMatrix[y] >> x) & 1;
}

bool Tetris::bitFromPlayerMatrix(unsigned char x, unsigned char y)
{
  if (x >= 4 || y >= 4) return false;

  unsigned char lookup = (y < 2) ? playerMatrix >> 8 & 0xFF : playerMatrix & 0xFF;
  if (y % 2 == 0) lookup = lookup >> 4;

  return (lookup >> x) & 1;
}

bool Tetris::checkCollision(unsigned char offsetX, unsigned char offsetY)
{
    for (unsigned char y = 0; y < 4; y++) {
      for (unsigned char x = 0; x < 4; x++) {
        if (bitFromPlayerMatrix(x, y) && (bitFromArenaMatrix(x+offsetX, y+offsetY) || x+offsetX >= 8 || y+offsetY >= 32)) {
          return true;
        }
      }
    }

    return false;
}

unsigned char Tetris::clearLines()
{
  unsigned char removedLines = 0;
  unsigned char line = 31;
  do {
    if (arenaMatrix[line] == 0xFF) {
      spliceLine(line);
      removedLines++;
    } else {
      line--;
    }
  } while(line > 0);

  return removedLines;
}

void Tetris::drawPage(unsigned char index)
{

  for (unsigned char y = 0; y < 32; y++) {
    unsigned char page = 0;

    for (unsigned char x = 0; x < 2; x++) {
      unsigned char bitX = x + index * 2 - playerOffsetX;
      unsigned char bitY = y - playerOffsetY;

      if(bitFromPlayerMatrix(bitX, bitY) || bitFromArenaMatrix(x + index * 2, y)) {
        unsigned char mask = 0b00001111;
        mask = mask << x*4;
        page ^= mask;
      }


    }

    TinyOLED.send_byte(page);
    TinyOLED.send_byte(page);
    TinyOLED.send_byte(page);
    TinyOLED.send_byte(page);
  }

}

void Tetris::dropBlock()
{
  if (checkCollision(playerOffsetX, playerOffsetY + 1)) {
    if (playerOffsetY == 0) {
      gameOver = true;
    }

    mergeMatrix();
    setRandomPlayerMatrix();
    playerOffsetY = 0;
    playerOffsetX = 3;
    fastDrop = false;
  } else {
    playerOffsetY++;
  }

  needsUIUpdate = true;
}

void Tetris::moveBlock(bool left)
{
  unsigned char newOffsetX = (left) ? playerOffsetX + 1 : playerOffsetX - 1;
  if (!checkCollision(newOffsetX, playerOffsetY)) {
    playerOffsetX = newOffsetX;
    needsUIUpdate = true;
  }
}

void Tetris::rotateBlock(bool clockWise)
{
  if (clockWise) {
    rotateBlockCW();
    if (checkCollision(playerOffsetX, playerOffsetY)) rotateBlockCCW();
  } else {
    rotateBlockCCW();
    if (checkCollision(playerOffsetX, playerOffsetY)) rotateBlockCW();
  }
}

void Tetris::rotateBlockCCW()
{
  rotateBlockCW();
  rotateBlockCW();
  rotateBlockCW();
}

void Tetris::rotateBlockCW()
{
  unsigned int newPlayerMatrix = 0;

  for (unsigned char y = 0; y < 4; y++) {
    for (unsigned char x = 0; x < 4; x++) {
      unsigned char bitIndex = y * 4 + x;
      if (bitFromPlayerMatrix(y,x)) {
          int mask = 1 << bitIndex;
          newPlayerMatrix ^= mask;
      }
    }
  }

  playerMatrix = newPlayerMatrix;

  trimPlayerMatrix();
}

void Tetris::mergeMatrix()
{
  for (unsigned char y = 0; y < 4; y++) {
    for (unsigned char x = 0; x < 4; x++) {
      if (bitFromPlayerMatrix(x, y)) {
        unsigned char stamp = 1 << (x + playerOffsetX);
        arenaMatrix[y + playerOffsetY] ^= stamp;
      }
    }
  }
}

void Tetris::setRandomPlayerMatrix()
{
  // Should this be moved to progmem?
  const unsigned int matrices[7] = {
    0b00010001 << 8 | 0b00010001, // I
    0b00000001 << 8 | 0b00010011, // J
    0b00001000 << 8 | 0b10001100, // L
    0b00110011 << 8 | 0b00000000, // O
    0b00110110 << 8 | 0b00000000, // S
    0b01110010 << 8 | 0b00000000, // T
    0b11000110 << 8 | 0b00000000  // Z
  };

  // playerMatrix = pgm_read_word_near(matrices + rand() % 7);
  randomSeed(millis());
  playerMatrix = matrices[random(0, 6)];

  trimPlayerMatrix();
}

void Tetris::spliceLine(unsigned char line)
{
  if (line > 0) {
    for (unsigned char l = line - 1; l > 0; l--) {
      arenaMatrix[l + 1] = arenaMatrix[l];
    }
  }
  arenaMatrix[0] = 0;

  needsUIUpdate = true;
}

void Tetris::trimPlayerMatrix()
{
  unsigned int mask;

  // Trim up
  mask = 0b11110000 << 8;
  while ((playerMatrix & mask) == 0) {
    playerMatrix <<= 4;
  }

  // Trim right
  mask = 0b00010001 << 8 | 0b00010001;
  while ((playerMatrix & mask) == 0) {
    playerMatrix >>= 1;
  }

  needsUIUpdate = true;
}

void Tetris::updateUI()
{
  if (!needsUIUpdate) return;
  // A bit ugly, but we need to run the update 2.
  // If we don't every second update is skipped.
  // I need to look into the reason.
  // For now, this works.
  for (unsigned char i = 0; i < 2; i++) {
    TinyOLED.setpos(0, 0);

    TinyOLED.send_data_start();
    for (unsigned char sprite = 0; sprite < 4; sprite ++) {
      drawPage(sprite);
    }
    TinyOLED.send_data_stop();
  }

  needsUIUpdate = false;
}

/** Public Methods **/

void Tetris::buttonHold(unsigned char index)
{
  switch (index) {
    case 0:
      fastDrop = true;
      break;
    case 1:
      //moveBlock(true);
      break;
    case 2:
      //moveBlock(false);
      break;
  }
}

void Tetris::buttonPressed(unsigned char index)
{
  switch (index) {
    case 0:
      rotateBlock(true);
      break;
    case 1:
      moveBlock(true);
      break;
    case 2:
      moveBlock(false);
      break;
  }
}

void Tetris::buttonRelease(unsigned char index)
{
  switch (index) {
    case 0:
      fastDrop = false;
      break;
    case 1:
      //moveBlock(true);
      break;
    case 2:
      //moveBlock(false);
      break;
  }
}

void Tetris::start()
{
  gameOver = false;
  speed = 300;
  playerOffsetY = 0;
  playerOffsetX = 3;
  playerScore = 0;
  fastDrop = false;
  memset(arenaMatrix, 0, 32);
  setRandomPlayerMatrix();
  TinyOLED.clear();
  needsUIUpdate = true;
}

bool Tetris::update()
{
  if (gameTimer > ((fastDrop) ? 5 : speed)) {
    unsigned char scoreDelta = clearLines();
    playerScore += scoreDelta * scoreDelta * 10;
    speed -= (scoreDelta * 10);

    dropBlock();

    gameTimer = 0;
  }

  updateUI();

  return gameOver;
}
