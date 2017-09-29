#include <RHReliableDatagram.h>
#include <RH_RF95.h>
#include <MsTimer2.h>
#include <stdio.h>

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
// sf
const int8_t sf = 7;

// send data
char data[30];
char cMaxAcc[10];
char cMinAcc[10];

// Analog Pin
const int analogPin00 = A0;
const int analogPin01 = A1;
const int analogPin02 = A2;

// variable for sensor
double xAxis = 0;
double yAxis = 0;
double zAxis = 0;
double comAcc = 0;

// Interval of timer interrupt (ms)
const uint8_t interval = 10;

// variable for Counter
volatile int count = 0;
uint64_t maxCount = 1000;

// store accleration
volatile double maxAcc = 0;
volatile double minAcc = 0;
volatile double sumAcc = 0;
volatile double aveAcc = 0;

void timerInterrupt() {
    // allow interrupt
    interrupts();

    // read sensor value
    xAxis = analogRead(analogPin00);
    yAxis = analogRead(analogPin01);
    zAxis = analogRead(analogPin02);

    // calc combined accleration
    comAcc = sqrt(xAxis*xAxis + yAxis*yAxis + zAxis*zAxis);

    // check max, min
    if(count == 0){
      maxAcc = comAcc;
      minAcc = comAcc;
    }
    if(comAcc > maxAcc) {
        maxAcc = comAcc;
    }

    if(comAcc < minAcc) {
        minAcc = comAcc;
    }

    // update counter
    count += 1;
    if(count%100 == 0) {
      Serial.print(xAxis);
      Serial.print(",");
      Serial.print(yAxis);
      Serial.print(",");
      Serial.print(zAxis);
      Serial.print(",");
      Serial.print(comAcc);
      Serial.print(",");
      Serial.println(count);
    }

}

void setup() {
    Serial.begin(9600);
    while (!Serial) ; // Wait for serial port to be available
    if (!manager.init())
        Serial.println("init failed");
    // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on

    // Setup frequency
    driver.setFrequency(frequency);
    // Setup Power,dBm
    driver.setTxPower(tPower, false);


    // setting msTimer2
    MsTimer2::set(interval, timerInterrupt);
    MsTimer2::start();
    Serial.println("mstimer2 start");
}


void loop() {

    if( count >= maxCount) {
        MsTimer2::stop();
        dtostrf(maxAcc, -9, 4, cMaxAcc);
        dtostrf(minAcc, -9, 4, cMinAcc);
        sprintf(data, "%s,%s", cMaxAcc, cMinAcc);
        Serial.println(maxAcc);
        Serial.println(minAcc);
        
        // Send a message to manager_server
        Serial.println("Sending to rf95_reliable_datagram_server");
        if (manager.sendtoWait(data, sizeof(data), SERVER_ADDRESS)) {
            Serial.println("sendtoWait success");
        } else {
            Serial.println("sendtoWait failed");
        }

        count = 0;
        maxAcc = 0;
        minAcc = 0;
        MsTimer2::start();
    }

}
