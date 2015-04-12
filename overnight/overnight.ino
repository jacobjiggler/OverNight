#include <Wire.h>
#include "RTClib.h"
#include <LiquidCrystal.h>
RTC_DS1307 RTC;
DateTime now; // Holds the current date and time information
const int hourPin = 2;
const int minPin = 3;
const int alarmPin = 4;
const int alarmTogglePin = 5;
const int buzzPin = 6;
const int backlightPin = 53;

boolean alarm = false; // Holds the current state of the alarm if on or off
boolean alarmSet = false; //Is true if alarm button is being held down
boolean buzzer = false; // holds if the buzzer is on or off
int hourPinState = 0;
int minPinState = 0;
int alarmPinState = 0;

int lastHourPinState = 1;
int lastMinPinState = 1;
int count = 0;
int alarmcount = 0;
int alarmHour = 0;
int alarmMin = 0;

LiquidCrystal lcd(46, 47, 48, 49, 50, 51, 52);


void setup() {
  //Initialization
  alarmHour = 23;
  alarmMin = 43;
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
  pinMode(buzzPin, OUTPUT);
  pinMode(alarmTogglePin, INPUT_PULLUP);
  noTone(buzzPin);
  digitalWrite(buzzPin, HIGH);
  digitalWrite(backlightPin, HIGH);
  
}



void loop() {
  // put your main code here, to run repeatedly:
  alarmPinState = digitalRead(alarmPin);
  alarm = digitalRead(alarmTogglePin);
  Serial.println(alarm);
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
          if(alarmHour < 23){
            alarmHour += 1;
          }
          else{
            alarmHour = 0;
          }
        }
      }

      if(minPinState != lastMinPinState){
        //if the state has changed increment the counter
        if(minPinState == LOW){
          Serial.println("Min Button pressed");
          //increment hour counter
          if(alarmMin < 59){
            alarmMin += 1;
          }
          else{
            alarmMin = 0;
          }
        }
      }



      lastHourPinState = hourPinState;
      lastMinPinState = minPinState;
  }
  if(alarmPinState == HIGH){
    alarmSet = false;
  }



  count+=1;
  delay(100);
  if(count == 10){
    if(alarmSet == false){
      showTime();
    }
    count=0;
  }

  //alarm check
  if (alarmHour == now.hour()){
    if (alarmMin == now.minute()){
      if (!buzzer){
        tone(buzzPin, 100, 2000);
        buzzer = true;
      }
      else {
        digitalWrite(buzzPin, HIGH);
    }
    }
    //min doesnt match
    else {
      buzzer = false;
    }
  }
  //hour doesnt match
  else {
    buzzer = false;
  }
  
  if(alarmSet){
    lcd.clear();
    //print the current alarm set time
    lcd.print("ALARM " + String(alarmHour < 10 ? "0" : "") + String(alarmHour) + ":" + (alarmMin < 10 ? "0" : "") + String(alarmMin));
  }
}

void showTime(){
  now = RTC.now();
  Serial.print("The current time is ");
  Serial.print((now.hour() < 10 ? "0" : "") + String(now.hour()) + ":" + (now.minute() < 10 ? "0" : "") + now.minute() + ":" + (now.second() < 10 ? "0" : "") + now.second() + "\n");
  lcd.clear();
  lcd.print((now.hour() < 10 ? "0" : "") + String(now.hour()) + ":" + (now.minute() < 10 ? "0" : "") + now.minute() + ":" + (now.second() < 10 ? "0" : "") + now.second() + (alarm ? "  ALARM" : ""));
}
