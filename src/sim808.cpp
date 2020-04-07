#include "sim808.h"
#include <TinyGPS++.h>

void Sim808::init(HardwareSerial & serial, uint32_t speed)
{
    sim808 = &serial;
    sim808->begin(speed);
}

void Sim808::gpsStart()
{
    sendCommand("AT+CGPSPWR=1");
    sendCommand("AT+CGPSRST=1");
}

void Sim808::gpsRead(GpsEntries & entries)
{
    const char * nmeaSentences = String(
        gpsReadNmea(0, "$GPGGA") + "\r\n" +
        gpsReadNmea(32, "$GPRMC") + "\r\n"
    ).c_str();

    TinyGPSPlus parser;

    while(*nmeaSentences)
        parser.encode(*nmeaSentences++);

    if (parser.location.isValid() && parser.date.isValid() && parser.time.isValid())
    {
        entries = {
            parser.location.lat(), 
            parser.location.lng(),
            { parser.date.year(), parser.date.month(), parser.date.day() },
            { parser.time.hour(), parser.time.minute(), parser.time.second() },
            .initialized = true
        };
    }
}

void Sim808::gprsSendLocation(const CfgEntries & cfgEntries, const GpsEntries & gpsEntries)
{
    sendCommand("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");
    sendCommand("AT+SAPBR=3,1,\"APN\",\"internet\"");
    sendCommand("AT+SAPBR=1,1");
    sendCommand("AT+HTTPINIT");
    sendCommand("AT+HTTPPARA=\"CID\",1");

    sendCommand("AT+HTTPPARA=\"URL\",\"http://chedv001.pythonanywhere.com/devices/" 
            + cfgEntries.deviceId + "/entries\"");

    sendCommand("AT+HTTPPARA=\"USERDATA\",\"Authorization: Token "
            + cfgEntries.authToken + "\"");

    sendCommand("AT+HTTPPARA=\"CONTENT\",\"application/json\"");

    String entries = gpsEntries.toJson();
    sendCommand("AT+HTTPDATA=" + String(entries.length()) + ",10000");

    sendCommand(entries);
    Serial.println(entries);

    sendCommand("AT+HTTPACTION=1");

    awaitResponse();

    sendCommand("AT+HTTPTERM");
    sendCommand("AT+SAPBR=0,1");
}

String Sim808::sendCommand(const String & command)
{
    sim808->println(command);
    Serial.println(command);

    return awaitResponse();
}

String Sim808::awaitResponse()
{
    const uint32_t timeout = millis() + 10000;
    while(!sim808->available() && millis() < timeout);

    if (sim808->available())
        return sim808->readString();
    return "timeout";
}

void Sim808::formatResponse(String & response) const
{
    uint8_t pos;

    if ((pos = response.indexOf(':')) != -1)
        response = response.substring(pos + 2);
    if ((pos = response.indexOf("\r\n")) != -1)
        response = response.substring(0, pos);
    
    Serial.println(response);
}

void Sim808::formatGpsReponse(const char * messageType, String & response) const
{
    uint8_t pos;

    if ((pos = response.indexOf(',')) != -1)
        response = messageType + response.substring(pos);
}

bool Sim808::checkState(const char * command, const char * status)
{
    String response = sendCommand(command);

    formatResponse(response);

    return response.equalsIgnoreCase(status);
}

String Sim808::gpsReadNmea(uint8_t number, const char * type)
{
    String response = sendCommand("AT+CGPSINF=" + number);

    formatResponse(response);
    formatGpsReponse(type, response);

    return response;
}