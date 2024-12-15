/*!
 * \file displayCtl.cpp
 * \brief Display Control
 *
 * This file contains the functions for the display control of the OLED display
 * It provides different pages for displaying all signals of the speedometer.
 *
 */
#include "displayCtl.h"

//******************************************************************
// Init Global Variables
//******************************************************************
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite needle = TFT_eSprite(&tft);
TFT_eSprite background = TFT_eSprite(&tft);
TFT_eSprite textSprite = TFT_eSprite(&tft);

//******************************************************************
// Show the engine speed on the screen
//******************************************************************
void showEngineSpeed(double speed)
{
    // Transfer the speed to a string
    String speedStr = String(speed, 0);

    // Create a 16-bit sprite
    textSprite.setColorDepth(16);
    textSprite.createSprite(SPEEDTEXT_WIDTH, SPEEDTEXT_HEIGHT);

    // Fill it with black (this will be the transparent colour this time)
    textSprite.fillSprite(TFT_BLACK);

    // Draw a background for the numbers (for testing only)
    // textSprite.fillRect(0, 0, SPEEDTEXT_WIDTH, SPEEDTEXT_HEIGHT, TFT_RED);

    // Set the font parameters
    textSprite.setTextSize(1);                    // Font size scaling is x1
    textSprite.setTextColor(TFT_WHITE);           // White text, no background colour
    textSprite.setTextDatum(MR_DATUM);            // Middle right text alignment
    textSprite.setFreeFont(&G7_Segment_7a32pt7b); // Set the font

    // Draw the text
    textSprite.drawString(speedStr, SPEEDTEXT_WIDTH, SPEEDTEXT_HEIGHT / 2);

    // Push the sprite to the background sprite
    textSprite.pushToSprite(&background, 52, 125, TFT_BLACK);

    // Delete sprite to free up the RAM
    textSprite.deleteSprite();
}

//******************************************************************
// Show the text on the screen
//******************************************************************
void showText(double speed)
{
    String speedStr = String(speed, 0);

    // Create a 16-bit sprite
    textSprite.setColorDepth(16);
    textSprite.createSprite(SPEEDTEXT_WIDTH, SPEEDTEXT_HEIGHT);

    // Fill it with black (this will be the transparent colour this time)
    textSprite.fillSprite(TFT_BLACK);

    // Draw a background for the numbers
    // textSprite.fillRect(0, 0, SPEEDTEXT_WIDTH, SPEEDTEXT_HEIGHT, TFT_RED);

    // Set the font parameters
    textSprite.setTextSize(1);        // Font size scaling is x1
    textSprite.setTextColor(TFT_RED); // White text, no background colour
    textSprite.setTextDatum(MR_DATUM);
    textSprite.setFreeFont(&airstrikeb3d18pt7b);

    // Draw Text
    textSprite.drawString(speedStr + " °C", STD_TEXT_WIDTH, STD_TEXT_HEIGHT / 2);

    // Push sprite to TFT screen CGRAM at coordinate x,y (top left corner)
    // All black pixels will not be drawn hence will show as "transparent"
    textSprite.pushToSprite(&background, 57, 50, TFT_BLACK);

    // Delete sprite to free up the RAM
    textSprite.deleteSprite();
}

//******************************************************************
// Show the engine Hours on the screen
//******************************************************************
void showEngineHours(double engineHours)
{
    // Transfer the engine hours to a string
    String engineHoursStr = String(engineHours, 1);

    // Create a 16-bit sprite
    textSprite.setColorDepth(16);
    textSprite.createSprite(ENGINEHOURS_TEXT_WIDTH, ENGINEHOURS_TEXT_HEIGHT);

    // Fill it with black (this will be the transparent colour this time)
    textSprite.fillSprite(TFT_BLACK);

    // Draw a background for the numbers (for testing only)
    // textSprite.fillRect(0, 0, ENGINEHOURS_TEXT_WIDTH, ENGINEHOURS_TEXT_HEIGHT, TFT_RED);

    // Set the font parameters
    textSprite.setTextSize(1);                     // Font size scaling is x1
    textSprite.setTextColor(TFT_WHITE);            // White text, no background colour
    textSprite.setTextDatum(MR_DATUM);             // Middle right text alignment
    textSprite.setFreeFont(&White_On_Black10pt7b); // Set the font

    // Draw Text
    textSprite.drawString(engineHoursStr + "h",(ENGINEHOURS_TEXT_WIDTH - 1), (ENGINEHOURS_TEXT_HEIGHT / 2) - 3);

    // Push sprite to TFT screen CGRAM at coordinate x,y (top left corner)
    // All black pixels will not be drawn hence will show as "transparent"
    textSprite.pushToSprite(&background, ENGINEHOURS_POSITION_X, ENGINEHOURS_POSITION_Y, TFT_BLACK);

    // Delete sprite to free up the RAM
    textSprite.deleteSprite();
}

