/**
 * \file  main.c
 * \brief Mainfile for the Speedometer project
 *
 * This file contains the main function for the Speedometer project. This
 * project task provide a simple display with the main task of displaying
 * the engine speed and other information. The Device is connected to a
 * NMEA 2000 network and receives the engine speed from the engine ECU.
 *
 * \author Matthias Werner
 * \date   November  2024
 * \version 0.1
 *
 *
 */
#include <Arduino.h>
#include <hardwareDef.h>
#include <stdint.h>

#include <displayCtl.h>

Adafruit_SSD1306 displayGlobal(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

DisplayCtl * displayControl;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Speedometer started");

  displayControl = new DisplayCtl(&displayGlobal);

  displayControl->drawline();

  // delay(2000);

  displayControl->displayEngineSpeed(3456);
  // delay(2000);
}

void loop()
{
  // put your main code here, to run repeatedly:
  Serial.println("Speedometer running");
  delay(1000);
  //displayControl.displayEngineSpeed(3456);
  delay(2000);
}

// void setup() {
//   Serial.begin(9600);

//   // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
//   if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
//     Serial.println(F("SSD1306 allocation failed"));
//     for(;;); // Don't proceed, loop forever
//   }

//   // Show initial display buffer contents on the screen --
//   // the library initializes this with an Adafruit splash screen.
//   display.display();
//   delay(2000); // Pause for 2 seconds

//   // Clear the buffer
//   display.clearDisplay();

//   // Draw a single pixel in white
//   display.drawPixel(10, 10, SSD1306_WHITE);

//   // Show the display buffer on the screen. You MUST call display() after
//   // drawing commands to make them visible on screen!
//   display.display();
//   delay(2000);
//   // display.display() is NOT necessary after every single drawing command,
//   // unless that's what you want...rather, you can batch up a bunch of
//   // drawing operations and then update the screen all at once by calling
//   // display.display(). These examples demonstrate both approaches...

//   testdrawline();      // Draw many lines

//   testdrawrect();      // Draw rectangles (outlines)

//   testfillrect();      // Draw rectangles (filled)

//   testdrawchar();      // Draw characters of the default font

//   testdrawstyles();    // Draw 'stylized' characters

//   testscrolltext();    // Draw scrolling text

//   testdrawbitmap();    // Draw a small bitmap image

//   // Invert and restore display, pausing in-between
//   display.invertDisplay(true);
//   delay(1000);
//   display.invertDisplay(false);
//   delay(1000);

//   testanimate(logo_bmp, LOGO_WIDTH, LOGO_HEIGHT); // Animate bitmaps
// }

// void loop() {
// }