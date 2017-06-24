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
File logFile;
String logFileName = "";

// analog pin
const int analogPin00 = A0;
const int analogPin01 = A1;
const int analogPin02 = A2;

// chip select pin (10:ss pin)
const int chipSelectSD = 4;
const int chipSelect = 10;

// variable for time rep
int secondsR = 0;
int milliSecondsR= 0;

// Interval of timer interrupt (ms)
const int interval = 10;

// time for debug
unsigned long nowMicroSec = 0;
unsigned long preMicroSec = 0;

// Write data
typedef struct DATA {
    int xAxis;
    int yAxis;
    int zAxis;
    time_t tm;
};

// Buffer
const int buffSize = 512;
const int buffNum = 4;

typedef struct BUFFER {
    bool full;
    DATA buf[buffSize];
}

volatile BUFFER ramBuffer[buffNum];
volatile int activeBuffNo = 0;
volatile int activeBuffCount = 0;
int writeBuffNo = 0;

void storeData(int xAxis, int yAxis, int zAxis, time_t tm) {
    ramBuffer[activeBuffNo].buf[activeBuffCount] = { xAxis, yAxis, zAxis, tm};
    activeBuffCount++;

    // check buffer
    if(activeBuffCount >= buffSize) {
        ramBuffer[activeBuffNo].full = true;
        // chanege next buffer
        if(activeBuffNo != buffNum) {
            activeBuffNo++;
        }else{
            activeBuffNo = 0;
        }
        activeBuffCount = 0;
    }
}

void writeData(int buffNo, File file) {
    for(int i = 0; i < buffSize; i++) {
        file.print(hour(ramBuffer[buffNo].tm));
        file.print(",");
        file.print(minute(ramBuffer[buffNo].tm));
        file.print(",");
        file.print(second(ramBuffer[buffNo].tm));
        file.print(",");
        file.print(ramBuffer[buffNo].xAxis);
        file.print(",");
        file.print(ramBuffer[buffNo].yAxis);
        file.print(",");
        file.print(ramBuffer[buffNo].zAxis);
    }

    // physically saved to the SD card
    flush();
}

void timerInterrupt() {
    // check time
    if(secondsR != int(second())){
        milliSecondsR = 0;
        Serial.print("*");
    }

    // read sensor val
    int xAxis = analogRead(analogPin00);
    int yAxis = analogRead(analogPin01);
    int zAxis = analogRead(analogPin02);

    // store data to ram buffer
    store(xAxis, yAxis, zAxis, now());

    //update time
    milliSecondsR += interval;
    secondsR = int(second());
}

String makeLogFileName() {
    // 8.3 format
    return String(month()) + "_" + String(day()) + "_" + String(hour()) + ".log";
}

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

    // Set the number of seconds between re-syncs
    //setSyncInterval(10);

    // Initialize time for milliSeconds
    milliSecondsR = 0;
    secondsR = int(second());

    // Initialize SD card
    if (!SD.begin(chipSelectSD)) {
        Serial.println("Card failed, or not present");
        //return;
    }else{
        Serial.println("succes init sd");
    }

    // open file / or create
    logFileName = makeLogFileName();
    Serial.println(logFileName);
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

    // write data to SD from ramBuffer
    if(ramBuffer[writeBuffNo].full) {
        writeData(writeBuffNo, logFile);
        ramBuffer[writeBuffNo].full = false;
    }
}
