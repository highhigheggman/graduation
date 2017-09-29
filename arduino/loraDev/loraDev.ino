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
 ** CS - pin 7
 a
 *rtc module

 */

#include <SPI.h>
#include <SD.h>
#include <MsTimer2.h>
#include <DS3232RTC.h>    //http://github.com/JChristensen/DS3232RTC
#include <Time.h>
#include <TimeLib.h>         //http://www.arduino.cc/playground/Code/Time
#include <Wire.h> //http://arduino.cc/en/Reference/Wire (included with Arduino IDE)

// log file
volatile File logFile;
String logFileName = "";

// analog pin
const int analogPin00 = A0;
const int analogPin01 = A1;
const int analogPin02 = A2;

// chip select pin (10:ss pin)
const int chipSelectSD = 4;
const int chipSelect = 10;

// variable for sensor
uint16_t xAxis = 0;
uint16_t yAxis = 0;
uint16_t zAxis = 0;

// variable for write SD counter
volatile int sdCount = 0;
const int sdWriteTime = 1000;

// Interval of timer interrupt (ms)
const int interval = 10;

void timerInterrupt() {
    // allow interrupt
    interrupts();

    // read sensor val
    xAxis = analogRead(analogPin00);
    yAxis = analogRead(analogPin01);
    zAxis = analogRead(analogPin02);

    // SD
    if(logFile){
        logFile.print(year());
        logFile.print("/");
        logFile.print(month());
        logFile.print("/");
        logFile.print(day());
        logFile.print(" ");
        logFile.print(hour());
        logFile.print(":");
        logFile.print(minute());
        logFile.print(":");
        logFile.print(second());
        logFile.print(",");
        logFile.print(xAxis);
        logFile.print(",");
        logFile.print(yAxis);
        logFile.print(",");
        logFile.println(zAxis);
    }else{
        Serial.println("missing open file");
        Serial.println(xAxis);
    }

    //update write SD counter
    sdCount += 1;
    if(sdCount%100 == 0) {
        Serial.println(sdCount);
    }

}

String makeLogFileName() {
    // 8.3 format
    return "/log/" + String(month()) + "_" + String(day()) + "/" + String(hour()) + ".log";
}

void setup() {

    // Initialize Serial
    Serial.begin(9600);

    // disturb shifting to slave mode / set ss pin to output
    pinMode(10, OUTPUT);

    // set system time using RTC
    setSyncProvider(RTC.get);
    if(timeStatus() != timeSet) {
        Serial.println("Unable to sync with the RTC");
    }else{
        Serial.println("RTC has set the system time");
    }

    // Set the number of seconds between re-syncs
    //setSyncInterval(10);

    // Initialize SD card
    if (!SD.begin(chipSelectSD)) {
        Serial.println("Card failed, or not present");
        //return;
    }else{
        Serial.println("succes init sd");
    }

    // open file / or create
    logFileName = makeLogFileName();
    String dirName = "/log/" + String(month()) + "_" + String(day());
    Serial.println(logFileName);
    if(!SD.exists("log")) {
        SD.mkdir("log");
    }
    if(!SD.exists(dirName)) {
        SD.mkdir(dirName);
    }
    logFile = SD.open(logFileName, FILE_WRITE);

    // setting msTimer2
    MsTimer2::set(interval, timerInterrupt);
    MsTimer2::start();
    Serial.println("mstimer2 start");
}

void loop() {

    // Normalization
    // 3.3V: 0-675, 5.0V: 0-1024
    //  xAxis = map(xAxis, 0, 675, 0, 255);
    //  yAxis = map(yAxis, 0, 675, 0, 255);
    //  zAxis = map(zAxis, 0, 675, 0, 255);

    // write data to SD
    if (sdCount > sdWriteTime) {
        MsTimer2::stop();
        Serial.println("WriteStart");
        Serial.print(hour());
        Serial.print(",");
        Serial.print(minute());
        Serial.print(",");
        Serial.println(second());

        if(logFileName.equals(makeLogFileName())) {
            //write to file
            logFile.flush();
        }else{
            //close file
            logFile.close();

            //reopen file every 1hours
            logFileName = makeLogFileName();
            String dirName = "/log/" + String(month()) + "_" + String(day());
            Serial.println(logFileName);

            if(!SD.exists(dirName)){
                SD.mkdir(dirName);
            }
            logFile = SD.open(logFileName, FILE_WRITE);

            // set system time using RTC
            setSyncProvider(RTC.get);
            if(timeStatus() != timeSet) {
                Serial.println("Unable to sync with the RTC");
            }else{
                Serial.println("RTC has set the system time");
            }
        }

        Serial.println("WriteFin");
        sdCount = 0;
        MsTimer2::start();
    }
}
