/*!
 * \file main.cpp
 * \brief Main File for the Speedometer Project
 *
 * This file contains the main loop and task definitions for the Speedometer project.
 *
 * \author Matthias Werner
 * \date   January 2025
 * \version 0.1
 */

#include <hardwareDef.h>
#include <Arduino.h>
#include "displayCtl.h"
#include <process_n2k.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

/*!
 * \brief Calculate the display brightness
 *
 * This function will calculate the display brightness based on the
 * analog value of the brightness sensor.
 *
 * \return uint8_t brightness setpoint for Display
 */
uint8_t calcDisplayBrightness();

/*!
 * \brief Set the display brightness
 *
 * This function will calculate the display brightness based on the
 * analog value of the brightness sensor.
 *
 */
void setDisplayBrightness();

/// Task handle for updating NMEA2000 messages
TaskHandle_t taskUpdateN2KHandle = NULL;
/// Task handle for updating the display
TaskHandle_t taskUpdateDisplayHandle = NULL;
/// Task handle for setting display brightness
TaskHandle_t taskSetDisplayBrightnessHandle = NULL;

/*!
 * \brief Task for updating NMEA2000 messages
 *
 * This task runs on core 0 and updates NMEA2000 messages every 50ms.
 *
 * \param parameter Pointer to task parameters (not used).
 */
void taskUpdateN2K(void *parameter)
{
  for (;;)
  {
    updateN2K();
    vTaskDelay(pdMS_TO_TICKS(50)); // Delay for 50ms
  }
}

/*!
 * \brief Task for updating the display
 *
 * This task runs on core 1 and updates the display every 100ms.
 *
 * \param parameter Pointer to task parameters (not used).
 */
void taskUpdateDisplay(void *parameter)
{
  for (;;)
  {
    updateDisplay(DisplayData.EngineSpeed, DisplayData.EngineCoolantTemperature, DisplayData.EngineHours, DisplayData.LowOilPressureWarning);
    vTaskDelay(pdMS_TO_TICKS(100)); // Delay for 100ms
  }
}

/*!
 * \brief Task for setting display brightness
 *
 * This task runs on core 1 and adjusts the display brightness every 250ms.
 *
 * \param parameter Pointer to task parameters (not used).
 */
void taskSetDisplayBrightness(void *parameter)
{
  for (;;)
  {
    setDisplayBrightness();
    vTaskDelay(pdMS_TO_TICKS(250)); // Delay for 250ms
  }
}

// *****************************************************************************
// Setup the the software
// *****************************************************************************
/*!
 * \brief Setup function
 *
 * Initializes the hardware, display, and tasks for multitasking.
 */
void setup(void)
{

  Serial.begin(115200);
  delay(500);

  // Init the display
  initDisplay();

  // Initial Display Delay for Startscreen
  delay(5000);

  // Set the PWM Output for the Display Brightness
  pinMode(TFT_BL, OUTPUT);
  // Set the brightness pin to input
  pinMode(BRIGHTNESS_PIN, INPUT);
  // set the resolution to 12 bits (0-4096)
  analogReadResolution(12);

  // Init the NMEA2000
  initN2K();

  // Create tasks for multitasking
  xTaskCreatePinnedToCore(taskUpdateN2K, "UpdateN2K", 2048, NULL, 1, &taskUpdateN2KHandle, 0); // Core 0
  xTaskCreatePinnedToCore(taskUpdateDisplay, "UpdateDisplay", 2048, NULL, 1, &taskUpdateDisplayHandle, 1); // Core 1
  xTaskCreatePinnedToCore(taskSetDisplayBrightness, "SetDisplayBrightness", 2048, NULL, 1, &taskSetDisplayBrightnessHandle, 1); // Core 1
}

// *****************************************************************************
// Main Loop
// *****************************************************************************
/*!
 * \brief Main loop
 *
 * The main loop is empty as tasks handle the functionality.
 */
void loop()
{
  // Empty loop as tasks handle the functionality
}

// *****************************************************************************
// Calculate the display brightness
// *****************************************************************************
uint8_t calcDisplayBrightness()
{
  static uint32_t lastValue = 0;
  uint8_t brightness = 0;
  uint32_t value = 0;

  // Read the brightness value 8 times and calculate the average
  for (uint8_t cnt = 0; cnt < 8; cnt++)
  {
    value += analogRead(BRIGHTNESS_PIN);
  }
  value = value >> 3;

  // Apply hysteresis to the measured value
  if (abs((int32_t)value - (int32_t)lastValue) < BRIGHTNESS_HYSTERESE)
  {
    value = lastValue;
  }
  else
  {
    lastValue = value;
  }

  // Adjust the brightness
  if (value > ANALOG_VALUE_NIGHT)
  {
    // Night, it is dark
    brightness = BRIGHTNESS_OUTPUT_NIGHT;
  }
  else if (value > ANALOG_VALUE_DAYLIGHT)
  {
    // Normal Daylight grey sky
    brightness = BRIGHTNESS_OUTPUT_DAYLIGHT;
  }
  else
  {
    // Now its bright Daylight
    brightness = BRIGHTNESS_OUTPUT_MAX_DAYLIGHT;
  }

  // Only if Debug is enabled
#ifdef DEBUG_DISPLAY_BRIGHTNESS
  Serial.print(millis());
  Serial.print(": Brightness Sensor Analog Value: ");
  Serial.print(value);
  Serial.print(" -- > Brightness: ");
  Serial.println(brightness);
#endif

  // return the brightness value
  return brightness;
}

// *****************************************************************************
// Set Display Brightness
// *****************************************************************************
void setDisplayBrightness()
{

  uint8_t brightness = 0;

  // Calculate the brightness
  brightness = calcDisplayBrightness();

  // Set the brightness
  analogWrite(TFT_BL, brightness);
}
