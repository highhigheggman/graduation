#include <stdio.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <RHReliableDatagram.h>
#include <RH_RF95.h>
#include <SPI.h>
#include <DS3232RTC.h>    //http://github.com/JChristensen/DS3232RTC
#include <Time.h>
#include <TimeLib.h>         //http://www.arduino.cc/playground/Code/Time
#include <Wire.h> //http://arduino.cc/en/Reference/Wire (included with Arduino IDE)

#define CLIENT_ADDRESS 1
#define SERVER_ADDRESS 2

//-----------------gps--------------------
SoftwareSerial ss(A1,A2);
TinyGPSPlus gps;

//-----------------LoRa-------------------
// Singleton instance of the radio driver
RH_RF95 driver;

// Class to manage message delivery and receipt, using the driver declared above
RHReliableDatagram manager(driver, CLIENT_ADDRESS);

// transmitter and receiver centre frequency
const float frequency = 923.6;
// transmitter power level in dBm (+5 to + 23)
const uint8_t tPower = 12;
// sf
const int8_t sf = 7;

int count;

// get data use GPS. and delay
static void smartDelay(unsigned long ms)
{
    unsigned long start = millis();

    do {
        while (ss.available()) {
            //char k = ss.read();
            gps.encode(ss.read());
            //Serial.print(k);
        }
    } while (millis() - start < ms);
    //Serial.println();
    //Serial.print("satellites:");
    //Serial.println(gps.satellites.value());
}

void setup() {
    Serial.begin(9600);
    ss.begin(9600);

    // set system time using RTC
    setSyncProvider(RTC.get);
    if(timeStatus() != timeSet) {
        Serial.println("Unable to sync with the RTC");
    }else{
        Serial.println("RTC has set the system time");
    }

    if (!manager.init())
        Serial.println("init failed");
    // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on

    // Setup frequency
    driver.setFrequency(frequency);
    // Setup Power,dBm
    driver.setTxPower(tPower, false);
    // Set Retries
    //manager.setRetries(0);
    manager.setTimeout(1000);
}

void loop() {
    if (true) {
        
        char data[20];
        //char latGPS[10];
        //char lngGPS[10];
        
        //dtostrf(gps.location.lAT(), -9, 4, latGPS);
        //dtostrf(gps.location.lng(), -9, 4, lngGPS);
        //sprintf(data, "%s,%s,%d", latGPS, lngGPS, count);

        // send Data
        //Serial.println("Sending to rf95_reliable_datagram_server");
        if (manager.sendtoWait(data, sizeof(data), SERVER_ADDRESS)) {
            Serial.print(gps.location.lat(), 6);
            Serial.print(",");
            Serial.print(gps.location.lng(), 6);
            Serial.print(",");
            Serial.print("1");
            Serial.print(",");
            Serial.print(driver.lastRssi(), DEC);
            Serial.print(",");

            Serial.print(year());
            Serial.print("/");
            Serial.print(month());
            Serial.print("/");
            Serial.print(day());
            Serial.print(" ");
            Serial.print(hour());
            Serial.print(":");
            Serial.print(minute());
            Serial.print(":");
            Serial.println(second());
            

            // delay and update
            smartDelay(4000);

        } else {
            //Serial.println("sendtoWait failed");
            Serial.print(gps.location.lat(), 6);
            Serial.print(",");
            Serial.print(gps.location.lng(), 6);
            Serial.print(",");
            Serial.print("0");
            Serial.print(",");
            Serial.print(driver.lastRssi(), DEC);
            Serial.print(",");
            
            Serial.print(year());
            Serial.print("/");
            Serial.print(month());
            Serial.print("/");
            Serial.print(day());
            Serial.print(" ");
            Serial.print(hour());
            Serial.print(":");
            Serial.print(minute());
            Serial.print(":");
            Serial.println(second());

            // delay and update
            smartDelay(4000);
        }

    } else {
        smartDelay(1000);
    }

}
