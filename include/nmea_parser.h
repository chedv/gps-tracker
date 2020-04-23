#ifndef _NMEA_PARSER_H_
#define _NMEA_PARSER_H_

#include <TinyGPS++.h>
#include "gps_entries.h"

class NmeaParser
{
public:

    void parseSentence(const String & sentence);

    bool isValid();

    GpsEntries getResult();

private:

    TinyGPSPlus tinyGps;
};

#endif // _NMEA_PARSER_H_