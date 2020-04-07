#ifndef _PORTS_H_
#define _PORTS_H_

#include <Arduino.h>

const uint8_t SD_spiPort = 2;

const uint8_t SD_csPin = PB12;

const uint32_t STM_address = 0x1FFFF7E8;

const uint32_t STM_defaultDelay = 60000;

const uint32_t USB_serialSpeed = 9600;

const uint32_t SIM_serialSpeed = 9600;

HardwareSerial & SIM_serial = Serial3;

#endif // _PORTS_H_