#include <DS3232RTC.h>
#include <Time.h>
#include <TimeLib.h>

void setup() {
  Serial.begin(9600);
  Serial.println("0000");
  
  setTime(23, 31, 30, 13, 2, 2009);   //set the system time to 23h31m30s on 13Feb2009
  RTC.set(now());                     //set the RTC from the system time

}

void loop() {
}
