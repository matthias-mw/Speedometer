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
  @brief  Structure for the data which will be displayed

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
  /// Low Oil Pressure Warning
  bool LowOilPressureWarning;
  /// Engine Discrete Status 1
  tN2kEngineDiscreteStatus1 EngineDiscreteStatus1;
  /// Engine Discrete Status 2
  tN2kEngineDiscreteStatus2 EngineDiscreteStatus2;
} tDisplayData;

/*! ******************************************************************
  @class  N2kMsgStatistics
  @brief  Class for the NMEA2000 message statistics

  This structure contains the statistics for the NMEA2000 messages. This
  data will be used for error handling and debugging of the N2K messages.  
 */
class N2kMsgStatistics
{
public:
  /// Constructor
  N2kMsgStatistics();
  /// Destructor
  ~N2kMsgStatistics();


  /*! ******************************************************************
    @brief Update message counter for certain PGN

    This function will update the message counter and timestamp
    for the given PGN.

    @param PGN PGN of the message
   */
  void UpdateMsgCnt(uint32_t PGN, uint8_t Instance = 0);

  /*! ******************************************************************
    @brief Increase failed message counter for certain PGN

    This function will increase the failed message counter for the given PGN.
    @param PGN PGN of the message
   */ 
  void IncreaseFailedMsgCnt(uint32_t PGN);

  /*! ******************************************************************
    @brief Get message counter for certain PGN

    This function will return the message counter for the given PGN.
    @param PGN PGN of the message
    @return uint32_t Message counter
   */
  uint32_t GetMsgCnt(uint32_t PGN);

  /*! ******************************************************************
    @brief Get failed message counter for certain PGN

    This function will return the failed message counter for the given PGN.
    @param PGN PGN of the message
    @return uint32_t Failed message counter
   */
  uint32_t GetFailedMsgCnt(uint32_t PGN);

  /*! ******************************************************************
    @brief Check if the N2K message is timed out

    This function will check if the N2K message for the engine
    instance @ref DISPLAY_ENGINE_INSTANCE is timed out. Therefore the
    message 127488L Engine Rapid is used.

    @sa N2K_MSG_ENGINE_RAPID_TIMEOUT
    @return bool true if timed out, false if not
   */ 
  bool N2kIsTimeOut(void);
  
  /*! ******************************************************************
    @brief Show the N2K message statistics via Serial
    This function will show the N2K message statistics via Serial.
    */
  void ShowStatistics(void);

private:
  /// Number of received messages System Time
  uint32_t SystemTimeCnt;
  /// Number of messages failed parse System Time
  uint32_t SystemTimeFailedCnt;
  /// Number of received messages Engine Rapid
  uint32_t EngineRapidCnt;
  /// Number of messages failed parse Engine Rapid
  uint32_t EngineRapidFailedCnt;
  /// Number of received messages Engine Dynamic
  uint32_t EngineDynamicCnt;
  /// Number of messages failed parse Engine Dynamic
  uint32_t EngineDynamicFailedCnt;
  /// Number of received messages Transmission Parameters
  uint32_t TransmissionParametersCnt;
  /// Number of messages failed parse Transmission Parameters
  uint32_t TransmissionParametersFailedCnt; 
  /// Timestamp of the last received message System Time
  uint32_t SystemTimeLastTimestamp;
  /// Timestamp of the last received message Engine Rapid
  uint32_t EngineRapidLastTimestamp;
  /// Timestamp of the last received message Engine Dynamic
  uint32_t EngineDynamicLastTimestamp;
  /// Timestamp of the last received message Transmission Parameters
  uint32_t TransmissionParametersLastTimestamp;

};


// Debug level is greater than 0
#if defined(DEBUG_ERROR) || defined(DEBUG_NSK_MSG) 

/// Pointer to the output stream
extern Stream *OutputStream;

#endif // End of DEBUG_LEVEL > 0

/// Global Structure for the display data
extern tDisplayData DisplayData;

/// Mutex to protect the Serial output for tread safety
extern SemaphoreHandle_t SerialOutputMutex;

/// Object for the N2K message statistics
extern N2kMsgStatistics N2kMessageStatistics;

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
            \ref HandleNMEA2000Msg, \ref NMEA2000.setMessageHandler
 */
void updateN2K(void);

#endif // _PROCESS_N2K_H_
