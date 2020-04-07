#include "str.h"

void str::substr(const char * str, char * buffer, uint8_t pos)
{
	uint8_t length = strlen(str);

	for (uint8_t i = 0; i < length - pos; i++) 
		buffer[i] = str[pos + i];
}

void str::substr(const char * str, char * buff, uint8_t left, uint8_t right)
{
	for (uint8_t i = 0; i < right - left; i++)
		buff[i] = str[left + i];
}