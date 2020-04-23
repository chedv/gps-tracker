#include "nmea_parser.h"

void NmeaParser::parseSentence(const String & sentence)
{
    const char * chars = sentence.c_str();

    while(*chars)
        if (tinyGps.encode(*chars++))
            break;
}

bool NmeaParser::isValid()
{
    return tinyGps.location.isValid()
        && tinyGps.date.isValid()
        && tinyGps.time.isValid()
        && tinyGps.satellites.isValid();
}

GpsEntries NmeaParser::getResult()
{
    return { 
        tinyGps.location.lat(), tinyGps.location.lng(), 
        {
            tinyGps.date.year(), tinyGps.date.month(), tinyGps.date.day(), 
            tinyGps.time.hour(), tinyGps.time.minute(), tinyGps.time.second() 
        },
        tinyGps.satellites.value()
    };
}