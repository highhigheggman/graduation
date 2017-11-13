#include <stdio.h>
#include <RHReliableDatagram.h>
#include <RH_RF95.h>

#define CLIENT_ADDRESS 1
#define SERVER_ADDRESS 2

//-----------------LoRa-------------------
// Singleton instance of the radio driver
RH_RF95 driver;

// Class to manage message delivery and receipt, using the driver declared above
RHReliableDatagram manager(driver, CLIENT_ADDRESS);

// transmitter and receiver centre frequency
const float frequency = 923.6;
// transmitter power level in dBm (+5 to + 23)
const uint8_t tPower = 13;
// sf
const int8_t sf = 7;

int count;
int countAll;

void setup() {
    Serial.begin(115200);

    if (!manager.init())
        Serial.println("init failed");
    // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on

    // Setup frequency
    driver.setFrequency(frequency);
    // Setup Power,dBm
    driver.setTxPower(tPower, false);
    // Set Retries
    manager.setRetries(0);

    // led
    pinMode(4, OUTPUT);

    count = 0;
    countAll = 0;
}

void loop() {

    char data[10];

    sprintf(data, "%d", count);

    // send Data
    //Serial.println("Sending to rf95_reliable_datagram_server");
    if (manager.sendtoWait(data, sizeof(data), SERVER_ADDRESS)) {
        digitalWrite(4,HIGH);
        count+=1;
        countAll+=1;

        delay(1000);

    } else {
        //Serial.println("sendtoWait failed");
        digitalWrite(4,LOW);
        countAll+=1;

        // delay and update
        delay(1000);
    }
    Serial.print(count);
    Serial.print("/");
    Serial.print(countAll);
    Serial.print(" RSSI:");
    Serial.println(driver.lastRssi(), DEC);

}
