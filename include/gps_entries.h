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
    return String(latitude) + ' ' + String(longitude) + ' ' + date.toString() + ' ' + time.toString();
}

inline String GpsEntries::toJson() const
{
    return "{ \"latitude\": " + String(latitude) + ", \"longitude\": " + String(longitude)
        + ", \"datetime\": \"" + date.toString() + ' ' + time.toString() + "\" }";
}

#endif // _GPS_ENTRIES_H_
