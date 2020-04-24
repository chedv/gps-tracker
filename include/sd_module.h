#ifndef _SD_MODULE_H_
#define _SD_MODULE_H_

#include "cfg_entries.h"
#include "gps_entries.h"
#include "sim808.h"

#include <SPI.h>
#include <SD.h>

class SdModule
{
public:

    void init(uint8_t spiPort, uint8_t csPin) const;

    void readConfig(CfgEntries & cfgEntries) const;

    void writeLocation(const GpsEntries & gpsEntries) const;

    void restoreLocation(const CfgEntries & cfgEntries, Sim808 & sim808) const;
};

#endif // _SD_MODULE_H_