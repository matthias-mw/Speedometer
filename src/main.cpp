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

#include <airstrikeb3d26pt7b.h>
#include <airstrikeb3d18pt7b.h>
#include <White_On_Black10pt7b.h>
#include <G7_Segment_7a32pt7b.h>

TFT_eSPI tft = TFT_eSPI(); // Create object "tft"

TFT_eSprite needle = TFT_eSprite(&tft); // Create Sprite object "needle" with pointer to "tft" object
                                        // the pointer is used by pushSprite() to push it onto the TFT

TFT_eSprite background = TFT_eSprite(&tft);
TFT_eSprite textSprite = TFT_eSprite(&tft); // Create Sprite object "needle" with pointer to "tft" object

// Size of sprite
#define IWIDTH 240
#define IHEIGHT 240

int i = 10;

/*! ******************************************************************
  @brief    Show the engine speed on the screen
  @details  This function will show the engine speed on the screen.
          The engine speed will be shown as a number in a box.
  @param    speed <double> The engine speed in RPM
  @return   void
*/
void showEngineSpeed(double speed)
{

// Size of sprite
#define IWIDTHx 136
#define IHEIGHTx 54

  String speedStr = String(speed, 0);

  // Create a 8-bit sprite 80 pixels wide, 35 high (2800 bytes of RAM needed)
  textSprite.setColorDepth(16);
  textSprite.createSprite(IWIDTHx, IHEIGHTx);

  // Fill it with black (this will be the transparent colour this time)
  textSprite.fillSprite(TFT_BLACK);

  // Draw a background for the numbers
  // textSprite.fillRect(0, 0, IWIDTHx, IHEIGHTx, TFT_RED);

  // Set the font parameters
  textSprite.setTextSize(1);          // Font size scaling is x1
  textSprite.setTextColor(TFT_WHITE); // White text, no background colour
  textSprite.setTextDatum(MR_DATUM);
  textSprite.setFreeFont(&G7_Segment_7a32pt7b);

  textSprite.drawString(speedStr, IWIDTHx, IHEIGHTx / 2);

  textSprite.pushToSprite(&background, 52, 125, TFT_BLACK);

  // Delete sprite to free up the RAM
  textSprite.deleteSprite();
}

void showText(double speed)
{

// Size of sprite
#define IWIDTHy 136
#define IHEIGHTy 54

  String speedStr = String(speed, 0);

  // Create a 8-bit sprite 80 pixels wide, 35 high (2800 bytes of RAM needed)
  textSprite.setColorDepth(16);
  textSprite.createSprite(IWIDTHx, IHEIGHTx);

  // Fill it with black (this will be the transparent colour this time)
  textSprite.fillSprite(TFT_BLACK);

  // Draw a background for the numbers
  // textSprite.fillRect(0, 0, IWIDTHx, IHEIGHTx, TFT_RED);

  // Set the font parameters
  textSprite.setTextSize(1);        // Font size scaling is x1
  textSprite.setTextColor(TFT_RED); // White text, no background colour
  textSprite.setTextDatum(MR_DATUM);
  textSprite.setFreeFont(&airstrikeb3d18pt7b);

  textSprite.drawString(speedStr + " °C", IWIDTHy, IHEIGHTy / 2);

  textSprite.pushToSprite(&background, 57, 50, TFT_BLACK);

  // Delete sprite to free up the RAM
  textSprite.deleteSprite();
}

