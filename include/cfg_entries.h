#ifndef _CFG_ENTRIES_H_
#define _CFG_ENTRIES_H_

#include <Arduino.h>

struct CfgEntries
{
    String deviceId;
    String authToken;
    uint32_t workDelay;
    uint16_t updateDist;
};

#endif // _CFG_ENTRIES_H_
