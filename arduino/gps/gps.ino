#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <RHReliableDatagram.h>
#include <RH_RF95.h>
#include <SPI.h>
#include <SD.h>

#define CLIENT_ADDRESS 1
#define SERVER_ADDRESS 2

//-----------------gps--------------------
TinyGPSPlus gps;
SoftwareSerial gpsSerial(0, 1); // RX, TX

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

// ---------------SD-----------------------
// log file
File logFile;
String logFileName = "";
// LoRa ack
bool ackL = false;


// get data use GPS. and delay
static void smartDelay(unsigned long ms)
{
    unsigned long start = millis();

    do {
        while (gpsSerial.available()) {
            char c = gpsSerial.read();
            gps.encode(gpsSerial.read());
            //gps.encode(gpsSerial.read());
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

}

void loop() {

    Serial.print("satellites:");
    Serial.print(gps.satellites.value());
    Serial.print(",");
    Serial.println(gpsSerial.available());

    // check satellites
    if (gps.satellites.value() > 0 && gps.satellites.isValid()) {
        Serial.println(gps.location.lat(), gps.location.lng());
        uint8_t data[] = "xxxxxx,xxxxxxx"; 

        // check location
        if (gps.location.isUpdated()) {
            Serial.println("update");
        }

        // send Data
        Serial.println("Sending to rf95_reliable_datagram_server");
        if (manager.sendtoWait(data, sizeof(data), SERVER_ADDRESS)) {
            ackL = true;
            // SD
            if (logFile) {
                logFile.print(gps.location.lat());
                logFile.print(",");
                logFile.print(gps.location.lng());
                logFile.print(",");
                logFile.println(ackL);
            } else {
                Serial.println("missing open file");
            }
            // delay and update
            smartDelay(10000);

        } else {
            Serial.println("sendtoWait failed");
            ackL = false;
            // SD
            if (logFile) {
                logFile.print(gps.location.lat());
                logFile.print(",");
                logFile.print(gps.location.lng());
                logFile.print(",");
                logFile.println(ackL);
            } else {
                Serial.println("missing open file");
            }
            // delay and update
            smartDelay(1000);
        }

    }else{
        smartDelay(1000);
    }

}
