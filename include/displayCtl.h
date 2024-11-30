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
#include <Adafruit_GFX.h>
#include "Adafruit_GC9A01A.h"

#define SCREEN_WIDTH  240 // OLED display width, in pixels
#define SCREEN_HEIGHT 240 // OLED display height, in pixels

/// Display constructor for primary hardware SPI connection
extern Adafruit_GC9A01A displayGlobal;  



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
  DisplayCtl(Adafruit_GC9A01A *dispGlobal);

  /*!
   * \brief Destructor
   *
   * This function clears the display and sets the display to the initial
   * state.
   */
  ~DisplayCtl();


  /*!
   * \brief Display engine speed
   *
   * This function displays the engine speed on the display.
   */
  void displayEngineSpeed( uint16_t engineSpeed );

private:


  // Display object for the OLED display
  Adafruit_GC9A01A *display;


};

#endif // _DISPLAYCTL_H_