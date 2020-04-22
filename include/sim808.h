#ifndef _SIM808_H_
#define _SIM808_H_

#include "cfg_entries.h"
#include "gps_entries.h"

class Sim808 
{
public:

    void init(HardwareSerial & serial, uint32_t speed);

    bool gpsAvailable();
    void gpsRead(GpsEntries & gpsEntries);

    bool gprsAvailable();
    void gprsSendLocation(const CfgEntries & cfgEntries, const GpsEntries & gpsEntries);

private:

    void sendCommandNoWait(const String & command);
    String sendCommand(const String & command, bool edit = false);

    bool awaitResponse(uint32_t timeout = 10000);
    String readResponse();

    static String parseResponse(const String & response);

    String readNmeaSentence(uint8_t sentenceCode);

    HardwareSerial * sim808;
};

#endif // _SIM808_H_