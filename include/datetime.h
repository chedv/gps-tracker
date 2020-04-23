#ifndef _DATETIME_H_
#define _DATETIME_H_

#include <Arduino.h>

struct DateTime
{
    String toString() const;

    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
};

inline String convert(int number)
{
    return number >= 10 ? String(number) : '0' + String(number);
}

inline String DateTime::toString() const
{
    return convert(month) + '/' + convert(day) + '/' + convert(year) + 'T' 
        + convert(hours) + ':' + convert(minutes) + ':' + convert(seconds) + 'Z';
}

#endif // _DATETIME_H_