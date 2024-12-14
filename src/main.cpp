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

int i = 10;

// #########################################################################
// Draw a number in a rounded rectangle with some transparent pixels
// #########################################################################
void numberBox(int x, int y, float num)
{

// Size of sprite
#define IWIDTHx 130
#define IHEIGHTx 60

  // Create a 8-bit sprite 80 pixels wide, 35 high (2800 bytes of RAM needed)
  img.setColorDepth(16);
  img.createSprite(IWIDTHx, IHEIGHTx);

  // Fill it with black (this will be the transparent colour this time)
  img.fillSprite(TFT_BLACK);

  // Draw a background for the numbers
  // img.fillRoundRect(0, 0, 180, 80, 1, TFT_RED);
  // img.drawRoundRect(0, 0, 80, 35, 15, TFT_WHITE);

  // Set the font parameters
  img.setTextSize(1); // Font size scaling is x1

  img.setTextColor(TFT_WHITE); // White text, no background colour

  // Set text coordinate datum to middle right
  img.setTextDatum(MR_DATUM);

  // Draw the number to 3 decimal places at 70,20 in font 4
  img.drawFloat(num, 0, IWIDTHx, IHEIGHTx / 2, 7);

  // Push sprite to TFT screen CGRAM at coordinate x,y (top left corner)
  // All black pixels will not be drawn hence will show as "transparent"
  img.pushSprite(x, y, TFT_BLACK);
  img.pushToSprite(&background, x, y, TFT_BLACK);

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

/*! ******************************************************************
  @brief    Show coolant temperature on the screen
  @details  This function will show the coolant temperature on the
          screen. The temperature will be shown as a arc with
          five segments. The arc will be filled with a colour to
          represent the temperature.

  @param    tCoolant <double> The temperature in degrees
  @return   void

*/
void showCoolantTemperature(double tCoolant)
{

// Define the colours for the arc segments
#define ARC_COLOR_OK 0x0582
#define ARC_COLOR_PASSIV 0x7bef
#define ARC_COLOR_CRITICAL 0xe800

#define MIN_TEMPERATURE 30
#define MAX_TEMPERATURE 110
#define CRITICAL_TEMPERATURE 98

  // Define the arc parameters
  size_t numSegments = 7;
  uint16_t outerRadius = 112;
  uint8_t arcWidth = 10;
  uint16_t angleSegment = 10;
  uint16_t angleSegmentSpace = 2;
  uint16_t angleStart = 330;
  uint16_t arcColor = 0x0000;

  uint16_t segmentStart[numSegments] = {};
  uint16_t segmentEnd[numSegments] = {};
  uint16_t segmentColour[numSegments] = {};

  // Map the temperature to the segment index
  uint8_t segmentIndex = (uint8_t)map(tCoolant, MIN_TEMPERATURE, MAX_TEMPERATURE, 0, 6);
  segmentIndex = constrain(segmentIndex, 0, numSegments - 1);
  if (tCoolant < MIN_TEMPERATURE)
  {
    segmentIndex = 0;
  }

  // ******************************************************************
  // Calculate the arc segments
  // ******************************************************************
  // Draw the arc segments
  for (uint8_t i = 0; i < numSegments; i++)
  {
    // Set the colour of the arc segment
    if (i < segmentIndex)
    {
      segmentColour[i] = ARC_COLOR_OK;
      if (tCoolant > CRITICAL_TEMPERATURE)
      {
        segmentColour[i] = ARC_COLOR_CRITICAL;
      }
    }
    else
    {
      segmentColour[i] = ARC_COLOR_PASSIV;
    }

    // Set the angle segment and space for the arc segment
    angleSegment = ((i == 0) || (i == numSegments - 1)) ? 1 : 10;
    angleSegmentSpace = ((i == 0) || (i == numSegments - 1)) ? 0 : 2;

    // Set the start and end angles for the arc segment
    segmentStart[i] = angleStart - angleSegment;
    segmentEnd[i] = angleStart;

    // Set the start angle for the next segment
    angleStart = angleStart - angleSegment;
    if (i < numSegments - 2)
    {
      angleStart = angleStart - angleSegmentSpace;
    }
  }

  // ******************************************************************
  // Draw the arc segments
  // ******************************************************************
  // Draw the first arc segments
  background.drawSmoothArc(120, 120, outerRadius, outerRadius - arcWidth, segmentStart[0], segmentEnd[0], segmentColour[0], segmentColour[0], true);
  // Draw the arc segments
  for (uint8_t i = 1; i < numSegments - 1; i++)
  {
    // Draw the last arc segment
    if (i == numSegments - 3)
    {
      background.drawSmoothArc(120, 120, outerRadius, outerRadius - arcWidth, segmentStart[numSegments - 1], segmentEnd[numSegments - 1], segmentColour[numSegments - 1], segmentColour[numSegments - 1], true);
    }
    // Draw the other arc segments
    background.drawSmoothArc(120, 120, outerRadius, outerRadius - arcWidth, segmentStart[i], segmentEnd[i], segmentColour[i], segmentColour[i], false);
  }
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
  background.pushImage(0, 0, 240, 240, _scale_1);

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

  if (i > 100)
  {
    background.pushImage(0, 0, 240, 240, _scale_2);
  }
  else
  {
    background.pushImage(0, 0, 240, 240, _scale_1);
  }

  showCoolantTemperature(i);
  numberBox(65, 130, i * 30);
  showNeedle(i * 30);

  background.pushSprite(0, 0);

  delay(100);

  i = i + 2;
  if (i > 130)
  {
    i = 10;
  }
  // delay(10);
}
