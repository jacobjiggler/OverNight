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
boolean alarmSet = false;
DateTime alarmTime;


int hourPinState = 0;
int minPinState = 0;
int alarmPinState = 0;

int lastHourPinState = 1;
int lastMinPinState = 1;
int count = 0;

int alarmHour = 0;
int alarmMin = 0;

LiquidCrystal lcd(46, 47, 48, 49, 50, 51, 52);
/*
enum alarmHour
{
  0,
  1,
  2
};

enum alarmMinutes{
  0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59
};
*/

void setup() {
  //Initialization
  Wire.begin();
  Serial.begin(9600);
  
  // Initialize LCD
  lcd.begin(16, 2);
  lcd.print("Program Starting");
  
  //Set up RTC
  RTC.begin(); 
  RTC.adjust(DateTime(__DATE__, __TIME__));
  
  
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
    alarmSet = true;
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
  else{
    alarmSet = false;
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
  
  count+=1;
  delay(100);
  if(count == 10){
    if(alarmSet == false){
      showTime();
      count=0;
    }
  }
  
  if(alarmSet){
    lcd.clear();
    
  }
}

void showTime(){
  now = RTC.now();
  Serial.print("The current time is ");
  Serial.print((now.hour() < 10 ? "0" : "") + String(now.hour()) + ":" + (now.minute() < 10 ? "0" : "") + now.minute() + ":" + (now.second() < 10 ? "0" : "") + now.second() + "\n");
  lcd.clear();
  lcd.print((now.hour() < 10 ? "0" : "") + String(now.hour()) + ":" + (now.minute() < 10 ? "0" : "") + now.minute() + ":" + (now.second() < 10 ? "0" : "") + now.second() + (alarm ? "  ALARM" : ""));
}

