/*!
 * \file process_n2k.cpp
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

#include <process_n2k.h>
#include <N2kMessagesEnumToStr.h>
#include <NMEA2000_CAN.h>

// Define DISPLAY_ENGINE_INSTANCE if not already defined
#ifndef DISPLAY_ENGINE_INSTANCE
#define DISPLAY_ENGINE_INSTANCE 0 // Default value, update as needed
#endif

// Only if Debug is enable
#if defined(DEBUG_ERROR) || defined(DEBUG_NSK_MSG)
// Output stream for the NMEA2000 messages
Stream *OutputStream;
#endif

// Global Structure for the display data
tDisplayData DisplayData;

// Object for the NMEA2000 messages statistics
N2kMsgStatistics N2kMessageStatistics;

// Handler for the NMEA2000 messages
tNMEA2000Handler NMEA2000Handlers[] = {
    {126992L, &SystemTime},
    {127488L, &EngineRapid},
    {127489L, &EngineDynamicParameters},
    {127493L, &TransmissionParameters},
    {0, 0} // Terminator for the PGN search function, indicating the end of the handler list
};

void updateN2K(void)
{
  // Process NMEA2000 messages
  NMEA2000.ParseMessages();
}

//*****************************************************************************
// Helper function to print "Failed to parse PGN" debug message
void PrintFailedToParsePGN(uint32_t PGN)
{
#ifdef DEBUG_ERROR
  // Tread safety with mutex SerialOutputMutex
  if (SerialOutputMutex)
  {
    if (xSemaphoreTake(SerialOutputMutex, pdMS_TO_TICKS(20)) == pdTRUE)
    {
      OutputStream->print("Failed to parse PGN: ");
      OutputStream->println(PGN);
      // free the mutex
      xSemaphoreGive(SerialOutputMutex);
    }
  }
#endif
}

//*****************************************************************************
// Print a label, a value and convert the value if a conversion function is given
//
template <typename T>
void PrintLabelValWithConversionCheckUnDef(const char *label, T val, double (*ConvFunc)(double val) = nullptr, bool AddLf = false, int8_t DecimalPlaces = -1)
{
  // Only if Debug is enabled
#ifdef DEBUG_NSK_MSG
  OutputStream->print(label);
  if (!N2kIsNA(val))
  {
    if (DecimalPlaces < 0)
    {
      if (ConvFunc)
      {
        OutputStream->print(ConvFunc(val));
      }
      else
      {
        OutputStream->print(val);
      }
    }
    else
    {
      if (ConvFunc)
      {
        OutputStream->print(ConvFunc(val), DecimalPlaces);
      }
      else
      {
        OutputStream->print(val, DecimalPlaces);
      }
    }
  }
  else
    OutputStream->print("not available");
  if (AddLf)
    OutputStream->println();
#endif
}

// *****************************************************************************
// Init the NMEA2000 Interface
int8_t initN2K(void)
{
#if defined(DEBUG_ERROR) || defined(DEBUG_NSK_MSG)
  OutputStream = &Serial;
  // Set Forward stream to Serial
  NMEA2000.SetForwardStream(OutputStream);
  // Set false below, if you do not want to see messages parsed to HEX withing library
  NMEA2000.EnableForward(DEBUG_RAW_N2K_MESSAGES);
#endif

  // NMEA2000.SetN2kCANReceiveFrameBufSize(50);
  // Do not forward bus messages at all
  NMEA2000.SetForwardType(tNMEA2000::fwdt_Text);
  NMEA2000.SetMsgHandler(HandleNMEA2000Msg);
  //  NMEA2000.SetN2kCANMsgBufSize(2);
  NMEA2000.Open();

#ifdef DEBUG_NSK_MSG
  OutputStream->print("NMEA2000 Interface Running...\n");
#endif

  // Success
  return 0;
}

//*****************************************************************************
// NMEA 2000 message handler
void HandleNMEA2000Msg(const tN2kMsg &N2kMsg)
{
  int iHandler;

// Only if Debug is enabled
#ifdef DEBUG_NSK_MSG
  // Tread safety with mutex SerialOutputMutex
  if (SerialOutputMutex)
  {
    if (xSemaphoreTake(SerialOutputMutex, pdMS_TO_TICKS(20)) == pdTRUE)
    {
      OutputStream->print(millis());
      OutputStream->print(": In Main Handler: ");
      OutputStream->println(N2kMsg.PGN);
      xSemaphoreGive(SerialOutputMutex);
    }
  }
#endif

  // Find handler in the list till PGN is 0 (end of list) or PGN is found
  for (iHandler = 0; NMEA2000Handlers[iHandler].PGN != 0 && !(N2kMsg.PGN == NMEA2000Handlers[iHandler].PGN); iHandler++)
    ;

  // Call handler if found
  if (NMEA2000Handlers[iHandler].PGN != 0)
  {
    NMEA2000Handlers[iHandler].Handler(N2kMsg);
  }
}

//*****************************************************************************
void EngineRapid(const tN2kMsg &N2kMsg)
{
  unsigned char EngineInstance;
  double EngineSpeed;
  double EngineBoostPressure;
  int8_t EngineTiltTrim;

  if (ParseN2kEngineParamRapid(N2kMsg, EngineInstance, EngineSpeed, EngineBoostPressure, EngineTiltTrim))
  {
    // Update N2k Statistics
    N2kMessageStatistics.UpdateMsgCnt(N2kMsg.PGN);
// Only if Debug is enabled
#ifdef DEBUG_NSK_MSG
    // Tread safety with mutex SerialOutputMutex
    if (SerialOutputMutex)
    {
      if (xSemaphoreTake(SerialOutputMutex, pdMS_TO_TICKS(20)) == pdTRUE)
      {
        OutputStream->print(millis());
        OutputStream->print(": ");
        PrintLabelValWithConversionCheckUnDef("Engine rapid params: ", EngineInstance, 0, true);
        PrintLabelValWithConversionCheckUnDef("  RPM: ", EngineSpeed, 0, true);
        PrintLabelValWithConversionCheckUnDef("  boost pressure (Pa): ", EngineBoostPressure, 0, true);
        PrintLabelValWithConversionCheckUnDef("  tilt trim: ", EngineTiltTrim, 0, true);
        // free the mutex
        xSemaphoreGive(SerialOutputMutex);
      }
    }
#endif
    // Update the display data if the engine instance is the one to be displayed
    if (EngineInstance == DISPLAY_ENGINE_INSTANCE)
    {
      // Update the display data
      DisplayData.EngineSpeed = EngineSpeed;
    }
  }
  else
  {
    // Update N2k Statistics
    N2kMessageStatistics.IncreaseFailedMsgCnt(N2kMsg.PGN);
    // Only if Debug enabled
    PrintFailedToParsePGN(N2kMsg.PGN);
  }
}

//*****************************************************************************
void EngineDynamicParameters(const tN2kMsg &N2kMsg)
{
  unsigned char EngineInstance;
  double EngineOilPress;
  double EngineOilTemp;
  double EngineCoolantTemp;
  double AlternatorVoltage;
  double FuelRate;
  double EngineHours;
  double EngineCoolantPress;
  double EngineFuelPress;
  int8_t EngineLoad;
  int8_t EngineTorque;
  tN2kEngineDiscreteStatus1 Status1;
  tN2kEngineDiscreteStatus2 Status2;

  if (ParseN2kEngineDynamicParam(N2kMsg, EngineInstance, EngineOilPress, EngineOilTemp, EngineCoolantTemp,
                                 AlternatorVoltage, FuelRate, EngineHours,
                                 EngineCoolantPress, EngineFuelPress,
                                 EngineLoad, EngineTorque, Status1, Status2))
  {
    // Update N2k Statistics
    N2kMessageStatistics.UpdateMsgCnt(N2kMsg.PGN);
// Only if Debug is enabled
#ifdef DEBUG_NSK_MSG
    // Tread safety with mutex SerialOutputMutex
    if (SerialOutputMutex)
    {
      if (xSemaphoreTake(SerialOutputMutex, pdMS_TO_TICKS(20)) == pdTRUE)
      {
        OutputStream->print(millis());
        OutputStream->print(": ");
        PrintLabelValWithConversionCheckUnDef("Engine dynamic params: ", EngineInstance, 0, true);
        PrintLabelValWithConversionCheckUnDef("  oil pressure (Pa): ", EngineOilPress, 0, true);
        PrintLabelValWithConversionCheckUnDef("  oil temp (C): ", EngineOilTemp, &KelvinToC, true);
        PrintLabelValWithConversionCheckUnDef("  coolant temp (C): ", EngineCoolantTemp, &KelvinToC, true);
        PrintLabelValWithConversionCheckUnDef("  altenator voltage (V): ", AlternatorVoltage, 0, true);
        PrintLabelValWithConversionCheckUnDef("  fuel rate (l/h): ", FuelRate, 0, true);
        PrintLabelValWithConversionCheckUnDef("  engine hours (h): ", EngineHours, &SecondsToh, true);
        PrintLabelValWithConversionCheckUnDef("  coolant pressure (Pa): ", EngineCoolantPress, 0, true);
        PrintLabelValWithConversionCheckUnDef("  fuel pressure (Pa): ", EngineFuelPress, 0, true);
        PrintLabelValWithConversionCheckUnDef("  engine load (%): ", EngineLoad, 0, true);
        PrintLabelValWithConversionCheckUnDef("  engine torque (%): ", EngineTorque, 0, true);

        // free the mutex
        xSemaphoreGive(SerialOutputMutex);
      }
    }
#endif

    // Update the display data if the engine instance is the one to be displayed
    if (EngineInstance == DISPLAY_ENGINE_INSTANCE)
    {
      DisplayData.EngineHours = SecondsToh(EngineHours);
      DisplayData.EngineOilPressure = PascalTomBar(EngineOilPress);
      DisplayData.EngineCoolantTemperature = KelvinToC(EngineCoolantTemp);
      DisplayData.EngineAlternatorVoltage = AlternatorVoltage;
      DisplayData.EngineDiscreteStatus1 = Status1;
      DisplayData.EngineDiscreteStatus2 = Status2;
      DisplayData.LowOilPressureWarning = Status1.Bits.LowOilPressure;
    }
  }
  else
  {
    // Update N2k Statistics
    N2kMessageStatistics.IncreaseFailedMsgCnt(N2kMsg.PGN);
    // Only if Debug is enabled
    PrintFailedToParsePGN(N2kMsg.PGN);
  }
}

//*****************************************************************************
void TransmissionParameters(const tN2kMsg &N2kMsg)
{
  unsigned char EngineInstance;
  tN2kTransmissionGear TransmissionGear;
  double OilPressure;
  double OilTemperature;
  unsigned char DiscreteStatus1;

  if (ParseN2kTransmissionParameters(N2kMsg, EngineInstance, TransmissionGear, OilPressure, OilTemperature, DiscreteStatus1))
  {
    // Update N2k Statistics
    N2kMessageStatistics.UpdateMsgCnt(N2kMsg.PGN);
// Only if Debug is enabled
#ifdef DEBUG_NSK_MSG
    if (OutputStream)
    {
      // Tread safety with mutex SerialOutputMutex
      if (SerialOutputMutex)
      {
        if (xSemaphoreTake(SerialOutputMutex, pdMS_TO_TICKS(20)) == pdTRUE)
        {
          OutputStream->print(millis());
          OutputStream->print(": ");
          PrintLabelValWithConversionCheckUnDef("Transmission params: ", EngineInstance, 0, true);
          OutputStream->print("  gear: ");
          PrintN2kEnumType(TransmissionGear, OutputStream);
          PrintLabelValWithConversionCheckUnDef("  oil pressure (Pa): ", OilPressure, 0, true);
          PrintLabelValWithConversionCheckUnDef("  oil temperature (C): ", OilTemperature, &KelvinToC, true);
          PrintLabelValWithConversionCheckUnDef("  discrete status: ", DiscreteStatus1, 0, true);
          // free the mutex
          xSemaphoreGive(SerialOutputMutex);
        }
      }
    }
#endif
  }
  else
  {
    // Update N2k Statistics
    N2kMessageStatistics.IncreaseFailedMsgCnt(N2kMsg.PGN);
    // Only if Debug is enabled
    PrintFailedToParsePGN(N2kMsg.PGN);
  }
}

//*****************************************************************************
void SystemTime(const tN2kMsg &N2kMsg)
{
  unsigned char SID;
  uint16_t SystemDate;
  double SystemTime;
  tN2kTimeSource TimeSource; // Declare TimeSource variable
  if (ParseN2kSystemTime(N2kMsg, SID, SystemDate, SystemTime, TimeSource))
  {
    // Validate parsed values
    if (SystemDate > 0 && SystemTime >= 0.0 && SystemTime < 86400.0)
    {
      // Update N2k Statistics
      N2kMessageStatistics.UpdateMsgCnt(N2kMsg.PGN);
// Only if Debug is enabled
#ifdef DEBUG_NSK_MSG
      // Tread safety with mutex SerialOutputMutex
      if (SerialOutputMutex)
      {
        if (xSemaphoreTake(SerialOutputMutex, pdMS_TO_TICKS(20)) == pdTRUE)
        {
          OutputStream->print(millis());
          OutputStream->print(": ");
          OutputStream->println("System time:");
          PrintLabelValWithConversionCheckUnDef("  SID: ", SID, 0, true);
          PrintLabelValWithConversionCheckUnDef("  days since 1.1.1970: ", SystemDate, 0, true);
          PrintLabelValWithConversionCheckUnDef("  seconds since midnight: ", SystemTime, 0, true);
          OutputStream->print("  time source: ");
          PrintN2kEnumType(TimeSource, OutputStream);
          // free the mutex
          xSemaphoreGive(SerialOutputMutex);
        }
      }

#endif
    }
    else
    {
// Only if Debug is enabled
#ifdef DEBUG_ERROR
      // Tread safety with mutex SerialOutputMutex
      if (SerialOutputMutex)
      {
        if (xSemaphoreTake(SerialOutputMutex, pdMS_TO_TICKS(20)) == pdTRUE)
        {
          OutputStream->println("Invalid system time data received.");
          // free the mutex
          xSemaphoreGive(SerialOutputMutex);
        }
      }
#endif
    }
  }
  else
  {
    // Update N2k Statistics
    N2kMessageStatistics.IncreaseFailedMsgCnt(N2kMsg.PGN);
    // Only if Debug is enabled
    PrintFailedToParsePGN(N2kMsg.PGN);
  }
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ----------------------> Message Statistics <-------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//************************************************
// Constructor
N2kMsgStatistics::N2kMsgStatistics()
{

  // Init the statistics
  SystemTimeCnt = 0;
  EngineRapidCnt = 0;
  EngineDynamicCnt = 0;
  TransmissionParametersCnt = 0;

  SystemTimeFailedCnt = 0;
  EngineRapidFailedCnt = 0;
  EngineDynamicFailedCnt = 0;
  TransmissionParametersFailedCnt = 0;

  SystemTimeLastTimestamp = 0;
  EngineRapidLastTimestamp = 0;
  EngineDynamicLastTimestamp = 0;
  TransmissionParametersLastTimestamp = 0;
}

//************************************************
// Destructor
N2kMsgStatistics::~N2kMsgStatistics()
{
}

//************************************************
// Update the message counter
void N2kMsgStatistics::UpdateMsgCnt(uint32_t PGN, uint8_t Instance)
{
  switch (PGN)
  {
    // System Time Message
  case 126992L:
    SystemTimeCnt++;
    SystemTimeLastTimestamp = millis();
    break;

    // Engine Rapid Message
  case 127488L:
    // Check for the correct instance
    if ((Instance == DISPLAY_ENGINE_INSTANCE) || (Instance == 0))
    {
      EngineRapidCnt++;
      EngineRapidLastTimestamp = millis();
    }

    break;

    // Engine Dynamic Message
  case 127489L:
    // Check for the correct instance
    if ((Instance == DISPLAY_ENGINE_INSTANCE) || (Instance == 0))
    {
      EngineDynamicCnt++;
      EngineDynamicLastTimestamp = millis();
    }
    break;

    // Transmission Parameters Message
  case 127493L:
    TransmissionParametersCnt++;
    TransmissionParametersLastTimestamp = millis();
    break;
  default:
    break;
  }
}

//************************************************
// Increase the failed message counter
void N2kMsgStatistics::IncreaseFailedMsgCnt(uint32_t PGN)
{
  switch (PGN)
  {
    // System Time Message
  case 126992L:
    SystemTimeFailedCnt++;
    break;

    // Engine Rapid Message
  case 127488L:
    EngineRapidFailedCnt++;
    break;

    // Engine Dynamic Message
  case 127489L:
    EngineDynamicFailedCnt++;
    break;

    // Transmission Parameters Message
  case 127493L:
    TransmissionParametersFailedCnt++;
    break;
  default:
    break;
  }
}

//************************************************
// Get the message counter
uint32_t N2kMsgStatistics::GetMsgCnt(uint32_t PGN)
{
  switch (PGN)
  {
    // System Time Message
  case 126992L:
    return SystemTimeCnt;
    break;

    // Engine Rapid Message
  case 127488L:
    return EngineRapidCnt;
    break;

    // Engine Dynamic Message
  case 127489L:
    return EngineDynamicCnt;
    break;

    // Transmission Parameters Message
  case 127493L:
    return TransmissionParametersCnt;
    break;
  default:
    return 0;
    break;
  }
}

//************************************************
// Get the failed message counter
uint32_t N2kMsgStatistics::GetFailedMsgCnt(uint32_t PGN)
{
  switch (PGN)
  {
    // System Time Message
  case 126992L:
    return SystemTimeFailedCnt;
    break;

    // Engine Rapid Message
  case 127488L:
    return EngineRapidFailedCnt;
    break;

    // Engine Dynamic Message
  case 127489L:
    return EngineDynamicFailedCnt;
    break;

    // Transmission Parameters Message
  case 127493L:
    return TransmissionParametersFailedCnt;
    break;
  default:
    return 0;
    break;
  }
}

//************************************************
// Check if the N2K message is timed out
bool N2kMsgStatistics::N2kIsTimeOut(void)
{
  if ((millis() - EngineRapidLastTimestamp) > N2K_MSG_ENGINE_RAPID_TIMEOUT)
  {
    return true;
  }
  else
  {
    return false;
  }
}

//************************************************
// Show the N2K message statistics
void N2kMsgStatistics::ShowStatistics(void)
{
  // Only if Debug is enabled
#ifdef DEBUG_N2K_MSG_STATISTICS
  // Tread safety with mutex SerialOutputMutex
  if (SerialOutputMutex)
  {
    if (xSemaphoreTake(SerialOutputMutex, pdMS_TO_TICKS(20)) == pdTRUE)
    {
      // Show the statistics via Serial
      Serial.println("N2K Message Statistics:");
      Serial.print("  System Time: ");
      Serial.print(SystemTimeCnt);
      Serial.print(" (failed: ");
      Serial.print(SystemTimeFailedCnt);
      Serial.println(")");
      Serial.print("  Engine Rapid: ");
      Serial.print(EngineRapidCnt);
      Serial.print(" (failed: ");
      Serial.print(EngineRapidFailedCnt);
      Serial.print(") Last: ");
      Serial.print(millis() - EngineRapidLastTimestamp);
      Serial.println("ms ago");
      Serial.print("  Engine Dynamic: ");
      Serial.print(EngineDynamicCnt);
      Serial.print(" (failed: ");
      Serial.print(EngineDynamicFailedCnt);
      Serial.println(")");
      Serial.print("  Transmission Parameters: ");
      Serial.print(TransmissionParametersCnt);
      Serial.print(" (failed: ");
      Serial.print(TransmissionParametersFailedCnt);
      Serial.println(")");
      // check if Engine Rapid is timed out
      if (N2kIsTimeOut())
      {
        Serial.println("  ---> Engine Rapid Message is timed out!");
      }
      else
      {
        Serial.println("  ---> Nsk Messages are coming in time");
      }

      // free the mutex
      xSemaphoreGive(SerialOutputMutex);
    }
  }

#endif
}
