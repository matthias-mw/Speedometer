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

// Size of sprite
#define IWIDTH 240
#define IHEIGHT 240

int i = 0;

// #########################################################################
// Draw a number in a rounded rectangle with some transparent pixels
// #########################################################################
// void numberBox(int x, int y, float num)
// {

// // Size of sprite
// #define IWIDTH 80
// #define IHEIGHT 35

//   // Create a 8-bit sprite 80 pixels wide, 35 high (2800 bytes of RAM needed)
//   needle.setColorDepth(8);
//   needle.createSprite(IWIDTH, IHEIGHT);

//   // Fill it with black (this will be the transparent colour this time)
//   needle.fillSprite(TFT_BLACK);

//   // Draw a background for the numbers
//   needle.fillRoundRect(0, 0, 80, 35, 15, TFT_RED);
//   needle.drawRoundRect(0, 0, 80, 35, 15, TFT_WHITE);

//   // Set the font parameters
//   needle.setTextSize(1);          // Font size scaling is x1
//   needle.setTextColor(TFT_WHITE); // White text, no background colour

//   // Set text coordinate datum to middle right
//   needle.setTextDatum(MR_DATUM);

//   // Draw the number to 3 decimal places at 70,20 in font 4
//   needle.drawFloat(num, 3, 70, 20, 4);

//   // Push sprite to TFT screen CGRAM at coordinate x,y (top left corner)
//   // All black pixels will not be drawn hence will show as "transparent"
//   needle.pushSprite(100, 100, TFT_BLACK);

//   // Delete sprite to free up the RAM
//   needle.deleteSprite();
// }

void showNeedle(int angle)
{



  // Create a 8-bit sprite 80 pixels wide, 35 high (2800 bytes of RAM needed)
  needle.setColorDepth(8);
  needle.createSprite(IWIDTH, IHEIGHT);

  // Fill it with black (this will be the transparent colour this time)
  needle.fillSprite(0x041f);

  // push image needle to sprite
  needle.setSwapBytes(true);
  needle.pushImage(0, 0, 240, 240, _needle);

  // rotate the needle
  needle.setPivot(120, 120);
  //needle.setRotation(angle);
   

  // Push sprite to TFT screen CGRAM at coordinate x,y (top left corner)
  // All black pixels will not be drawn hence will show as "transparent"
  //needle.pushSprite(0, 0, 0x041f);
  needle.pushSprite(0, 0, 0x041f);
  // Delete sprite to free up the RAM
  needle.deleteSprite();
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
  //needle.pushRotated(&background,90);


  
}

void loop()
{

  //needle.fillSprite(TFT_RED);
  //needle.fillSprite(0x041f);
  background.pushImage(0, 0, 240, 240, _scale);
  needle.pushRotated(&background,i,0x041f);
  background.pushSprite(0, 0);

  i++;
  if (i > 360)
  {
    i = 0;
  }
  delay(10);

  // Show time in milliseconds to draw and then push 1 sprite to TFT screen
  // numberBox( 10, 10, (millis()-dt)/500.0 );

  //delay(500);
}
