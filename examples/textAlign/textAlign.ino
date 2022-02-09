/*
 * Simple example of text alignment (by padding)
 */

#include <DM8BA10.h>
#include <charset/latin_basic.h>

// pins
#define CS_PIN        9
#define WR_PIN        8
#define DATA_PIN      7
#define BACKLIGHT_PIN 6

// display settings
#define PLACES_COUNT 10
#define REFRESH_RATE 1000 // display update speed in ms

DM8BA10* LCD;

const DM8BA10::Padding alignment[] = { DM8BA10::Padding::Right, DM8BA10::Padding::Left, DM8BA10::Padding::Both };
volatile byte curAlignmentIndex = 0;
const String texts[] = { "Left", "Right", "Center" };

void setup()
{   
  LCD = new DM8BA10(new LatinBasicCharset(), CS_PIN, WR_PIN, DATA_PIN, BACKLIGHT_PIN);
  LCD->backlight();
}

void loop()
{
  static uint32_t lastUpd = 0;

  auto nowMs = millis();

  if (nowMs - lastUpd > REFRESH_RATE)
  {
    LCD->println(texts[curAlignmentIndex], alignment[curAlignmentIndex]);

    if (curAlignmentIndex++ >= 2)
      curAlignmentIndex = 0;

    lastUpd = nowMs;
  }
}
