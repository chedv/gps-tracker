#ifndef _GPS_ENTRIES_H_
#define _GPS_ENTRIES_H_

#include "datetime.h"

struct GpsEntries
{
    String toJson() const;

    double latitude;
    double longitude;
    DateTime datetime;
    uint8_t satellites;
};

inline String GpsEntries::toJson() const
{
    return "{ \"latitude\": " + String(latitude, 6) + ", \"longitude\": " + String(longitude, 6)
        + ", \"datetime\": \"" + datetime.toString() + "\"" + ", \"satellites\": " + satellites + " }";
}

#endif // _GPS_ENTRIES_H_