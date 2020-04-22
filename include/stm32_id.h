#ifndef _STM32_ID_H_
#define _STM32_ID_H_

#include "config.h"

inline uint32_t stm32Number(uint8_t shift)
{
    return *(((uint32_t *)STM_ID_ADDRESS) + shift);
}

inline String stm32Id()
{
    return String(stm32Number(1), HEX) + String(stm32Number(2), HEX);
}

#endif // _STM32_ID_H_