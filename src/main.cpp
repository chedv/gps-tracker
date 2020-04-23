#include "config.h"
#include "sim808.h"
#include "sd_module.h"
#include "timer.h"
#include "stm32_id.h"

GpsEntries gpsEntries;
CfgEntries cfgEntries;

Sim808 sim808;
SdModule sdModule;

Timer timer;

void setup()
{
    Serial.begin(USB_SERIAL_SPEED);

    cfgEntries.deviceId = stm32Id();
    cfgEntries.workDelay = STM_DEFAULT_DELAY;

    sim808.init(SIM_SERIAL, SIM_SERIAL_SPEED);
    sdModule.init(SD_SPI_PORT, SD_CS_PIN);
    sdModule.readConfig(cfgEntries);

    timer.setInterval(cfgEntries.workDelay);
}

void loop()
{
    if (timer.available())
    {
        if (sim808.gpsAvailable() && sim808.gpsRead(gpsEntries))
        {
            if (sim808.gprsAvailable())
                sim808.gprsSendLocation(cfgEntries, gpsEntries);

            sdModule.writeLocation(gpsEntries);
        }
        timer.update();
    }
}