/*!
 * \file process_n2k.h
 * \brief Processing all NMEA2000 messages
 *
 * This file contains the functions for processing all NMEA2000 messages.
 *
 * \author Matthias Werner
 * \date   January 2025
 * \version 0.1
 *
 *
 */

#ifndef _PROCESS_N2K_H_
#define _PROCESS_N2K_H_

#include <hardwareDef.h>

#include <N2kMessages.h>

/*! ******************************************************************
  @struct tNMEA2000Handler
  @brief  Structure for the NMEA2000 message handler

  This structure contains the PGN and the handler function for the
  NMEA2000 message handler.

 */
typedef struct
{
  /// PGN of the message
  unsigned long PGN;
  /// Handler function for the message
  void (*Handler)(const tN2kMsg &N2kMsg);
} tNMEA2000Handler;

/*! ******************************************************************
  @struct tDisplayData
  @brief  Structure for the data wich will be displayed

  This structure contains the data which will be displayed on the
  screen.
 */
typedef struct
{
  /// Engine Speed
  double EngineSpeed;
  /// Engine Hours
  double EngineHours;
  /// Coolant Temperature
  double EngineCoolantTemperature;
  /// Engine Oil Pressure
  double EngineOilPressure;
  /// Engine Alternator Voltage
  double EngineAlternatorVoltage;
  /// Low Oil Pressure Waring
  bool LowOilPressureWarning;
  /// Engine Discrete Status 1
  tN2kEngineDiscreteStatus1 EngineDiscreteStatus1;
  /// Engine Discrete Status 2
  tN2kEngineDiscreteStatus2 EngineDiscreteStatus2;
} tDisplayData;

#if DEBUG_LEVEL > 0

/// Pointer to the output stream
extern Stream *OutputStream;

#endif

/// Global Structure for the display data
extern tDisplayData DisplayData;


/*! ******************************************************************
  @brief    Init the NMEA2000 Inteface
  @details  This function will init the NMEA2000 bus and the message
            handler.
  @return   int8_t 0 if success, -1 if failed
 */
int8_t initN2K(void);


/*! ******************************************************************
  @brief    Evaluate System Time Message
  @details  This function will evaluate the system time message and
            store the data in the display data structure.
            \ref DisplayData

  @param    N2kMsg NMEA2000 message
 */
void SystemTime(const tN2kMsg &N2kMsg);

/*! ******************************************************************
  @brief    Evaluate EngineRapid Message
  @details  This function will evaluate the engine rapid message and
            store the data in the display data structure.
            \ref DisplayData

  @param    N2kMsg NMEA2000 message
 */
void EngineRapid(const tN2kMsg &N2kMsg);

/*! ******************************************************************
  @brief    Evaluate EngineDynamic Message
  @details  This function will evaluate the engine dynamic message and
            store the data in the display data structure.
            \ref DisplayData

  @param    N2kMsg NMEA2000 message
 */
void EngineDynamicParameters(const tN2kMsg &N2kMsg);

/*! ******************************************************************
  @brief    Evaluate Transmission Parameters Message
  @details  This function will evaluate the transmission parameters
            message and store the data in the display data structure.
            \ref DisplayData

  @param    N2kMsg NMEA2000 message
 */
void TransmissionParameters(const tN2kMsg &N2kMsg);

/*! ******************************************************************
  @brief    NMEA2000 Message handler
  @details  This function will handle incoming the NMEA2000 messages and
            call the corresponding handler function.

  @param    N2kMsg NMEA2000 message
 */
void HandleNMEA2000Msg(const tN2kMsg &N2kMsg);

/*! ******************************************************************
  @brief    Update the NMEA2000 messages
  @details  this function will check the NMEA2000 bus for new messages
            and call the message handler for each message.
            \ref HandleNMEA2000Msg, \ref NEA2000.setMessageHandler
 */
void updateN2K(void);

#endif // _PROCESS_N2K_H_
