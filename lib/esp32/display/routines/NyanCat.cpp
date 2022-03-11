#include <Arduino.h>
#include <FastLED.h>
#include "display/DisplayRoutine.h"

namespace display_routine {

#define CAT_FRAME_COUNT  3
#define CAT_FRAME_WIDTH 13
#define CAT_FRAME_HEIGHT 6

static const CRGB CAT_PALETTE[] = {
  CRGB(CRGB::SaddleBrown),
  CRGB(CRGB::White),
  CRGB(CRGB::DeepPink),
  CRGB(0x0)
};

static const CRGB WHITE = CRGB(CRGB::White).nscale8(92);
static const CRGB BKG_COLOR = CRGB(0x000010);

static const uint32_t CAT_FRAMES[3][78] = {
{
3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 
3, 3, 3, 0, 2, 2, 2, 2, 1, 2, 0, 3, 1, 
3, 3, 3, 0, 2, 2, 2, 2, 1, 1, 1, 1, 1, 
3, 1, 1, 0, 2, 2, 2, 2, 1, 1, 1, 1, 1, 
1, 3, 3, 0, 0, 0, 0, 0, 0, 1, 1, 1, 3, 
3, 3, 3, 1, 3, 1, 3, 3, 3, 1, 3, 1, 3
},
{
3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 
3, 3, 3, 0, 2, 2, 2, 1, 2, 2, 0, 1, 3, 
3, 3, 3, 0, 2, 2, 2, 1, 1, 1, 1, 1, 3, 
1, 1, 1, 0, 2, 2, 2, 1, 1, 1, 1, 1, 3, 
3, 3, 3, 0, 0, 0, 0, 0, 1, 1, 1, 3, 3, 
3, 3, 1, 3, 1, 3, 3, 3, 1, 3, 1, 3, 3
},
{
3, 3, 3, 0, 0, 0, 0, 1, 0, 0, 0, 1, 3, 
3, 3, 3, 0, 2, 2, 2, 1, 1, 1, 1, 1, 3, 
1, 1, 3, 0, 2, 2, 2, 1, 1, 1, 1, 1, 3, 
3, 1, 1, 0, 2, 2, 2, 2, 1, 1, 1, 3, 3, 
3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 
3, 3, 1, 3, 1, 3, 3, 3, 1, 3, 1, 3, 3
}
};

#define RB_FRAME_WIDTH  4
#define RB_FRAME_HEIGHT 6

static const CRGB RB_PALETTE[] = {
  CRGB(CRGB::DarkRed).nscale8(92),
  CRGB(CRGB::DarkOrange).nscale8(92),
  CRGB(CRGB::Yellow).nscale8(92),
  CRGB(CRGB::DarkGreen).nscale8(92),
  CRGB(CRGB::DarkBlue).nscale8(92),
  CRGB(CRGB::Purple).nscale8(92)
};

struct Star {
  int row;
  int col;
  int frame;
  int vel;

  void init(int width, int height) {
    row = random8(0, height);
    col = random8(0, width);
    vel = random8(1,3);
    frame = 0;
  }

  void update(int width, int height) {
    if (col - vel < -2) {
      row = random8(0, height);
      col = width - 1;
      vel = random8(1,4);
      frame = 0;
    } else {
      col -= vel;
      frame = (frame + random8(0,2)) % 3;
    }
  }
};

#define NUM_STARS 7 
static Star stars[NUM_STARS];

NyanCat::NyanCat(display::Display& display): 
  DisplayRoutine(display), 
  catFrame(0),
  catPosR(1),
  catPosC(-CAT_FRAME_WIDTH) {
}

void NyanCat::init() {
  catFrame = 0;
  catPosR = 1;
  catPosC = -CAT_FRAME_WIDTH;

  for (int idx = 0; idx < NUM_STARS; idx++) {
    stars[idx].init(display.width(), display.height());
  }
}

void drawPixel(display::Display& display, int r, int c, CRGB color) {
  if (r >= 0 && c >= 0 && r < display.height() && c < display.width()) {
    display(r,c) = color;
  }
}

void drawCat(display::Display& display, int frame, int r, int c) {
  for (auto row = 0; row < CAT_FRAME_HEIGHT; row++) {
    for (auto col = 0; col < CAT_FRAME_WIDTH; col++) {
      auto color = CAT_PALETTE[CAT_FRAMES[frame][row * CAT_FRAME_WIDTH + col]];
      if (color != CRGB(0)) {
        drawPixel(display, row + r, col + c, color);
      }
    }
  }
}

void drawRainbow(display::Display& display, int frame, int catPosR, int catPosC) {
  int segs = 1 + (catPosC - 1 + RB_FRAME_WIDTH) / RB_FRAME_WIDTH;
  for (int seg = 0; seg < segs; seg++) {
    int r = (seg & 1) == (frame & 1) ? 1 : 2;
    int c = (catPosC - 1) - (RB_FRAME_WIDTH * seg);
    for (int row = 0; row < RB_FRAME_HEIGHT; row++) {
      int pixr = row + r;
      for (int col = 0; col < RB_FRAME_WIDTH; col++) {
        int pixc = col + c;
        drawPixel(display, pixr, pixc, RB_PALETTE[row]);
      }
    }
  }
}

void drawStar(display::Display& display, const Star& star) {
  if (star.frame == 0) {
    drawPixel(display, star.row, star.col, WHITE);
  } else if (star.frame == 1) {
    drawPixel(display, star.row - 1, star.col, WHITE);
    drawPixel(display, star.row + 1, star.col, WHITE);
    drawPixel(display, star.row, star.col - 1, WHITE);
    drawPixel(display, star.row, star.col + 1, WHITE);
  } else {
    for (int row = star.row - 1; row <= star.row + 1; row++) {
      for (int col = star.col - 1; col <= star.col + 1; col++) {
        if (!(row == star.row && col == star.col)) {
          drawPixel(display, row, col, WHITE);
        }
      }
    }
  }
}

void NyanCat::step(const display::DisplayConfig& config) {
  for (auto idx = 0; idx < display.size(); idx++) {
    display[idx] = BKG_COLOR;
  }

  drawRainbow(display, catFrame, catPosR, catPosC);

  for (int idx = 0; idx < NUM_STARS; idx++) {
    drawStar(display, stars[idx]);
  }

  drawCat(display, catFrame, catPosR, catPosC);

  for (int idx = 0; idx < NUM_STARS; idx++) {
    stars[idx].update(display.width(), display.height());
  }

  catFrame = (catFrame + 1) % CAT_FRAME_COUNT;
  if (catPosC < 16) {
    catPosC += 1;
  }
  if (catFrame == 2) {
    catPosR = 0;
  } else {
    catPosR = 1;
  }
}

}