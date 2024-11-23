#include <LiquidCrystal_I2C.h>
#include "uRTCLib.h"

//set up LCD screen
LiquidCrystal_I2C lcd(0x27,16,2);

// uRTCLib rtc;
uRTCLib rtc(0x68);

//define column pins
int C1 = 6;
int C2 = 7;
int C3 = 8;
int C4 = 9;

//define row pins
int R1 = 2;
int R2 = 3;
int R3 = 4;
int R4 = 5;

//define keymap
const int nRows = 4;
const int nCols = 4;

//buzzer pin
int buzzer = 11;

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

//define alarm toggle variable
int alarmEN = 0;

//define alarm to 00:00 to start
String alarm_time = "00:00";

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
  delay(1000);
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

  //set buzzer as output
  pinMode(buzzer, OUTPUT);

  //setup serial monitor
  Serial.begin(9600);

  //setup RTC
  URTCLIB_WIRE.begin();
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
  
  //set cursor to 4, 0
  lcd.setCursor(4, 0);

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
  
  // Ensure valid range for hours
  if (h < 0 || h > 23) {
    lcd.clear();
    lcd.print("Invalid Hours!");
    delay(1500);
    lcd.clear();
    return; // Exit if invalid input
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
  
  // Ensure valid range for hours
  if (m < 0 || m > 59) {
    lcd.clear();
    lcd.print("Invalid Mins!");
    delay(1500);
    lcd.clear();
    return; // Exit if invalid input
  }
  //set secs to 00
  int s = 0;
  
  //send time to RTC
  rtc.set(s, m, h, 0, 0, 0, 0); //(second, minute, hour, dayOfWeek, dayOfMonth, month, year)
  lcd.clear(); //clear the LCD
}

//this function sets the alarm from user inputs to the keypad and retuens an alarm string
String set_alarm() {
  // Initialize the LCD display
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("A Hours: "); // Prompt user for hours

  int A_h = 0; // Variable to store hours
  // Get two characters for hours
  for (int i = 0; i < 2; i++) {
    char key = 0;

    // Wait for a key press
    while (!key) key = keypadListen();

    // Check if the key is a valid digit
    if (key >= '0' && key <= '9') {
      lcd.print(key); // Display the digit on the LCD
      A_h = A_h * 10 + (key - '0'); // Accumulate the number
    } else {
      i--; // If invalid key, repeat the step
    }
  }

  // Ensure valid range for hours
  if (A_h < 0 || A_h > 23) {
    lcd.clear();
    lcd.print("Invalid Hours!");
    delay(1500);
    lcd.clear();
    return "00:00"; // Exit if invalid input
  }

  lcd.setCursor(0,1);
  lcd.print("A Mins: "); // Prompt user for minutes

  int A_m = 0; // Variable to store minutes
  // Get two characters for minutes
  for (int i = 0; i < 2; i++) {
    char key = 0;

    // Wait for a key press
    while (!key) key = keypadListen();

    // Check if the key is a valid digit
    if (key >= '0' && key <= '9') {
      lcd.print(key); // Display the digit on the LCD
      A_m = A_m * 10 + (key - '0'); // Accumulate the number
    } else {
      i--; // If invalid key, repeat the step
    }
  }

  // Ensure valid range for minutes
  if (A_m < 0 || A_m > 59) {
    lcd.clear();
    lcd.print("Invalid Mins!");
    delay(1500);
    lcd.clear();
    return "00:00"; // Exit if invalid input
  }

  // Format and return the alarm time
  char alarmTime[6];
  sprintf(alarmTime, "%02d:%02d", A_h, A_m); // Ensure two-digit formatting
  delay(100);

  lcd.clear();
  return String(alarmTime);
}


void disp_alarm(String alarm, int state){
  lcd.setCursor(0,1);

  lcd.print("A: ");
  lcd.print(alarm);

  if (state == 1){
    lcd.setCursor(12, 1);
    lcd.print(" ON");
  }

  if (state == 0){
    lcd.setCursor(12, 1);
    lcd.print("OFF");
  }
}

int toggle_alarm(){
  //toggle the alarm and return
  alarmEN = !alarmEN;
  return alarmEN;

}

void loop() {
  // put your main code here, to run repeatedly:
  //read keypad press
  char key = keypadListen();
  Serial.println(key);

  //set the time if D is pressed
  if (key == 'D'){
    set_time();
  }

  //set the alarm if A is pressed 
  if (key == 'A'){
    alarm_time = set_alarm();
  }

  //toggle alarm if B is pressed
  int alarm_state;
  if (key == 'B'){
    alarm_state = toggle_alarm();
  }

  //display current time
  String current_time = get_time(); 
  //display alarm time
  disp_alarm(alarm_time, alarm_state);

  if (current_time == alarm_time && alarm_state == 1){
    digitalWrite(buzzer, HIGH);
    delay(750);
    digitalWrite(buzzer, LOW);
  }

  
}
