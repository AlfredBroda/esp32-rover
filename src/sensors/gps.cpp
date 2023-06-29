
#include "gps.h"

SFE_UBLOX_GPS myGPS;

void GPS::init()
{
    do
    {
        Serial.println("GPS: trying 38400 baud");
        Serial2.begin(38400);
        if (myGPS.begin(Serial2) == true)
            break;

        delay(100);
        Serial.println("GPS: trying 9600 baud");
        Serial2.begin(9600);
        if (myGPS.begin(Serial2) == true)
        {
            Serial.println("GPS: connected at 9600 baud, switching to 38400");
            myGPS.setSerialRate(38400);
            delay(100);
        }
        else
        {
            // myGPS.factoryReset();
            delay(2000); // Wait a bit before trying again to limit the Serial output
        }
    } while (1);
    Serial.println("GPS serial connected");

    myGPS.setUART1Output(COM_TYPE_UBX); // Set the UART port to output UBX only
    myGPS.setI2COutput(COM_TYPE_UBX);   // Set the I2C port to output UBX only (turn off NMEA noise)
    myGPS.saveConfiguration();          // Save the current settings to flash and BBR
}

void GPS::loop()
{
    long latitude = myGPS.getLatitude();
    Serial.printf("Lat: %0.4f ", latitude);

    long longitude = myGPS.getLongitude();
    Serial.printf("Long: %0.4f (deg*10^7)", longitude);

    long altitude = myGPS.getAltitude();
    Serial.printf("Alt: %0.2fmm ", altitude);

    byte SIV = myGPS.getSIV();
    Serial.printf("SIV: %d ", SIV);

    uint8_t fix = myGPS.getFixType();
    Serial.printf("Fix: %d\n", fix);
}
