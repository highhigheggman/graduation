#include <RHReliableDatagram.h>
#include <RH_RF95.h>
#include <SPI.h>
#include <Console.h>

#define CLIENT_ADDRESS 1
#define SERVER_ADDRESS 2

//If you use Dragino IoT Mesh Firmware, uncomment below lines.
//For product: LG01. 
#define BAUDRATE 115200

int led = A2;

// Singleton instance of the radio driver
RH_RF95 driver;

// Class to manage message delivery and receipt, using the driver declared above
RHReliableDatagram manager(driver, SERVER_ADDRESS);

// transmitter and receiver centre frequency
const float frequency = 923.6;
// transmitter power level in dBm (+5 to + 23)
const uint8_t tPower = 20;

void setup()
{
    Serial.begin(9600);
    pinMode(led, OUTPUT);     
    Bridge.begin(BAUDRATE);
    Console.begin();
    while (!Console) ; // Wait for console port to be available
    Console.println("Start Sketch");
    
    if (!manager.init())
        Console.println("init failed");
    // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on

    // Setup frequency
    driver.setFrequency(frequency);
    // Setup Power,dBm
    driver.setTxPower(tPower, false);

    Console.print("Listening on frequency: ");
    Console.println(frequency);
}

uint8_t data[] = "And hello back to you";
// Dont put this on the stack:
char buf[RH_RF95_MAX_MESSAGE_LEN];

void loop()
{
    if (manager.available())
    {
        // Wait for a message addressed to us from the client
        uint8_t len = sizeof(buf);
        uint8_t from;
        if (manager.recvfromAck(buf, &len, &from))
        {
            digitalWrite(led, HIGH);
            Console.print("got request from : 0x");
            Console.print(from, HEX);
            Console.print(": ");
            Console.println(buf);
            Console.print("RSSI: ");
            Console.println(driver.lastRssi(), DEC);

            // Send a reply back to the originator client
            //if (!manager.sendtoWait(data, sizeof(data), from))
            //    Console.println("sendtoWait failed");
        }
    }
}
