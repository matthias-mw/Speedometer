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

#include <Arduino.h>
#include <TFT_eSPI.h> // Include the graphics library (this includes the sprite functions)

// Images used for the display
#include <startscreen.h>
#include <scale.h>

// Special Fonts for the display
#include <airstrikeb3d26pt7b.h>
#include <airstrikeb3d18pt7b.h>
#include <White_On_Black10pt7b.h>
#include <G7_Segment_7a32pt7b.h>

/// Create object "tft" for the display interaction
extern TFT_eSPI tft;

/// Create Sprite object "needle" with pointer to "tft" object the pointer 
/// is used by pushSprite() to push it onto the TFT
extern TFT_eSprite needle;
/// Create Sprite object "background" with pointer to "tft" object the pointer
/// is used by pushSprite() to push it onto the TFT
extern TFT_eSprite background;
/// Create Sprite object "textSprite" with pointer to "tft" object the pointer
/// is used by pushSprite() to push it onto the TFT
extern TFT_eSprite textSprite;

// Size of the Display
#define IWIDTH 240
#define IHEIGHT 240

// Define the colours for the arc segments
#define COOLANT_ARC_COLOR_OK 0x0d00
#define COOLANT_ARC_COLOR_PASSIV 0x528a
#define COOLANT_ARC_COLOR_CRITICAL 0xd800
#define COOLANT_MIN_TEMPERATURE 30
#define COOLANT_MAX_TEMPERATURE 125
#define COOLANT_CRITICAL_TEMPERATURE 98
#define COOLANT_ARC_OUTER_DIAMETER 108
#define COOLANT_ARC_INNER_DIAMETER 100
#define COOLANT_ARC_ANGLE_START 330
#define COOLANT_ARC_ANGLE_END 270
#define COOLANT_TEXT_WIDTH 61
#define COOLANT_TEXT_HEIGHT 28
#define COOLANT_TEXT_POSITION_X 170
#define COOLANT_TEXT_POSITION_Y 85

// Define Speedtext Spritesize
#define SPEEDTEXT_WIDTH 136
#define SPEEDTEXT_HEIGHT 54

// Define Standard Text Spritesize
#define STD_TEXT_WIDTH 136
#define STD_TEXT_HEIGHT 54

// Define Engine Hours Spritesize
#define ENGINEHOURS_TEXT_WIDTH 91
#define ENGINEHOURS_TEXT_HEIGHT 28
#define ENGINEHOURS_POSITION_X 49
#define ENGINEHOURS_POSITION_Y 185

/*! ******************************************************************
  @brief    Show the engine speed on the screen
  @details  This function will show the engine speed on the screen.
          The engine speed will be shown as a number in a box.
  @param    speed <double> The engine speed in RPM
  @return   void
*/
void showEngineSpeed(double speed);

void showText(double speed);

/*! ******************************************************************
  @brief    Show the engine Hours on the screen
  @details  This function will show the engine hours on the screen.
          The engine hours will be shown as a number in a box.
  @param    engineHours <double> The engine hours
  @return   void
*/
void showEngineHours(double engineHours);

/*! ******************************************************************
  @brief    Show a needle on the screen at a given engine speed
  @details  This function will show a needle on the screen at a given
          engine speed. The needle will be rotated to the correct
          angle to represent the engine speed.

  @param    speed  The engine speed in RPM
*/
void showNeedle(int speed);

/*! ******************************************************************
  @brief    Show coolant temperature on the screen
  @details  This function will show the coolant temperature on the
          screen. The temperature will be shown as a arc with
          five segments. The arc will be filled with a colour to
          represent the temperature.

  @param    tCoolant <double> The temperature in degrees
  @return   void

*/
void showCoolantTemperature(double tCoolant);


#endif // _DISPLAYCTL_H_