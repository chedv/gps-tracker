#include "sim808.h"
#include <TinyGPS++.h>

void Sim808::init(HardwareSerial & serial, uint32_t speed)
{
    sim808 = &serial;
    sim808->begin(speed);
}

bool Sim808::gpsAvailable()
{
    if (sendCommand("AT+CGPSPWR?", true).toInt() == 1)
    {
        return sendCommand("AT+CGPSSTATUS?", true)
            .equalsIgnoreCase("Location 3D Fix");
    }
    sendCommand("AT+CGPSPWR=1");
    return false;
}

void Sim808::gpsRead(GpsEntries & entries)
{
    const uint8_t sentenceCode = 32;
    const char * nmeaSentence = readNmeaSentence(sentenceCode).c_str();
    TinyGPSPlus parser;

    while(*nmeaSentence)
    {
        if (parser.encode(*nmeaSentence++))
        {
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
    }
}

bool Sim808::gprsAvailable()
{
    const String response = sendCommand("AT+CGREG?", true);

    const uint8_t pos = response.indexOf(',');
    if (pos >= 0 && pos < response.length() - 1)
    {
        const uint8_t statusCode = response.substring(pos + 1).toInt();
        return statusCode == 0 || statusCode == 5;
    }
    return false;
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
    Serial.println(readResponse());

    sendCommand("AT+HTTPTERM");
    sendCommand("AT+SAPBR=0,1");
}

void Sim808::sendCommandNoWait(const String & command)
{
    sim808->println(command);
    Serial.println(command);
}

String Sim808::sendCommand(const String & command, bool parse)
{
    sendCommandNoWait(command);

    String response = readResponse();
    return parse ? parseResponse(response) : response;
}

bool Sim808::awaitResponse(uint32_t timeout)
{
    const uint32_t timeDelay = millis() + timeout;
    while(!sim808->available() && millis() < timeDelay);

    return sim808->available();
}

String Sim808::readResponse()
{
    return awaitResponse() ? sim808->readString() : "timeout";
}

String Sim808::readNmeaSentence(uint8_t sentenceCode)
{
    sendCommandNoWait("AT+CGPSOUT=" + String(sentenceCode));
    String response = "timeout";

    if (awaitResponse())
    {
        sim808->readStringUntil('$');
        response = '$' + sim808->readStringUntil('\n') + '\n';
    }
    sendCommand("AT+CGPSOUT=0");
    return response;
}

String Sim808::parseResponse(const String & response)
{
    String result = response;
    uint8_t pos;

    if ((pos = result.indexOf(':')) != -1)
        result = result.substring(pos + 2);
    if ((pos = result.indexOf("\r\n")) != -1)
        result = result.substring(0, pos);
    
    Serial.println(result);
    return result;
}