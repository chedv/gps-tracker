#ifndef _GPS_ENTRIES_H_
#define _GPS_ENTRIES_H_

#include "datetime.h"

struct GpsEntries
{
    String toString() const;
    String toJson() const;

    double latitude;
    double longitude;

    Date date;
    Time time;

    bool initialized;
};

inline String GpsEntries::toString() const
{
    return String(latitude, 6) + ' ' + String(longitude, 6) + ' ' + date.toString() + ' ' + time.toString();
}

inline String GpsEntries::toJson() const
{
    return "{ \"latitude\": " + String(latitude, 6) + ", \"longitude\": " + String(longitude, 6)
        + ", \"datetime\": \"" + date.toString() + ' ' + time.toString() + "\" }";
}

#endif // _GPS_ENTRIES_H_
