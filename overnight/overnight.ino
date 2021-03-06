#include <Wire.h>
#include "RTClib.h"
#include <LiquidCrystal.h>
#include "TimerOne.h"

RTC_DS1307 RTC;
DateTime now; // Holds the current date and time information
const int hourPin = 2;
const int minPin = 3;
const int alarmPin = 4;
const int alarmTogglePin = 5;
const int buzzPin = 6;
const int chargePin = 7;
const int backlightPin = 53;

boolean alarm = false; // Holds the current state of the alarm if on or off
boolean alarmSet = false; //Is true if alarm button is being held down
boolean buzzer = false; // holds if the buzzer is on or off
boolean buzzer_oscillate = false;
int buzzer_overflows = 0;
boolean charge = false;
int hourPinState = 0;
int minPinState = 0;
int alarmPinState = 0;

int lastHourPinState = 1;
int lastMinPinState = 1;
int count = 0;
int alarmcount = 0;
int alarmHour = 0;
int alarmMin = 0;
int chargeHour = 25;
int chargeMin = 61;
LiquidCrystal lcd(46, 47, 48, 49, 50, 51, 52);


void setup() {
  Wire.begin();
  Serial.begin(9600);

  // Initialize LCD
  lcd.begin(16, 2);
  lcd.print("Program Starting");

  //Set up RTC
  RTC.begin();
  //RTC.adjust(DateTime(__DATE__, __TIME__));
  now = RTC.now();
  
  // Initialize the alarm set time
  alarmHour = now.hour();
  if(now.minute() > 59){
    alarmHour += 1;
    alarmMin = 60 - now.minute();
  }
  else {
    alarmMin = now.minute() + 1;
  }
  
  Serial.print("The alarm is initially set to ");
  Serial.print(alarmHour);
  Serial.print(":");
  Serial.print(alarmMin);
  Serial.print("\n");

  // Initialize timer for oscillating the buzzer
  Timer1.initialize(500000);
  Timer1.attachInterrupt(handleBuzzer);

  //I/O

  //buttons
  pinMode(hourPin, INPUT);
  pinMode(minPin, INPUT);
  pinMode(alarmPin, INPUT);
  pinMode(alarmTogglePin, INPUT_PULLUP);
  pinMode(chargePin, OUTPUT);
  pinMode(backlightPin, OUTPUT);
  pinMode(buzzPin, OUTPUT);
  digitalWrite(buzzPin, HIGH);
  digitalWrite(buzzPin, HIGH);
  digitalWrite(chargePin, HIGH);
  digitalWrite(backlightPin, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  alarmPinState = digitalRead(alarmPin);
  alarm = digitalRead(alarmTogglePin);
  if (!alarm){
    charge = true;
    digitalWrite(chargePin, LOW);
  }
  else {
    charge = false;
    digitalWrite(chargePin, HIGH);
    if (now.hour() > chargeHour || (now.hour() == chargeHour && now.minute() >= chargeMin) && now.hour() < alarmHour || ( now.hour() == alarmHour && now.minute() < alarmMin)){
      charge = true;
      digitalWrite(chargePin, LOW);
   }
  }
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
          setChargeTime();

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
          setChargeTime();
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
      if (now.second() == 0) {
        if (alarm) {
          charge = false;
          digitalWrite(chargePin, HIGH);
          Serial.println("initially enabling buzzer");
          buzzer = true;
        }
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

   if (chargeHour == now.hour()){
    if (chargeMin == now.minute()){
      if (now.second() == 0) {
        charge = true;
        digitalWrite(chargePin, LOW);
        Serial.println("started charging");
      }
     }
  }

  
  if(alarmSet){
    lcd.clear();
    //print the current alarm set time
    lcd.print("ALARM " + String(alarmHour < 10 ? "0" : "") + String(alarmHour) + ":" + (alarmMin < 10 ? "0" : "") + String(alarmMin));
  }
 //setChargeTime();
}

void showTime(){
  now = RTC.now();
  Serial.print("The current time is ");
  Serial.print((now.hour() < 10 ? "0" : "") + String(now.hour()) + ":" + (now.minute() < 10 ? "0" : "") + now.minute() + ":" + (now.second() < 10 ? "0" : "") + now.second() + "\n");
  lcd.clear();
  lcd.print((now.hour() < 10 ? "0" : "") + String(now.hour()) + ":" + (now.minute() < 10 ? "0" : "") + now.minute() + ":" + (now.second() < 10 ? "0" : "") + now.second() + (alarm ? "  ALARM" : ""));
}
void setChargeTime(){
   Serial.println(chargeHour);
   Serial.println(chargeMin);
  //decrement everything by 70 minutes
  if(alarm){
    //charge = false;
    //digitalWrite(chargePin, HIGH);
    
    if (alarmHour == 0){
      chargeHour = 24;
    }
    else {
      chargeHour = alarmHour - 1;
    }
    if(alarmMin <= 10){
        if (alarmHour == 0){
          chargeHour = 24;
        }
        else {
          chargeHour = chargeHour - 1;
        }
       chargeMin =  60 - (10 - alarmMin);
    }
    else {
     chargeMin = alarmMin - 10;
    }
    if (now.hour() > chargeHour || (now.hour() == chargeHour && now.minute() >= chargeMin) && now.hour() < alarmHour || ( now.hour() == alarmHour && now.minute() < alarmMin)){
      Serial.println("yes");
      charge = true;
      digitalWrite(chargePin, LOW);
   }
   Serial.println(chargeHour);
   Serial.println(chargeMin);
 }
//if now is between charge time and current time begin charging and set charge to 1
}

void handleBuzzer() {  
  if (buzzer) {
    if (buzzer_overflows > 10) {
      buzzer_overflows = 0;
      digitalWrite(buzzPin, HIGH);
      buzzer = false;
      return;
    }
    
    if (buzzer_oscillate) {
      digitalWrite(buzzPin, LOW);
    }
    else {
      digitalWrite(buzzPin, HIGH);
    }
    
    buzzer_oscillate = !buzzer_oscillate;
    buzzer_overflows++;
  }
}
