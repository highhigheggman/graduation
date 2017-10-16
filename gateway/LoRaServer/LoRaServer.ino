#include <RHReliableDatagram.h>
#include <RH_RF95.h>
#include <SPI.h>
#include <Console.h>
#include <Process.h>
#include <stdio.h>
#include <string.h>

#define CLIENT_ADDRESS 1
#define SERVER_ADDRESS 2


//If you use Dragino IoT Mesh Firmware, uncomment below lines.
//For product: LG01. 
#define BAUDRATE 115200

// Singleton instance of the radio driver
RH_RF95 driver;

// Class to manage message delivery and receipt, using the driver declared above
RHReliableDatagram manager(driver, SERVER_ADDRESS);

// transmitter and receiver centre frequency
const float frequency = 923.6;
// transmitter power level in dBm (+5 to + 23)
const uint8_t tPower = 20;

// command. bridge lib
Process date;
String nowDate;
Process post;
String postRet;

void setup()
{
    Serial.begin(9600);  
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

// Dont put this on the stack:
char buf[40];

void loop()
{
    if (manager.available())
    {
        // Wait for a message addressed to us from the client
        uint8_t len = sizeof(buf);
        uint8_t from;
        if (manager.recvfromAck(buf, &len, &from))
        {
            Console.print("got request from : 0x");
            Console.print(from, HEX);
            Console.print(": ");
            Console.println(buf);
            Console.print("RSSI: ");
            Console.println(driver.lastRssi(), DEC);

            //split buf
            String maxAcc = String(strtok(buf, ","));
            maxAcc.trim();
            String minAcc = String(strtok(NULL , ","));
            minAcc.trim();

            //get time
            if(!date.running()){
              date.begin("date");
              date.addParameter("+""%Y-%m-%d %H:%M:%S""");
              date.run();
              nowDate = date.readString();

              Console.print(nowDate);
            }

            // http POST/ curl
            if(!post.running()){
              post.begin("curl");
              post.addParameter("-d ""deviceId=10001""");
              post.addParameter("-d ""maxAcc=" + maxAcc + """");
              post.addParameter("-d ""minAcc=" + minAcc + """");
              post.addParameter("-d ""time=" + nowDate + """");
              post.addParameter("http://150.95.148.129/recvPost.php");
              post.run();

              postRet = post.readString();
              Console.println(postRet);
            }

        }
    }
}
