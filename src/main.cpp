/*
  Sketch to show creation of a sprite with a transparent
  background, then plot it on the TFT.

  Example for library:
  https://github.com/Bodmer/TFT_eSPI

  A Sprite is notionally an invisible graphics screen that is
  kept in the processors RAM. Graphics can be drawn into the
  Sprite just as it can be drawn directly to the screen. Once
  the Sprite is completed it can be plotted onto the screen in
  any position. If there is sufficient RAM then the Sprite can
  be the same size as the screen and used as a frame buffer.

  A 16-bit Sprite occupies (2 * width * height) bytes in RAM.

  On a ESP8266 Sprite sizes up to 126 x 160 can be accommodated,
  this size requires 40kBytes of RAM for a 16-bit colour depth.

  When 8-bit colour depth sprites are created they occupy
  (width * height) bytes in RAM, so larger sprites can be
  created, or the RAM required is halved.
*/

#include <hardwareDef.h>
#include <Arduino.h>
#include "displayCtl.h"



int i = 10;



void setup(void)
{
  Serial.begin(250000);

  // Init the display
  initDisplay();



  delay(3000);

}

void loop()
{

  // needle.fillSprite(TFT_RED);
  // needle.fillSprite(0x041f);
  uint32_t dt = millis();

  if (i > 100)
  {
    background.pushImage(0, 0, 240, 240, _scale_2);
  }
  else
  {
    background.pushImage(0, 0, 240, 240, _scale_1);
  }

  showCoolantTemperature(i);
  showEngineSpeed(i * 29);
  showNeedle(i * 30);
  showEngineHours(i * 23.1);
  // showText(i * 21);

  background.pushSprite(0, 0);

  delay(1);

  i = i + 2;
  if (i > 130)
  {
    i = 10;
  }
  // delay(10);
}
