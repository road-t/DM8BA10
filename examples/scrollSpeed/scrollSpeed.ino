/* 
* Here you can see a way to control text scrolling
* speed (actually display refresh rate) with potentiometer.
*/

#include <DM8BA10.h>
#include <charset/latin_basic.h>

// specify pins for clock and data wires
#define CS_PIN            9
#define WR_PIN            8
#define DATA_PIN          7
#define BACKLIGHT_PIN     6 // or connect backlight pin to VCC (+5V)
#define POTENTIOMETER_PIN A0

// display settings
#define PLACES_COUNT      10
#define REFRESH_RATE      200 // display update speed in ms

// serial settings
#define SERIAL_SPEED      9600
#define SPEED_TEXT      "SPEED: "

DM8BA10* LCD;

word RefreshRate = REFRESH_RATE; // current refresh rate
String text = DEFAULT_TEXT + String(RefreshRate);

void setup()
{
  Serial.begin(SERIAL_SPEED);
  
  LCD = new DM8BA10(new LatinBasicCharset(), CS_PIN, WR_PIN, DATA_PIN, BACKLIGHT_PIN);
  LCD->backlight();

  text = LCD->getCharset()->remap(text);
}


void loop()
{
  static word strPos =  0;
  static uint32_t lastUpd = 0, lastRead = 0;;

  auto nowMs = millis();

  // Display text
  if (nowMs - lastUpd > RefreshRate)
  {
    LCD->scroll(text, strPos++);
    
    if (strPos == text.length())
    {
      strPos = 0;
    }

    lastUpd = nowMs;
  }

  // don't poll too frequently
  if (nowMs - lastRead > 100)
  {
    auto pot = analogRead(POTENTIOMETER_PIN);

    RefreshRate = (pot + 1) * 2; // analog reads gives 10-bit value 0..1023, so we'll have range 2..2048ms

    text = SPEED_TEXT + String(RefreshRate);
    
    lastRead = nowMs;
  }
}
