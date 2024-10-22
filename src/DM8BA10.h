/*
  Created by Ilya 'road_t' Annikov 16.01.2022
  Repository URL: https://github.com/road-t/DM8BA10
*/

#ifndef ROADT_DM8BA10_H
#define ROADT_DM8BA10_H

#ifdef ESP_IDF
#include "esp_idf.h"
#else
#include <Arduino.h>
#endif

/*
Each bit of a 2-byte word represents particular segment:

    /-----------\   /-----------\
   ||     F     || ||     7     ||
    \-----------/   \-----------/
   /-\ /--\      /-\      /--\ /-\
  |   |\   \    |   |    /   /|   |
  |   | \   \   |   |   /   / |   |
  | D |  \ E \  | 6 |  / 4 /  | 5 |
  |   |   \   \ |   | /   /   |   |
  |   |    \   \|   |/   /    |   |
   \-/      \--/ \-/ \--/      \-/
    /-----------\   /-----------\
   ||     C    || ||     3     ||
    \-----------/   \-----------/
   /-\      /--\ /-\ /--\      /-\
  |   |    /   /|   |\   \    |   |
  |   |   /   / |   | \   \   |   |
  | 9 |  / B /  | A |  \ 2 \  | 1 |
  |   | /   /   |   |   \   \ |   |
  |   |/   /    |   |    \   \|   |
   \-/ \--/      \-/      \--/ \-/
    /-----------\   /-----------\
   ||     8     || ||     0     ||
    \-----------/   \-----------/


 Elements addressing:

 |------------------------|
 | addr | bits |  element |
 |------|------|----------|
 | 0x00 | 0-15 | symbol 0 |
 | 0x04 | 0-15 | symbol 1 |
 | 0x08 | 0-15 | symbol 2 |
 | 0x0C | 0-15 | symbol 3 |
 | 0x10 | 0-15 | symbol 4 |
 | 0x14 | 0-15 | symbol 5 |
 | 0x18 | 0-15 | symbol 6 |
 | 0x1C | 0-15 | symbol 7 |
 | 0x20 | 0-15 | symbol 8 |
 | 0x24 | 0-15 | symbol 9 |
 |------|------|----------|
 | 0x29 |   2  | point 8  |
 | 0x29 |   1  | point 7  |
 | 0x29 |   0  | point 6  |
 | 0x2B |   2  | point 5  |
 | 0x2B |   1  | point 4  |
 | 0x2B |   0  | point 3  |
 | 0x2D |   2  | point 2  |
 | 0x2D |   1  | point 1  |
 | 0x2D |   0  | point 0  |
 |------------------------|
*/

#include <charset/charset.h>

// Communication prefixes
#define BIT_PREFIX_CMD  0b100 // HT162x commands (Start with 0b100, ends with an unused byte
#define BIT_PREFIX_DATA 0b101

// Command codes
#define  CMD_OSC_OFF  0x00  // SYS DIS    (0000-0000-X) Turn off system oscillator, LCD bias gen [default]
#define  CMD_OSC_ON   0x01  // SYS EN     (0000-0001-X) Turn on  system oscillator
#define  CMD_LCD_OFF  0x02  // LCD OFF    (0000-0010-X) Turn off LCD display [default]
#define  CMD_LCD_ON   0x03  // LCD ON     (0000-0011-X) Turn on  LCD display

// Place addresses
const byte SYMBOL_ADDRESSES[] = { 0x24, 0x20, 0x1C, 0x18, 0x14, 0x10, 0x0C, 0x08, 0x04, 0x00 };
const byte POINTS_CLUSTERS[] = { 0x29, 0x2B, 0x2D };

// Misc
#define WRITE_DELAY   2  // microseconds – Tclk min. on data sheet - overhead is more than this at low clock speeds

class DM8BA10
{
  public:
    DM8BA10(Charset* charset, int8_t csPin, int8_t wrPin, int8_t dataPin, int8_t backlightPin = -1);
    ~DM8BA10();

    // Turns all the segments off and resets current position
    void clearDisplay();

    // Sets current position
    // 0 – most left place, -1 – most right place
    byte setPos(int8_t);

    void systemOscillator(bool on = true);
    void LCD(bool on = true);
    void backlight(bool on = true);

    // Switches all the segments on or off
    void allSegments(bool on = true);

    // Draws anything by filling segments, specified
    // by bits in ch parameter at pos
    void drawChar(word ch, byte pos);

    // Displays specified character at specified position
    // or at current position if pos == -1.
    // Doesn't affect the current position
    void setChar(const byte value, int8_t pos = -1);

    // Padding types
    enum Padding { Right, Left, Both };

    // Fills all places on display and resets current position
    // This function is slower than print() and consumes
    // extra memory.
    // Padding is off by default though
    void println(String str, Padding podType = Right);

    // Just prints the string starting from specified
    // (or current position if pos == -1)
    // to the end of the string or the indicator.
    // It doesn't overwrite places outside of
    // current string and can cause artifacts.
    // current position is being reset only if the display is full
    byte print(String str, int8_t pos = -1);

    // Displays string starting at `start` and adds a space
    // to the end of string
    void scroll(String text, word start = 0);

    // Decimal points aren't part of common symbols
    // so this function turns them on and off
    // by its index from left to right
    void point(byte index, bool on = true);

    // Displays specified decimal point, turning
    // off all others
    void setPoint(byte index);

    // Turns off all the decimal points
    void resetPoints();

    // Returns string padded with spaces to fill 
    // all the places on the display
    String padString(String text, Padding padType);

    // Returns Charset object to access its properties or methods
    inline Charset* getCharset() { return charset; }

  private:
    Charset* charset;

    int8_t csPin = -1;
    int8_t wrPin = -1;
    int8_t dataPin = -1;
    int8_t backlightPin = -1;
    
    const byte* places = SYMBOL_ADDRESSES;
    const byte displaySize = sizeof(SYMBOL_ADDRESSES);

    const byte* pointClusters = POINTS_CLUSTERS;
    const byte pointClustersCount = sizeof(POINTS_CLUSTERS);
  
    byte curPos = 0;
    bool backlightOn;

    void sendCommand(unsigned char CMD);
    void sendBits(word data, byte bits);
    void sendData(byte addr, word sdata, byte bits = 16);
    
    inline void beginTransfer();
    inline void endTransfer();
};

#endif //ROADT_DM8BA10_H
