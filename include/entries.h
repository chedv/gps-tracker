#ifndef _ENTRIES_H_
#define _ENTRIES_H_

#include <Arduino.h>

const uint8_t DEVICE_ID_SIZE = 16;
const uint8_t AUTH_TOKEN_SIZE = 40;
const uint8_t GPS_ENTRY_SIZE = 82;

struct Entries 
{
    uint32_t timer;
    uint16_t workDelay = 30000;
    
    char deviceId[DEVICE_ID_SIZE + 1];
    char authToken[AUTH_TOKEN_SIZE + 1];
    char gpsEntry[GPS_ENTRY_SIZE + 1];

}; // struct Entries

extern Entries entries;

#endif // _ENTRIES_H_