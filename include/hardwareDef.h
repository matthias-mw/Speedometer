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


// --------> NMEA2000 <---------------------
/// GPIO where the CAN-TX is connected
#define ESP32_CAN_TX_PIN (gpio_num_t)35
/// GPIO where the CAN-RX is connected
#define ESP32_CAN_RX_PIN (gpio_num_t)34


#endif // _HARDWAREDEF_H_