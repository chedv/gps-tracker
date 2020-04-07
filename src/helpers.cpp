#include "helpers.h"
#include <avr/boot.h>
#include <sd.h>
#include <str.h>

void Helpers::readConfig(Entries * entries)
{
    const uint8_t bufferSize = 80;
    char fileBuffer[bufferSize + 1] = { 0 };

    SdModule::read("CONFIG.TXT", fileBuffer, bufferSize);
    
    char * split = strtok(fileBuffer, "\n");
	while (split != 0)
	{
        uint8_t pos = {};

        if ((pos = str::indexOf(split, '=')) != -1)
        {
            const uint8_t paramNameSize = 10;
            char paramNameBuffer[paramNameSize + 1] = { 0 };

            str::substr(split, paramNameBuffer, 0, pos);

            const uint8_t resultSize = 40;
            char resultBuffer[resultSize + 1] = { 0 };

            if (strcmp(paramNameBuffer, "work_delay") == 0)
            {
                str::substr(split, resultBuffer, pos + 1);
                uint32_t workDelay = atoi(resultBuffer);

                if (workDelay >= 0 && workDelay <= 600000)
                    entries->workDelay = workDelay;
            }
            else if (strcmp(paramNameBuffer, "auth_token") == 0)
            {
                str::substr(split, resultBuffer, pos + 1);
                strncpy(entries->authToken, resultBuffer, AUTH_TOKEN_SIZE);
            }
        }
		split = strtok(0, "\n");
	}
}

void Helpers::createIdentifier(Entries * entries)
{
    uint8_t resultSize = 32;
    char result[resultSize + 1] = { 0 };

    for (uint8_t i = 0; i < 16; ++i)
    {
        const uint8_t value = boot_signature_byte_get(i);

        if (value < 0x10)
            strcat(result, "0");
        
        const uint8_t digitsCount = 2;
        char digits[digitsCount + 1] = { 0 };

        itoa(value, digits, HEX);

        for (uint8_t i = 0; i < digitsCount; i++)
            digits[i] = toupper(digits[i]);

        strcat(result, digits);
    }
    strncpy(entries->deviceId, result, DEVICE_ID_SIZE);
}