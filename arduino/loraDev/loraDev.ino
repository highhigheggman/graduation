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
a
 *rtc module

 */

#include <SPI.h>
#include <SD.h>
//#include <MsTimer2.h>
#include <DS3232RTC.h>    //http://github.com/JChristensen/DS3232RTC
#include <Time.h>
#include <TimeLib.h>         //http://www.arduino.cc/playground/Code/Time
#include <Wire.h> //http://arduino.cc/en/Reference/Wire (included with Arduino IDE)

// log file
File logFile;
String logFileName;

// analog pin
const int analogPin00 = A0;
const int analogPin01 = A1;
const int analogPin02 = A2;

// chip select pin (10:ss pin)
const int chipSelectSD = 7;
const int chipSelect = 10;

// variable for sensor
int xAxis = 0;
int yAxis = 0;
int zAxis = 0;

// Interval of timer interrupt (ms)
//const int interval = 20;

/*
   void timerInterrupt() {
// read sensor val
xAxis = analogRead(analogPin00);
yAxis = analogRead(analogPin01);
zAxis = analogRead(analogPin02);

// make a string for assembling the data to log
//String dataString = String(xAxis + "," + yAxis + "," + zAxis);
String dataString = String(xAxis);

logFile.println(dataString);
Serial.println(dataString);
}
 */

void setup() {
    
    // Initialize Serial
    Serial.begin(9600);

    // disturb shifting to slave mode / set ss pin to output
    pinMode(10, OUTPUT);

    // set system time using RTC
    setSyncProvider(RTC.get);
    if(timeStatus() != timeSet)
        Serial.println("Unable to sync with the RTC");
    else
        Serial.println("RTC has set the system time");

    // Initialize SD card
    if (!SD.begin(7)) {
    Serial.println("Card failed, or not present");
    //return;
    }else{
      Serial.println("succes init sd");
    }

    // open file / or create
    //logFile = SD.open(logFileName, FILE_WRITE);

    // setting msTimer2
    //MsTimer2::set(interval, timerInterrupt);
    //MsTimer2::start;
}

void loop() {

    // Normalization
    // 3.3V: 0-675, 5.0V: 0-1024
    //  xAxis = map(xAxis, 0, 675, 0, 255);
    //  yAxis = map(yAxis, 0, 675, 0, 255);
    //  zAxis = map(zAxis, 0, 675, 0, 255);

    Serial.print(hour());
    Serial.print(":");
    Serial.print(minute());
    Serial.print(":");
    Serial.print(second());
    Serial.print("-");
    Serial.print(analogRead(analogPin00));
    Serial.print(",");
    Serial.print(analogRead(analogPin01));
    Serial.print(",");
    Serial.print(analogRead(analogPin02));
    Serial.println();

    delay(100);
}
