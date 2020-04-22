#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <Arduino.h>

const uint8_t SD_SPI_PORT = 2;

const uint8_t SD_CS_PIN = PB12;

const uint32_t STM_ID_ADDRESS = 0x1FFFF7E8;

const uint32_t STM_DEFAULT_DELAY = 60000;

const uint32_t USB_SERIAL_SPEED = 9600;

const uint32_t SIM_SERIAL_SPEED = 9600;

HardwareSerial & SIM_SERIAL = Serial3;

#endif // _CONFIG_H_