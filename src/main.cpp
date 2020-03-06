#include <sim808.h>
#include <sd.h>
#include <helpers.h>
#include <str.h>

//-------------------------------------------------------------------------

void setup()
{
    Serial.begin(9600);

    SIM808::init();
    SdModule::init();
    
    Helpers::readConfig(&entries);
    Helpers::createIdentifier(&entries);

    Serial.println(entries.deviceId);
    Serial.println(entries.authToken);
    Serial.println(entries.workDelay);
}

void loop()
{
    if (millis() - entries.timer >= entries.workDelay)
    {
        if (SIM808::gpsAvailable())
            SIM808::gpsRead(&entries);

        if (!str::empty(entries.gpsEntry) && SIM808::gprsAvailable())
            SIM808::gprsSendRequest(&entries);

        entries.timer = millis();
    }
}

//-------------------------------------------------------------------------
