#ifndef _SIM808_H_
#define _SIM808_H_

#include "cfg_entries.h"
#include "gps_entries.h"

class Sim808 
{
public:

    void init(HardwareSerial & serial, uint32_t speed);

    void gpsStart();
    bool gpsAvailable();
    void gpsRead(GpsEntries & gpsEntries);

    bool gprsAvailable();
    void gprsSendLocation(const CfgEntries & cfgEntries, const GpsEntries & gpsEntries);

private:

    String sendCommand(const String & command);
    String awaitResponse();

    void formatResponse(String & response) const;
    void formatGpsReponse(const char * type, String & response) const;
    
    bool checkState(const char * command, const char * status);

    String gpsReadNmea(uint8_t number, const char * type);

    HardwareSerial * sim808;
};

inline bool Sim808::gpsAvailable()
{
    return checkState("AT+CGPSSTATUS?", "Location 3D Fix");
}

inline bool Sim808::gprsAvailable()
{
    return checkState("AT+CPIN?", "READY");
}

#endif // _SIM808_H_