//******************************************************************
// Show a needle on the screen at a given engine speed
//******************************************************************
void showNeedle(int speed)
{
    int angle = map(speed, 0, 4000, 226, 386);
    // Limit the angle to 0-359
    if (angle > 359)
    {
        angle = angle - 360;
    }
    // Draw the needle at the given angle
    //needle.pushRotated(&background, angle, 0x041f);
    needle.pushRotated(&background, angle, TFT_BLACK);
}

//******************************************************************
// Show coolant temperature on the screen
//******************************************************************
void showCoolantTemperature(double tCoolant)
{
    double angleSegment = 0;
    uint16_t arcColor = 0x0000;

    // String with the coolant temperature
    String tCoolantStr = String(tCoolant, 0);

    // ******************************************************************
    // Draw the Arc
    // ******************************************************************
    // Specify the colour of the arc
    if (tCoolant > COOLANT_CRITICAL_TEMPERATURE)
    {
        arcColor = COOLANT_ARC_COLOR_CRITICAL;
    }
    else
    {
        arcColor = COOLANT_ARC_COLOR_OK;
    }
    // Limit the temperature to the min and max values
    if (tCoolant > COOLANT_MAX_TEMPERATURE)
    {
        tCoolant = COOLANT_MAX_TEMPERATURE;
    }
    if (tCoolant < COOLANT_MIN_TEMPERATURE + 2)
    {
        tCoolant = COOLANT_MIN_TEMPERATURE + 2;
        arcColor = COOLANT_ARC_COLOR_PASSIV;
    }

    // Calculate the angles for the arc
    angleSegment = (double)(COOLANT_ARC_ANGLE_END - COOLANT_ARC_ANGLE_START) / (COOLANT_MAX_TEMPERATURE - COOLANT_MIN_TEMPERATURE);
    angleSegment = (double)COOLANT_ARC_ANGLE_START + angleSegment * (tCoolant - COOLANT_MIN_TEMPERATURE);

    // Draw the arc on the background
    background.drawSmoothArc(120, 120, COOLANT_ARC_OUTER_DIAMETER, COOLANT_ARC_INNER_DIAMETER, (uint16_t)angleSegment, COOLANT_ARC_ANGLE_START, arcColor, TFT_BLACK, true);

    // ******************************************************************
    // Draw the Value
    // ******************************************************************

    // Create a 8-bit sprite
    textSprite.setColorDepth(16);
    textSprite.createSprite(COOLANT_TEXT_WIDTH, COOLANT_TEXT_HEIGHT);

    // Fill it with black (this will be the transparent colour this time)
    textSprite.fillSprite(TFT_BLACK);

    // Draw a background for the numbers
    // textSprite.fillRect(0, 0, 61, 28, TFT_GREEN);

    // Set the font parameters
    textSprite.setTextSize(1); // Font size scaling is x1
    if (tCoolant > COOLANT_CRITICAL_TEMPERATURE)
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
    textSprite.drawString(tCoolantStr + " C", (COOLANT_TEXT_WIDTH - 1), (COOLANT_TEXT_HEIGHT / 2) - 3);

    // Push sprite to TFT screen CGRAM at coordinate x,y (top left corner)
    // All black pixels will not be drawn hence will show as "transparent"
    textSprite.pushToSprite(&background, COOLANT_TEXT_POSITION_X, COOLANT_TEXT_POSITION_Y, TFT_BLACK);

    // Delete sprite to free up the RAM
    textSprite.deleteSprite();
}
