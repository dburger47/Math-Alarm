#include <LiquidCrystal_I2C.h>
#include "uRTCLib.h"

bool x = true;
bool y = true;
//set up LCD screen
LiquidCrystal_I2C lcd(0x27,16,2);

// uRTCLib rtc;
uRTCLib rtc(0x68);

//define column pins
int C1 = 5;
int C2 = 4;
int C3 = 3;
int C4 = 2;

//define row pins
int R1 = 6;
int R2 = 7;
int R3 = 8;
int R4 = 9;

//define keymap
const int nRows = 4;
const int nCols = 4;

char Keymap[nRows][nCols] = {
  {'A', 'B', 'C', 'D'},
  {'3', '6', '9', '#'},
  {'2', '5', '8', '0'},
  {'1', '4', '7', '*'}
};


//define corresponding pins
int pin_rows[nRows] = {R4, R3, R2, R1};
int pin_cols[nCols] = {C1, C2, C3, C4};

//define inital button values
int buttonPriorValue = 1;
int buttonValue = 0;

char keypadListen(){
  char keyValue = 0;
 //scan columns 1 by 1 
  digitalWrite(C1, HIGH);
  digitalWrite(C2, HIGH);
  digitalWrite(C3, HIGH);
  digitalWrite(C4, HIGH);

  for(int col = 0; col < nCols; col++){
    digitalWrite(pin_cols[col], LOW);
    
  
    //scan rows to see if key is pressed
    for(int row = 0; row < nRows; row++){
      //if row is low return character
      buttonValue = debounce(pin_rows[row], buttonPriorValue);
      if(buttonValue == 0){
        if(buttonValue != buttonPriorValue){
          buttonPriorValue = buttonValue;
        }
        keyValue = Keymap[row][col];
        delay(100);
      }
      
    }
    //set column back high for next iteration
    digitalWrite(pin_cols[col], HIGH);
    
  }  
  return keyValue;
}

int debounce(int PIN, int last_val){
  //read value
  int new_val = digitalRead(PIN);
  //check if is it not equal to last value
  if(new_val != last_val){
    delay(10);
    new_val = digitalRead(PIN);
  }
  return new_val;
}


void setup() {
  delay(3000);
  // put your setup code here, to run once:
  //setup for LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);

  //set columns as outputs
  pinMode(C1, OUTPUT);
  pinMode(C2, OUTPUT);
  pinMode(C3, OUTPUT);
  pinMode(C4, OUTPUT);

  //set rows as input pullup
  pinMode(R1, INPUT_PULLUP);
  pinMode(R2, INPUT_PULLUP);
  pinMode(R3, INPUT_PULLUP);
  pinMode(R4, INPUT_PULLUP);

  //setup serial monitor
  Serial.begin(9600);

  //set up RTC
  URTCLIB_WIRE.begin();
  //set time to midnight
  rtc.set(0, 0, 0, 0, 0, 0, 0); //(second, minute, hour, dayOfWeek, dayOfMonth, month, year)

}

//this function gets the time from the RTC, displays it to the LCD, and returns a time string to be compared with the alarm string
String get_time(){
  //refresh to get recent time 
  rtc.refresh();
  
  //get hours, mins, secs
  int hours = rtc.hour();
  int mins = rtc.minute();
  int secs = rtc.second();

  //store time to be compared for alarm
  String time = "";
  
  //set cursor to 0, 0
  lcd.setCursor(0, 0);

  //make hh format
  if (hours < 10){
    lcd.print("0"); //print to LCD
    time = time + "0"; //append to time string
  }
  lcd.print(hours); //print to LCD
  time = time + String(hours); //append to time string

  //add ":"
  lcd.print(":"); //print to LCD
  time = time + ":";  //append to time string

  //make mm format
  if (mins < 10){
    lcd.print("0"); //print to LCD
    time = time + "0"; //append to time string
  }
  lcd.print(mins); //print to LCD
  time = time + String(mins); //append to time string

  //print ss to LCD
  lcd.print(":");
  if (secs < 10){
    lcd.print("0");
  }
  lcd.print(secs);

  //return time string
  return time;
}

//this function sets the time from user inputs to the keypad
void set_time(){
  //set hours
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Hours: "); //prompt user for hrs 

  int h = 0; //variable to store hh
  //get two char from user
  for (int i = 0; i < 2; i++) {
    char key = 0;

    // Wait for a key press
    while (!key) key = keypadListen();

    // Check if the key is a valid digit
    if (key >= '0' && key <= '9') {
      lcd.print(key); // Display the digit on the LCD
      h = h * 10 + (key - '0'); // Accumulate h
    } 
    else {
      i--; // If invalid key, repeat the step
    }
  }
  
  //set mins
  lcd.setCursor(0, 1);
  lcd.print("Mins: "); //prompt user for mins

  int m = 0; //variable to store mm
  //get two char from user
  for (int i = 0; i < 2; i++) {
    char key = 0;

    // Wait for a key press
    while (!key) key = keypadListen();

    // Check if the key is a valid digit
    if (key >= '0' && key <= '9') {
      lcd.print(key); // Display the digit on the LCD
      m = m * 10 + (key - '0'); // Accumulate m
    } 
    else {
      i--; // If invalid key, repeat the step
    }
  }

  //set secs to 00
  int s = 0;
  
  //send time to RTC
  rtc.set(s, m, h, 0, 0, 0, 0); //(second, minute, hour, dayOfWeek, dayOfMonth, month, year)
  lcd.clear(); //clear the LCD
}

//this function sets the alarm from user inputs to the keypad and retuens an alarm string
String set_alarm(){
  //set hours
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("A Hours: "); //prompt user for hrs

  int A_h = 0; //variable to store hh
  //get two char from user
  for (int i = 0; i < 2; i++) {
    char key = 0;

    // Wait for a key press
    while (!key) key = keypadListen();

    // Check if the key is a valid digit
    if (key >= '0' && key <= '9') {
      lcd.print(key); // Display the digit on the LCD
      A_h = A_h * 10 + (key - '0'); // Accumulate the number
    } 
    else {
      i--; // If invalid key, repeat the step
    }
  }
  
  //set mins
  lcd.setCursor(0, 1);
  lcd.print("A Mins: "); //prompt user for mins

  int A_m = 0; //variable to store mm
  //get two char from user
  for (int i = 0; i < 2; i++) {
    char key = 0;

    // Wait for a key press
    while (!key) key = keypadListen();

    // Check if the key is a valid digit
    if (key >= '0' && key <= '9') {
      lcd.print(key); // Display the digit on the LCD
      A_m = A_m * 10 + (key - '0'); // Accumulate the number
    } 
    else {
      i--; // If invalid key, repeat the step
    }
  }
  //create alarm string
  String alarm_time = String(A_h) + ":" + String(A_m);

  lcd.clear(); //clear LCD
  
  //return alarm string 
  return alarm_time;
}

void loop() {
  // put your main code here, to run repeatedly:
  while(x){
    set_time();
    x = false;
  }
  
  String current_time = get_time(); 
  Serial.print("Time: ");
  Serial.println(current_time);

  String alarm_time;
  while (y){
    alarm_time = set_alarm();
    y = false;
  }
  Serial.print("Alarm: ");
  Serial.println(alarm_time);








}
