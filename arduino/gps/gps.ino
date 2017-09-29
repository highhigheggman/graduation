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
SoftwareSerial gpsSerial(A1, A2); // RX, TX

//-----------------LoRa-------------------
// Singleton instance of the radio driver
RH_RF95 driver;

// Class to manage message delivery and receipt, using the driver declared above
RHReliableDatagram manager(driver, CLIENT_ADDRESS);

// transmitter and receiver centre frequency
const float frequency = 923.6;
// transmitter power level in dBm (+5 to + 23)
const uint8_t tPower = 20;
// sf
const int8_t sf = 7;

int count;

// get data use GPS. and delay
static void smartDelay(unsigned long ms)
{
    unsigned long start = millis();

    do {
        while (gpsSerial.available()) {
            char c = gpsSerial.read();
            gps.encode(gpsSerial.read());
            Serial.write(c);
        }
    } while (millis() - start < ms);
    Serial.println("***********");
}

void setup() {
    Serial.begin(115200);
    gpsSerial.begin(9600);

    if (!manager.init())
        Serial.println("init failed");
    // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on

    // Setup frequency
    driver.setFrequency(frequency);
    // Setup Power,dBm
    driver.setTxPower(tPower, false);

    // led
    pinMode(3, OUTPUT);
    pinMode(4, OUTPUT);

    count = 0;
}

void loop() {

    Serial.print("satellites:");
    Serial.println(gps.satellites.value());

    if (true) {
        if(gps.satellites.value() > 0){
          digitalWrite(3,HIGH);
        }else{
          digitalWrite(3,LOW);
        }
        
        Serial.print("count");
        Serial.println(count);
        
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
            digitalWrite(4,HIGH);

            // delay and update
            smartDelay(10000);

            count+=1;

        } else {
            Serial.println("sendtoWait failed");
            digitalWrite(4,LOW);

            // delay and update
            smartDelay(1000);
        }

    } else {
        digitalWrite(3,LOW);
        smartDelay(1000);
    }

}
