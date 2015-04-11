#include <Wire.h>
#include "RTClib.h"

RTC_DS1307 RTC;
DateTime now; // Holds the current date and time information
boolean alarm = false; // Holds the current state of the alarm

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  RTC.begin();
  Serial.begin(9600); 
  RTC.adjust(DateTime(__DATE__, __TIME__));
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
