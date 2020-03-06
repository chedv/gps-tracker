#ifndef _STR_H_
#define _STR_H_

#include <Arduino.h>

//-------------------------------------------------------------------------

namespace str {

//-------------------------------------------------------------------------

bool empty(const char * str);

uint8_t indexOf(const char * str, char chr);

void substr(const char * str, char * buffer, uint8_t pos);

void substr(const char * str, char * buffer, uint8_t left, uint8_t right);

//-------------------------------------------------------------------------

}; // namespace str

//-------------------------------------------------------------------------

inline bool str::empty(const char * str)
{
    return strlen(str) == 0;
}

inline uint8_t str::indexOf(const char * str, char chr)
{
    return strchr(str, chr) - str;
}

//-------------------------------------------------------------------------

#endif // _STR_H_