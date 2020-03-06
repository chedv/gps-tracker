#ifndef _SIM808_H_
#define _SIM808_H_

#include <SoftwareSerial.h>
#include <entries.h>

//-------------------------------------------------------------------------

namespace SIM808 {

//-------------------------------------------------------------------------

extern SoftwareSerial sim808;

//-------------------------------------------------------------------------

void init();

//-------------------------------------------------------------------------

bool gpsAvailable();

void gpsRead(Entries * entries);

//-------------------------------------------------------------------------

bool gprsAvailable();

void gprsSendRequest(const Entries * entries);

//-------------------------------------------------------------------------

void sendCommand(const char * command);

void sendCommand(const char * command, char * buffer, uint8_t bufferSize);

//-------------------------------------------------------------------------

void awaitResponse(char * buffer, uint8_t bufferSize);

void formatResponse(char * response, uint8_t responseSize);

void formatGpsReponse(const char * messageType, char * response, uint8_t responseSize);

//-------------------------------------------------------------------------

bool checkStatus(const char * command, const char * status);

//-------------------------------------------------------------------------

}; // namespace SIM808

//-------------------------------------------------------------------------

inline bool SIM808::gpsAvailable()
{
    return checkStatus("AT+CGPSSTATUS?", "Location 3D Fix");
}

inline bool SIM808::gprsAvailable()
{
    return checkStatus("AT+CPIN?", "READY");
}

//-------------------------------------------------------------------------

#endif // _SIM808_H_