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
#include <Arduino.h>
#include <TFT_eSPI.h> // Include the graphics library (this includes the sprite functions)

#include <startscreen.h>
#include <scale.h>

TFT_eSPI tft = TFT_eSPI(); // Create object "tft"

TFT_eSprite needle = TFT_eSprite(&tft); // Create Sprite object "needle" with pointer to "tft" object
                                        // the pointer is used by pushSprite() to push it onto the TFT

TFT_eSprite background = TFT_eSprite(&tft);
TFT_eSprite img = TFT_eSprite(&tft); // Create Sprite object "needle" with pointer to "tft" object

// Size of sprite
#define IWIDTH 240
#define IHEIGHT 240

int i = 0;

// #########################################################################
// Draw a number in a rounded rectangle with some transparent pixels
// #########################################################################
void numberBox(int x, int y, float num)
{

// Size of sprite
#define IWIDTHx 80
#define IHEIGHTx 35

  // Create a 8-bit sprite 80 pixels wide, 35 high (2800 bytes of RAM needed)
  img.setColorDepth(8);
  img.createSprite(IWIDTHx, IHEIGHTx);

  // Fill it with black (this will be the transparent colour this time)
  img.fillSprite(TFT_BLACK);

  // Draw a background for the numbers
  img.fillRoundRect(0, 0, 80, 35, 15, TFT_RED);
  img.drawRoundRect(0, 0, 80, 35, 15, TFT_WHITE);

  // Set the font parameters
  img.setTextSize(1);          // Font size scaling is x1
  img.setTextColor(TFT_WHITE); // White text, no background colour

  // Set text coordinate datum to middle right
  img.setTextDatum(MR_DATUM);

  // Draw the number to 3 decimal places at 70,20 in font 4
  img.drawFloat(num, 0, 70, 20, 4);

  // Push sprite to TFT screen CGRAM at coordinate x,y (top left corner)
  // All black pixels will not be drawn hence will show as "transparent"
  img.pushSprite(x, y, TFT_BLACK);

  // Delete sprite to free up the RAM
  img.deleteSprite();
}

/*! ******************************************************************
  @brief    Show a needle on the screen at a given engine speed
  @details  This function will show a needle on the screen at a given
          engine speed. The needle will be rotated to the correct
          angle to represent the engine speed.

  @param    speed  The engine speed in RPM
*/
void showNeedle(int speed)
{
  int angle = map(speed, 0, 4000, 226, 386);
  // Limit the angle to 0-359
  if (angle > 359)
  {
    angle = angle - 360;
  }
  // Draw the needle at the given angle
  needle.pushRotated(&background, angle, 0x041f);
}

void setup(void)
{
  Serial.begin(250000);

  tft.init();
  tft.fillScreen(TFT_WHITE);
  tft.setSwapBytes(true);
  tft.setRotation(0);
  tft.setPivot(120, 120);
  tft.pushImage(0, 0, 240, 240, _startscreen);

  delay(3000);

  background.createSprite(IWIDTH, IHEIGHT);
  background.setSwapBytes(true);
  background.setColorDepth(16);
  background.createSprite(IWIDTH, IHEIGHT);
  background.pushImage(0, 0, 240, 240, _scale);

  needle.createSprite(20, 100);
  needle.setColorDepth(16);
  needle.setSwapBytes(true);
  needle.pushImage(0, 0, 20, 100, _needle);
  needle.setPivot(10, 100);
  // //needle.fillSprite(0x041f);  // Fill sprite with transparent colour
  // needle.pushRotated(&background,90);
}

void loop()
{

  // needle.fillSprite(TFT_RED);
  // needle.fillSprite(0x041f);
  uint32_t dt = millis();
  background.pushImage(0, 0, 240, 240, _scale);
  showNeedle(i);
  background.pushSprite(0, 0);

  // Show time in milliseconds to draw and then push 1 sprite to TFT screen
  // numberBox( 100, 100, (millis()-dt));
  numberBox(100, 100, i);
  delay(1000);

  i = i + 250;
  if (i > 4000)
  {
    i = 0;
  }
  // delay(10);
}
