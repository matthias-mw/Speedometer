/*! \file 	hardwareDef.h
 *  \brief  Definition of Hardware Interfaces
 *
 *  Test
 *
 * \author 		Matthias Werner
 * \date		10 Sep 2022
 *
 * - Hardware:          az-delivery-devkit-v4
 * - Prozessor:         ESP32-WROOM
 *
 * ************************************************************
 * \version PROJECT_VERSION
 *************************************************************/

#ifndef _HARDWAREDEF_H_
#define _HARDWAREDEF_H_

#include <Arduino.h>


/*! ******************************************************************
  \brief  Debug Level for the project

  This defines the debug level for the project. The debug level
  is used to define the output of debug messages. The higher the
  level the more debug messages are printed.

  0: No debug messages
  1: Only error messages
  2: Error and warning messages
  3: Error, warning and info messages
*/
#define DEBUG_LEVEL 1

/// Define if raw N2K messages should be printed (if DEBUG_LEVEL > 0) true/false
#define DEBUG_RAW_N2K_MESSAGES false  

/// Define Engine Instance to be displayed
#define DISPLAY_ENGINE_INSTANCE 0


// --------> Display Brightness<---------------------
/// Define the Pin for Brightness Measurement
#define BRIGHTNESS_PIN 1
/// Define Brightness Analog Value for Max Daylight
#define ANALOG_VALUE_MAX_DAYLIGHT 500
/// Define Brightness Output Value for Max Daylight
#define BRIGHTNESS_OUTPUT_MAX_DAYLIGHT 253

/// Define Brightness Analog Value for Daylight
#define ANALOG_VALUE_DAYLIGHT 1000
/// Define Brightness Output Value for Daylight
#define BRIGHTNESS_OUTPUT_DAYLIGHT 120

/// Define Brightness Analog Value for Night
#define ANALOG_VALUE_NIGHT 2000
/// Define Brightness Output Value for Night
#define BRIGHTNESS_OUTPUT_NIGHT 20



// --------> NMEA2000 <---------------------
/// GPIO where the CAN-TX is connected
#define ESP32_CAN_TX_PIN (gpio_num_t)4
/// GPIO where the CAN-RX is connected
#define ESP32_CAN_RX_PIN (gpio_num_t)5


#endif // _HARDWAREDEF_H_