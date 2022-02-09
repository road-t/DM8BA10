/* 
* This example demonstrates displaying
* a string, received from the serial port.
* If the string is longer than 10 symbols
* it will be scrolled.
*/

#include <DM8BA10.h>
#include <charset/cyrillic_slavic.h>

// specify pins for clock and data wires
#define CS_PIN          9
#define WR_PIN          8
#define DATA_PIN        7
#define BACKLIGHT_PIN   6 // or connect backlight pin to VCC (+5V)

// display settings
#define PLACES_COUNT    10
#define REFRESH_RATE    200 // display update speed in ms

// serial settings
#define SERIAL_SPEED    9600
#define DEFAULT_TEXT    "DMA8BA10"

DM8BA10* LCD;

String text = DEFAULT_TEXT;

void setup()
{
  Serial.begin(SERIAL_SPEED);
  
  LCD = new DM8BA10(new CyrillicSlavicCharset(), CS_PIN, WR_PIN, DATA_PIN, BACKLIGHT_PIN);
  LCD->backlight();

  text = LCD->getCharset()->remap(text);
}


void loop()
{
  static word strPos =  0;
  static uint32_t lastUpd = 0;

  auto nowMs = millis();

  // Display text
  if (nowMs - lastUpd > REFRESH_RATE)
  {
    // scroll text if the string is longer than display capacity
    if (text.length() > PLACES_COUNT)
    {
      LCD->scroll(text, strPos++);
     
      if (strPos == text.length())
      {
        strPos = 0;
      }
    }
    else
    {
      LCD->println(text);
    }

    lastUpd = nowMs;
  }

  // Read string from serial port
  auto inputStr = Serial.readString();

  if (inputStr.length())
  {
    text = LCD->getCharset()->remap(inputStr);
    
    strPos = 0;
  }
}