/*! ******************************************************************
  @brief    Show the engine Hours on the screen
  @details  This function will show the engine hours on the screen.
          The engine hours will be shown as a number in a box.
  @param    engineHours <double> The engine hours
  @return   void
*/
void showEngineHours(double engineHours)
{

  String engineHoursStr = String(engineHours, 1);

  // Create a 8-bit sprite 80 pixels wide, 35 high (2800 bytes of RAM needed)
  textSprite.setColorDepth(16);
  textSprite.createSprite(91, 28);

  // Fill it with black (this will be the transparent colour this time)
  textSprite.fillSprite(TFT_BLACK);

  // Draw a background for the numbers
  textSprite.fillRect(0, 0, 91, 28, TFT_BLACK);

  // Set the font parameters
  textSprite.setTextSize(1);          // Font size scaling is x1
  textSprite.setTextColor(TFT_WHITE); // White text, no background colour
  textSprite.setTextDatum(MR_DATUM);
  textSprite.setFreeFont(&White_On_Black10pt7b);

  // Draw Text
  textSprite.drawString(engineHoursStr + "h", 90, 11);

  // Push sprite to TFT screen CGRAM at coordinate x,y (top left corner)
  // All black pixels will not be drawn hence will show as "transparent"
  textSprite.pushToSprite(&background, 49, 185, TFT_BLACK);

  // Delete sprite to free up the RAM
  textSprite.deleteSprite();
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
#define ARC_COLOR_OK 0x0d00
#define ARC_COLOR_PASSIV 0x528a
#define ARC_COLOR_CRITICAL 0xd800

#define MIN_TEMPERATURE 30
#define MAX_TEMPERATURE 125
#define CRITICAL_TEMPERATURE 98

  // Define the arc parameters
  uint16_t outerRadius = 108;
  uint8_t arcWidth = 8;

  double angleSegment = 10;
  uint16_t angleStart = 330;
  uint16_t angleEnd = 270;
  uint16_t arcColor = 0x0000;


  // String with the coolant temperature
  String tCoolantStr = String(tCoolant, 0);

  // ******************************************************************
  // Draw the Arc 
  // ******************************************************************
  // Specify the colour of the arc
  if (tCoolant > CRITICAL_TEMPERATURE)
  {
    arcColor = ARC_COLOR_CRITICAL;
  }
  else
  {
    arcColor = ARC_COLOR_OK;
  }
  // Limit the temperature to the min and max values
  if (tCoolant > MAX_TEMPERATURE){
    tCoolant = MAX_TEMPERATURE;
  }
  if (tCoolant < MIN_TEMPERATURE + 2) 
  {
    tCoolant = MIN_TEMPERATURE + 2;
    arcColor = ARC_COLOR_PASSIV;
  }

  // Calculate the angles for the arc
  angleSegment =(double)(angleEnd-angleStart) / (MAX_TEMPERATURE-MIN_TEMPERATURE);
  angleSegment = (double)angleStart + angleSegment * (tCoolant-MIN_TEMPERATURE);

  // Draw the arc on the background
  background.drawSmoothArc(120, 120, outerRadius, outerRadius - arcWidth, (uint16_t)angleSegment, angleStart, arcColor, TFT_BLACK, true);

  // ******************************************************************
  // Draw the Value
  // ******************************************************************
 
  // Create a 8-bit sprite 80 pixels wide, 35 high (2800 bytes of RAM needed)
  textSprite.setColorDepth(16);
  textSprite.createSprite(61, 28);

  // Fill it with black (this will be the transparent colour this time)
  textSprite.fillSprite(TFT_BLACK);

  // Draw a background for the numbers
  // textSprite.fillRect(0, 0, 61, 28, TFT_GREEN);

  // Set the font parameters
  textSprite.setTextSize(1); // Font size scaling is x1
  if (tCoolant > CRITICAL_TEMPERATURE)
  {
    textSprite.setTextColor(TFT_GREEN); // White text, no background colour
  }
  else
  {
    textSprite.setTextColor(TFT_WHITE); // White text, no background colour
  }
  textSprite.setTextDatum(MR_DATUM);
  textSprite.setFreeFont(&White_On_Black10pt7b);

  // Draw Text
  textSprite.drawString(tCoolantStr + " C", 60, 11);

  // Push sprite to TFT screen CGRAM at coordinate x,y (top left corner)
  // All black pixels will not be drawn hence will show as "transparent"
  textSprite.pushToSprite(&background, 170, 85, TFT_BLACK);

  // Delete sprite to free up the RAM
  textSprite.deleteSprite();
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
  showEngineSpeed(i * 29);
  showNeedle(i * 30);
  showEngineHours(i * 23.1);
  // showText(i * 21);

  background.pushSprite(0, 0);

  delay(500);

  i = i + 2;
  if (i > 130)
  {
    i = 10;
  }
  // delay(10);
}
