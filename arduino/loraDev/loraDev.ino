// send sensor info

/*
   write sensor data and time to SD card

   The circuit:

 *acceleration sensor (http://akizukidenshi.com/download/ds/akizuki/AKI-KXR94-Module.pdf)
 ** xAxis - A0
 ** yAxis - A1
 ** zAxis - A2

 *SD card module
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4

 *rtc module

 */

#include <SPI.h>
#include <SD.h>
#include <MsTimer2.h>

// log file
File logFile
const string logFileName = "dataLog.csv";

// analog pin
const int analogPin00 = A0;
const int analogPin01 = A1;
const int analogPin02 = A2;

// chip select pin (10:ss pin)
const int chipSelectSD = 4;
const int chipSelect = 10;

// variable
int xAxis = 0;
int yAxis = 0;
int zAxis = 0;

// Interval of timer interrupt (ms)
const int interval = 2;

void timerInterrupt() {
    // read sensor val
    xAxis = analogRead(analogPin00);
    yAxis = analogRead(analogPin01);
    zAxis = analogRead(analogPin02);

    // make a string for assembling the data to log
    String dataString = string(xAxis + "," + yAxis + "," + zAxis);

    dataFile.println(dataString);
}

void setup() {
    // Initialize Serial
    //Serial.begin(9600);

    // disturb shifting to slave mode / set ss pin to output
    pinMode(10, OUTPUT);

    // Initialize SD card
    if (!SD.begin(chipSelectSD)) {
        //Serial.println("Card failed, or not present");
        return;
    }

    // open file / or create
    logFile = SD.open(logFileName, FILE_WRITE);

    // setting msTimer2
    MsTimer2::set(interval, timerInterrupt);
    MsTimer2::start;
}

void loop() {

    // Normalization
    // 3.3V: 0-675, 5.0V: 0-1024
    //  xAxis = map(xAxis, 0, 675, 0, 255);
    //  yAxis = map(yAxis, 0, 675, 0, 255);
    //  zAxis = map(zAxis, 0, 675, 0, 255);

}
