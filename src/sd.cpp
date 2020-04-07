#include <sd.h>

FATFS SdModule::fs;

uint8_t SdModule::init()
{
    FRESULT result = {};

    const uint32_t timeout = millis() + 1000;
    while ((result = pf_mount(&fs)) != FR_OK && millis() < timeout);

    return result;
}

uint8_t SdModule::read(const char * path, char * buffer, uint8_t bufferSize)
{
    UINT br = {};

    if (pf_open(path) == FR_OK)
        pf_read(buffer, bufferSize, &br);
    
    return br;
}