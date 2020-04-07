#ifndef _DATETIME_H_
#define _DATETIME_H_

#include <Arduino.h>

struct Date
{
    String toString() const;

    uint16_t year;
    uint8_t month;
    uint8_t day;
};

struct Time
{
    String toString() const;

    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
};

inline String convert(int number)
{
    return number >= 10 ? String(number) : '0' + String(number);
}

inline String Date::toString() const
{
    return convert(month) + '/' + convert(day) + '/' + convert(year);
}

inline String Time::toString() const
{
    return convert(hours) + ':' + convert(minutes) + ':' + convert(seconds);
}

#endif // _DATETIME_H_