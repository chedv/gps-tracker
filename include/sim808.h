#ifndef _SIM808_H_
#define _SIM808_H_

#include "cfg_entries.h"
#include "gps_entries.h"

class Sim808 
{
public:

    void init(HardwareSerial & serial, uint32_t speed);

    bool gpsAvailable();
    bool gpsRead(GpsEntries & gpsEntries);

    bool gprsAvailable();
    bool gprsSendLocation(const CfgEntries & cfgEntries, const GpsEntries & gpsEntries);

private:

    static String parseResponse(const String & response);

    void sendCommandNoWait(const String & command);
    String sendCommand(const String & command, bool edit = false);

    bool awaitResponse(uint32_t timeout = 10000);
    String readResponse();

    String readNmeaSentence(uint8_t sentenceCode);
    uint16_t readHttpStatusCode();

    HardwareSerial * sim808;
};

#endif // _SIM808_H_