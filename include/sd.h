#ifndef _SD_H_
#define _SD_H_

#include <Arduino.h>
#include <PetitFS.h>
#include <SoftwareSerial.h>

namespace SdModule 
{

extern FATFS fs;

uint8_t init();

uint8_t read(const char * path, char * buffer, uint8_t bufferSize);

}; // namespace SdModule

#endif // _SD_H_