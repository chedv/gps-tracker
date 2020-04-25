#include "sim808.h"
#include "nmea_parser.h"

void Sim808::init(HardwareSerial & serial, uint32_t speed)
{
    sim808 = &serial;
    sim808->begin(speed);
}

bool Sim808::gpsAvailable()
{
    if (sendCommand("AT+CGPSPWR?", true).toInt() == 1)
        return sendCommand("AT+CGPSSTATUS?", true).equalsIgnoreCase("Location 3D Fix");

    sendCommand("AT+CGPSPWR=1");
    return false;
}

bool Sim808::gpsRead(const CfgEntries & cfgEntries, GpsEntries & entries)
{
    static const uint8_t GPGGA = 2;
    static const uint8_t GPRMC = 32;

    NmeaParser parser;

    parser.parseSentence(readNmeaSentence(GPGGA));
    parser.parseSentence(readNmeaSentence(GPRMC));
    
    if (parser.isValid())
    {
        GpsEntries result = parser.getResult();

        const double updateDist = cfgEntries.updateDist;
        if (NmeaParser::isLocationUpdated(result, entries, updateDist))
        {
            entries = result;
            return true;
        }
    }
    return false;
}

bool Sim808::gprsAvailable()
{
    const String response = sendCommand("AT+CGREG?", true);

    const uint8_t pos = response.indexOf(',');
    if (pos >= 0 && pos < response.length() - 1)
    {
        static const uint8_t REGISTERED_STATUS = 1;
        static const uint8_t REGISTERED_ROAMING_STATUS = 5;

        const uint8_t statusCode = response.substring(pos + 1).toInt();
        return statusCode == REGISTERED_STATUS || statusCode == REGISTERED_ROAMING_STATUS;
    }
    return false;
}

bool Sim808::gprsSendLocation(const CfgEntries & cfgEntries, const String & gpsEntries)
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

    sendCommand("AT+HTTPDATA=" + String(gpsEntries.length()) + ",10000");
    sendCommand(gpsEntries);

    sendCommand("AT+HTTPACTION=1");
    uint16_t statusCode = readHttpStatusCode();

    sendCommand("AT+HTTPTERM");
    sendCommand("AT+SAPBR=0,1");

    static const uint16_t HTTP_200_OK = 200;
    static const uint16_t HTTP_201_CREATED = 201;

    return statusCode == HTTP_200_OK || statusCode == HTTP_201_CREATED;
}

String Sim808::sendCommand(const String & command, bool parse)
{
    sendCommandNoWait(command);
    return readResponse(parse);
}

void Sim808::sendCommandNoWait(const String & command)
{
    sim808->println(command);
    Serial.println(command);
}

bool Sim808::awaitResponse(uint32_t timeout)
{
    const uint32_t timeDelay = millis() + timeout;
    while(!sim808->available() && millis() < timeDelay);

    return sim808->available();
}

String Sim808::readResponse(bool parse)
{
    if (!awaitResponse())
        return "timeout";
    
    String response = sim808->readString();

    if (!parse)
        return response;

    uint8_t pos;

    if ((pos = response.indexOf(':')) != -1)
        response = response.substring(pos + 2);
    if ((pos = response.indexOf("\r\n")) != -1)
        response = response.substring(0, pos);

    Serial.println(response);
    return response;
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

uint16_t Sim808::readHttpStatusCode()
{
    String response = readResponse(true);

    uint8_t start = response.indexOf(',');
    uint8_t end = response.lastIndexOf(',');

    if (start != -1 && end != -1)
    {
        uint16_t statusCode = response.substring(start + 1, end).toInt();
        if (statusCode >= 100 && statusCode < 720)
            return statusCode;
    }
    return 0;
}