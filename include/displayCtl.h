/*!
 * \file displayCtl.h
 * \brief Display Control
 *
 * This file contains the functions for the display control of the OLED display
 * It provides different pages for displaying all signals of the speedometer.
 *
 * \author Matthias Werner
 * \date   November  2024
 * \version 0.1
 *
 *
 */

#ifndef _DISPLAYCTL_H_
#define _DISPLAYCTL_H_

#include <hardwareDef.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library.
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET -1       // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

extern Adafruit_SSD1306 displayGlobal;

//extern TwoWire I2CWire;

#define NUMFLAKES 10 // Number of snowflakes in the animation example

#define LOGO_HEIGHT 16
#define LOGO_WIDTH 16

static const unsigned char PROGMEM logo_bmp[] =
    {0b00000000, 0b11000000,
     0b00000001, 0b11000000,
     0b00000001, 0b11000000,
     0b00000011, 0b11100000,
     0b11110011, 0b11100000,
     0b11111110, 0b11111000,
     0b01111110, 0b11111111,
     0b00110011, 0b10011111,
     0b00011111, 0b11111100,
     0b00001101, 0b01110000,
     0b00011011, 0b10100000,
     0b00111111, 0b11100000,
     0b00111111, 0b11110000,
     0b01111100, 0b11110000,
     0b01110000, 0b01110000,
     0b00000000, 0b00110000};

/*! \class DisplayCtl
 *  \brief Display Control
 *
 *  This class provides the functions for the display control of the OLED
 *  display. It provides different pages for displaying all signals of the
 *  speedometer.
 */
class DisplayCtl
{

public:
  /*!
   * \brief Constructor
   *
   * This function initializes the display and sets the display to the
   * initial state.
   */
  DisplayCtl(Adafruit_SSD1306 *dispGlobal);

  /*!
   * \brief Destructor
   *
   * This function clears the display and sets the display to the initial
   * state.
   */
  ~DisplayCtl();

  /*!
   * \brief Draw a line
   *
   * This function draws a line on the display.
   */
  void drawline();

  /*!
   * \brief Display engine speed
   *
   * This function displays the engine speed on the display.
   */
  void displayEngineSpeed( uint16_t engineSpeed );

private:


  // Display object for the OLED display
  Adafruit_SSD1306 *display;


};

#endif // _DISPLAYCTL_H_