/*! \file 	hardwareDef.h
 *  \brief  Definition of Hardware Interfaces
 *
 *  Test
 *
 * \author 		Matthias Werner
 * \date		10 Sep 2022
 *
 * - Hardware:          Seeed Studio Xiao ESP32-S3 Dev Board
 * - Prozessor:         ESP32-S3
 *
 * ************************************************************
 * \version PROJECT_VERSION
 *************************************************************/

#ifndef _HARDWAREDEF_H_
#define _HARDWAREDEF_H_

#include <Arduino.h>


// --------------> Debug Messages<-------------------
/// Define if error messages should be printed true/false
#define DEBUG_ERROR true

/// Define if raw N2K messages should be printed true/false
#define DEBUG_RAW_N2K_MESSAGES false  

/// Define if NMEA2000 messages should be printed true/false
#define DEBUG_NSK_MSG true

/// Define if display brightness should be printed true/false
#define DEBUG_DISPLAY_BRIGHTNESS true

/// Define if N2k Message Statistics should be printed true/false
#define DEBUG_N2K_MSG_STATISTICS true

// --------> Config N2K Message Engine ID  <--------------
/// Define Engine Instance to be displayed
#define DISPLAY_ENGINE_INSTANCE 0

/// Define Time out [ms] for Engine Rapid Message
#define N2K_MSG_ENGINE_RAPID_TIMEOUT 10000


// --------> Display Brightness<---------------------
/// Define the Pin for Brightness Measurement
#define BRIGHTNESS_PIN 1

/// Define Hysterese for Brightness Measurement
#define BRIGHTNESS_HYSTERESE 50

/// Define Brightness Analog Value for Max Daylight
#define ANALOG_VALUE_MAX_DAYLIGHT 500
/// Define Brightness Output Value for Max Daylight
#define BRIGHTNESS_OUTPUT_MAX_DAYLIGHT 253

/// Define Brightness Analog Value for Daylight
#define ANALOG_VALUE_DAYLIGHT 1500
/// Define Brightness Output Value for Daylight
#define BRIGHTNESS_OUTPUT_DAYLIGHT 120

/// Define Brightness Analog Value for Night
#define ANALOG_VALUE_NIGHT 1900
/// Define Brightness Output Value for Night
#define BRIGHTNESS_OUTPUT_NIGHT 20



// --------> NMEA2000 <---------------------
/// GPIO where the CAN-TX is connected
#define ESP32_CAN_TX_PIN (gpio_num_t)4
/// GPIO where the CAN-RX is connected
#define ESP32_CAN_RX_PIN (gpio_num_t)5


#endif // _HARDWAREDEF_H_