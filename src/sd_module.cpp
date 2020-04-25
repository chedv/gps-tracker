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

                if (key.equals("work_delay"))
                {
                    const uint32_t workDelay = value.toInt();
                    if (workDelay >= 30000 && workDelay <= 600000)
                        cfgEntries.workDelay = workDelay;
                }
                else if (key.equals("location_upd_dist"))
                {
                    const uint16_t updateDist = value.toInt();
                    if (updateDist >= 0 && updateDist <= 1000)
                        cfgEntries.updateDist = updateDist;
                }
                else if (key.equals("auth_token"))
                    cfgEntries.authToken = value;
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
        file.println(gpsEntries.toJson());
        file.close();
    }
}

void SdModule::restoreLocation(const CfgEntries & cfgEntries, Sim808 & sim808) const
{
    File file = SD.open("log.txt", FILE_READ);

    if (file)
    {
        while (file.available())
            sim808.gprsSendLocation(cfgEntries, file.readStringUntil('\n'));

        file.close();
        SD.remove("log.txt");
    } 
}