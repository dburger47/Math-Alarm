#include <LiquidCrystal_I2C.h>
#include "uRTCLib.h"


//set up LCD screen
LiquidCrystal_I2C lcd(0x27,16,2);

// uRTCLib rtc;
uRTCLib rtc(0x68);


void setup() {
  delay(3000);
  // put your setup code here, to run once:
  //setup for LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  //setup serial monitor
  Serial.begin(9600);

  URTCLIB_WIRE.begin();
  rtc.set(50, 59, 23, 5, 13, 1, 22); //(second, minute, hour, dayOfWeek, dayOfMonth, month, year)

}

//this function gets the time from the RTC
void get_time(){
  //refresh to get recent time 
  rtc.refresh();

  //get hours, mins, secs
  int hours = rtc.hour();
  int mins = rtc.minute();
  int secs = rtc.second();

  lcd.setCursor(0, 0);
  if (hours < 10) lcd.print("0");
  lcd.print(hours);
  lcd.print(":");
  if (mins < 10) lcd.print("0");
  lcd.print(mins);
  lcd.print(":");
  if (secs < 10) lcd.print("0");
  lcd.print(secs);

}
void loop() {
  // put your main code here, to run repeatedly:
 get_time();


}
