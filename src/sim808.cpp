#include "sim808.h"
#include <str.h>

//-------------------------------------------------------------------------

const uint8_t rxPin = 3;
const uint8_t txPin = 2;
const uint16_t serialSpeed = 9600;

SoftwareSerial SIM808::sim808(rxPin, txPin);

//-------------------------------------------------------------------------

void SIM808::init()
{
    sim808.begin(serialSpeed);
    sim808.listen();

    sendCommand("AT+CGPSPWR=1");
    sendCommand("AT+CGPSRST=1");
}

//-------------------------------------------------------------------------

void SIM808::gpsRead(Entries * entries)
{
    const char * const command = "AT+CGPSINF=32";

    const uint8_t responseSize = 120;
    char response[responseSize + 1] = { 0 };

    sendCommand(command, response, responseSize);

    formatResponse(response, responseSize);
    formatGpsReponse("$GPRMC", response, responseSize);

    memset(entries->gpsEntry, 0, GPS_ENTRY_SIZE);
    strncpy(entries->gpsEntry, response, GPS_ENTRY_SIZE);
}

//-------------------------------------------------------------------------

void SIM808::gprsSendRequest(const Entries * entries)
{
    const uint8_t bufferSize = 120;
    char buffer[bufferSize + 1] = { 0 };

    sendCommand("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");
    sendCommand("AT+SAPBR=3,1,\"APN\",\"internet\"");
    sendCommand("AT+SAPBR=1,1");
    sendCommand("AT+HTTPINIT");
    sendCommand("AT+HTTPPARA=\"CID\",1");

    snprintf(buffer, bufferSize, 
            "AT+HTTPPARA=\"URL\",\"https://chedv001.pythonanywhere.com/devices"
            "/%s/entries\"", entries->deviceId);
    sendCommand(buffer);

    sendCommand("AT+HTTPSSL=1");

    memset(buffer, 0, bufferSize);
    snprintf(buffer, bufferSize, 
            "AT+HTTPPARA=\"Authorization\","
            "\"Token %s\"", entries->authToken);
    sendCommand(buffer);

    sendCommand("AT+HTTPPARA=\"CONTENT\",\"text/nmea\"");

    memset(buffer, 0, bufferSize);
    snprintf(buffer, bufferSize, "AT+HTTPDATA=%d,10000", strlen(entries->gpsEntry));
    sendCommand(buffer);

    sendCommand(entries->gpsEntry);
    sendCommand("AT+HTTPACTION=1");

    memset(buffer, 0, bufferSize);
    awaitResponse(buffer, bufferSize);

    sendCommand("AT+HTTPTERM");
    sendCommand("AT+SAPBR=0,1");
}

//-------------------------------------------------------------------------

void SIM808::sendCommand(const char * command)
{
    sim808.println(command);
    Serial.println(command);
}

void SIM808::sendCommand(const char * command, char * buffer, uint8_t bufferSize)
{
    sendCommand(command);
    
    memset(buffer, 0, bufferSize);
    awaitResponse(buffer, bufferSize);
}

//-------------------------------------------------------------------------

void SIM808::awaitResponse(char * buffer, uint8_t bufferSize)
{
    const uint32_t timeout = millis() + 10000;
    while(!sim808.available() && millis() < timeout);

    if (sim808.available())
        sim808.readBytes(buffer, bufferSize);
    else
        strncpy(buffer, "timeout", bufferSize);
    
    Serial.println(buffer);
}

void SIM808::formatResponse(char * response, uint8_t responseSize)
{
    char buffer[responseSize + 1] = { 0 };
    uint8_t pos = {};

    if ((pos = str::indexOf(response, ':')) != -1)
    {
        str::substr(response, buffer, pos + 2);

        memset(response, 0, responseSize);
        strncpy(response, buffer, responseSize);
    }
    if ((pos = strcspn(response, "\r\n")) != -1)
    {
        memset(buffer, 0, responseSize);
        
        str::substr(response, buffer, 0, pos);

        memset(response, 0, responseSize);
        strncpy(response, buffer, responseSize);
    }
    Serial.println(response);
}

void SIM808::formatGpsReponse(const char * messageType, char * response, uint8_t responseSize)
{
    uint8_t pos = {};

    if ((pos = str::indexOf(response, ',')) != -1)
    {
        char buffer[responseSize + 1] = { 0 };
        str::substr(response, buffer, pos);

        memset(response, 0, responseSize);
        strcat(response, messageType);
        strcat(response, buffer);
    }
    Serial.println(response);
}

//-------------------------------------------------------------------------

bool SIM808::checkStatus(const char * command, const char * status)
{
    uint8_t bufferSize = 120;
    char responseBuffer[bufferSize + 1] = { 0 };

    sendCommand(command, responseBuffer, bufferSize);
    formatResponse(responseBuffer, bufferSize);

    return strcmp(responseBuffer, status) == 0;
}

//-------------------------------------------------------------------------
