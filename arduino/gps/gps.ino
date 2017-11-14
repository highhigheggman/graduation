#include <stdio.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <RHReliableDatagram.h>
#include <RH_RF95.h>
#include <SPI.h>

#define CLIENT_ADDRESS 1
#define SERVER_ADDRESS 2

//-----------------gps--------------------
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
        while (Serial.available()) {
            char c = Serial.read();
            gps.encode(Serial.read());
            Serial.write(c);
        }
    } while (millis() - start < ms);
    Serial.println("***********");
}

void setup() {
    Serial.begin(9600);

    if (!manager.init())
        Serial.println("init failed");
    // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on

    // Setup frequency
    driver.setFrequency(frequency);
    // Setup Power,dBm
    driver.setTxPower(tPower, false);
}

void loop() {

    Serial.print("satellites:");
    Serial.println(gps.satellites.value());

    if (true) {
        
        char data[30];
        char latGPS[10];
        char lngGPS[10];
        
        dtostrf(gps.location.lng(), -9, 4, latGPS);
        Serial.print(gps.location.lat());
        Serial.print(" ");
        Serial.println(latGPS);

        dtostrf(gps.location.lng(), -9, 4, lngGPS);
        Serial.print(gps.location.lng());
        Serial.print(" ");
        Serial.println(lngGPS);
        
        sprintf(data, "%s,%s,%d", latGPS, lngGPS, count);
        Serial.println(data);
        Serial.println();
        
        // check location
        if (gps.location.isUpdated()) {
            Serial.println("update");
        }

        // send Data
        Serial.println("Sending to rf95_reliable_datagram_server");
        if (manager.sendtoWait(data, sizeof(data), SERVER_ADDRESS)) {

            // delay and update
            smartDelay(10000);

        } else {
            Serial.println("sendtoWait failed");

            // delay and update
            smartDelay(5000);
        }

    } else {
        smartDelay(1000);
    }

}
