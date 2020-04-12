#include "sd_module.h"

void SdModule::init(uint8_t spiPort, uint8_t csPin) const
{
    SPI.setModule(spiPort);
    SD.begin(csPin);
}

void SdModule::readConfig(CfgEntries & cfgEntries) const
{
    File file = SD.open("config.txt", FILE_READ);

    if (file)
    {
        while (file.available())
        {
            String buffer = file.readStringUntil('\n');

            uint8_t pos;
            if ((pos = buffer.indexOf('=')) != -1)
            {
                const String key = buffer.substring(0, pos);
                const String value = buffer.substring(pos + 1);

                if (key.equals("auth_token"))
                    cfgEntries.authToken = value;
                else if (key.equals("work_delay"))
                {
                    const uint32_t workDelay = value.toInt();
                    if (workDelay > 0 && workDelay <= 600000)
                        cfgEntries.workDelay = workDelay;
                }
            }
        }
        file.close();
    }
}

void SdModule::writeLocation(const GpsEntries & gpsEntries) const
{
    File file = SD.open("log.txt", FILE_WRITE);

    if (file)
    {
        file.println(gpsEntries.toString());
        file.close();
    }
}