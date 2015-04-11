#include <Wire.h>
#include "RTClib.h"
#include <LiquidCrystal.h>

RTC_DS1307 RTC;
DateTime now; // Holds the current date and time information
const int hourPin = 2;
const int minPin = 3;
const int alarmPin = 4;

const int backlightPin = 53;

boolean alarm = false; // Holds the current state of the alarm
DateTime alarmTime;


int hourPinState = 0;
int minPinState = 0;
int alarmPinState = 0;

int lastHourPinState = 1;
int lastMinPinState = 1;

LiquidCrystal lcd(46, 47, 48, 49, 50, 51, 52);

void setup() {
  //Initialization
  Wire.begin();
  Serial.begin(9600);
  
  // Initialize LCD
  lcd.begin(16, 2);
  lcd.print("Hello wor;ld");
  
  //Set up RTC
  RTC.begin(); 
  RTC.adjust(DateTime(__DATE__, __TIME__));
  
  //alarmTime Initialized
  /*
  alarmTime.Hour = 0;
  alarmTime.Minute = 0;
  alarmTime.Seconds = 0; 
  */
  
  
  //I/O 
  
  //buttons
  pinMode(hourPin, INPUT);
  pinMode(minPin, INPUT);
  pinMode(alarmPin, INPUT);
  pinMode(backlightPin, OUTPUT);
  
  digitalWrite(backlightPin, HIGH);
}



void loop() { 
  // put your main code here, to run repeatedly:
  alarmPinState = digitalRead(alarmPin);
  if(alarmPinState == LOW){
    //implement Change Alarm Time
      //Serial.println("SETTING ALARM");
      hourPinState = digitalRead(hourPin);
      minPinState = digitalRead(minPin);
      if(hourPinState !=lastHourPinState){
        //if the state has changed increment the counter
        if(hourPinState == LOW){
          Serial.println("Hour Button pressed");
          //increment hour counter
        }
      }
      
      if(minPinState != lastMinPinState){
        //if the state has changed increment the counter
        if(minPinState == LOW){
          Serial.println("Min Button pressed");
          //increment hour counter
        }
      }
      
      
      
      lastHourPinState = hourPinState;
      lastMinPinState = minPinState;
  }

  /*
  hourPinState = digitalRead(hourPin);
  minPinState = digitalRead(minPin);
  alarmPinState = digitalRead(alarmPin);
  
  if(alarmPinState == LOW){
    Serial.println("alarm");
  }
  if(minPinState == LOW){
    Serial.println("min");
  }
  if(hourPinState == LOW){
    Serial.println("hour");
  }
  */
 
  showTime();
  delay(1000);
}

void showTime(){
  now = RTC.now();
  Serial.print("The current time is ");
  Serial.print((now.hour() < 10 ? "0" : "") + String(now.hour()) + ":" + (now.minute() < 10 ? "0" : "") + now.minute() + ":" + (now.second() < 10 ? "0" : "") + now.second() + "\n");
  
}

