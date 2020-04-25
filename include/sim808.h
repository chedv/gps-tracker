#ifndef _SIM808_H_
#define _SIM808_H_

#include "cfg_entries.h"
#include "gps_entries.h"

class Sim808 
{
public:

    void init(HardwareSerial & serial, uint32_t speed);

    bool gpsAvailable();
    bool gpsRead(const CfgEntries & cfgEntries, GpsEntries & gpsEntries);

    bool gprsAvailable();
    bool gprsSendLocation(const CfgEntries & cfgEntries, const String & gpsEntries);

private:

    String sendCommand(const String & command, bool parse = false);
    void sendCommandNoWait(const String & command);

    bool awaitResponse(uint32_t timeout = 10000);
    String readResponse(bool parse);

    String readNmeaSentence(uint8_t sentenceCode);
    uint16_t readHttpStatusCode();

    HardwareSerial * sim808;
};

#endif // _SIM808_H_