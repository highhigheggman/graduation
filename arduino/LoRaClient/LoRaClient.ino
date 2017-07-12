#include <RHReliableDatagram.h>
#include <RH_RF95.h>
#include <SPI.h>

#define CLIENT_ADDRESS 1
#define SERVER_ADDRESS 2 

// Singleton instance of the radio driver
RH_RF95 driver;

// Class to manage message delivery and receipt, using the driver declared above
RHReliableDatagram manager(driver, CLIENT_ADDRESS);

// transmitter and receiver centre frequency
const float frequency = 923.6;
// transmitter power level in dBm (+5 to + 23)
const uint8_t tPower = 20;

const int8_t sf = 7;

void setup()
{
    Serial.begin(9600);
    while (!Serial) ; // Wait for serial port to be available
    if (!manager.init())
        Serial.println("init failed");
    // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on

    // Setup frequency
    driver.setFrequency(frequency);
    // Setup Power,dBm
    driver.setTxPower(tPower, false);

}

//uint8_t data[] = "1abcdefghi2abcdefghi3abcdefghi4abcdefghi5abcdefghi6abcdefghi7abcdefghi8abcdefghi9abcdefghi10abcdefgh11abcdefgh12abcdefgh13abcdefgh14abcdefgh15abcdefgh16abcdefgh17abcdefgh18abcdefgh19abcdefgh20abcdefgh21abcdefgh22abcdefgh23abcdefgh24abcdefgh25abcdefgh";
uint8_t data[] = "aaa";
// Dont put this on the stack:
//uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];

void loop()
{
    Serial.println("Sending to rf95_reliable_datagram_server");

    // Send a message to manager_server
    if (manager.sendtoWait(data, sizeof(data), SERVER_ADDRESS))
    {
      /*
        // Now wait for a reply from the server
        uint8_t len = sizeof(buf);
        uint8_t from;
        if (manager.recvfromAckTimeout(buf, &len, 2000, &from))
        {
            Serial.print("got reply from : 0x");
            Serial.print(from, HEX);
            Serial.print(": ");
            Serial.println((char*)buf);
        }
        else
        {
            Serial.println("No reply, is rf95_reliable_datagram_server running?");
        }
        */
    }
    else
        Serial.println("sendtoWait failed");
    delay(500);
}
