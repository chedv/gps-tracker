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
    Serial.begin(USB_serialSpeed);

    gpsEntries.initialized = false;
    cfgEntries.deviceId = stm32Id();
    cfgEntries.workDelay = STM_defaultDelay;

    sim808.init(SIM_serial, SIM_serialSpeed);
    sdModule.init(SD_spiPort, SD_csPin);
    sdModule.readConfig(cfgEntries);

    timer.setInterval(cfgEntries.workDelay);
    timer.update();
}

void loop() 
{
    if (timer.available())
    {
        if (sim808.gpsAvailable())
            sim808.gpsRead(gpsEntries);

        if (gpsEntries.initialized)
        {
            if (sim808.gprsAvailable())
                sim808.gprsSendLocation(cfgEntries, gpsEntries);

            sdModule.writeLocation(gpsEntries);
        }
        timer.update();
    }
}