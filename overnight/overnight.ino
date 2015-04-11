#include <Wire.h>
#include "RTClib.h"

RTC_DS1307 RTC;
DateTime now; // Holds the current date and time information
const int hourPin = 3;
const int minPin = 4;
const int alarmPin = 5;

boolean alarm = false; // Holds the current state of the alarm

void setup() {
  //Initialization
  Wire.begin();
  Serial.begin(9600);
  
  //Set up RTC
  RTC.begin(); 
  RTC.adjust(DateTime(__DATE__, __TIME__));
  
  //I/O 
  pinMode(hourPin, INPUT);
  pinMode(minPin, INPUT);
  pinMode(alarmPin, INPUT);
}



void loop() { 
  // put your main code here, to run repeatedly:
  showTime();
  delay(2000);
}

void showTime(){
  now = RTC.now();
  Serial.print("The current time is ");
  Serial.print((now.hour() < 10 ? "0" : "") + String(now.hour()) + ":" + (now.minute() < 10 ? "0" : "") + now.minute() + ":" + (now.second() < 10 ? "0" : "") + now.second() + "\n");
}
