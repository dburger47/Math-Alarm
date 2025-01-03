//libraries to control the LCD and RTC
#include <LiquidCrystal_I2C.h>
#include "uRTCLib.h"

//set up LCD screen
LiquidCrystal_I2C lcd(0x27,16,2);

// uRTCLib rtc;
uRTCLib rtc(0x68);

//define column pins for keypad
int C1 = 6;
int C2 = 7;
int C3 = 8;
int C4 = 9;

//define row pins for keypad
int R1 = 2;
int R2 = 3;
int R3 = 4;
int R4 = 5;

//these values are used in the keypadListen function in the for loops
const int nRows = 4;
const int nCols = 4;

//buzzer pin
int buzzer_pin = 11;

//define keymap for keypad
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

//this function returns the char that is pressed on the keypad
char keypadListen(){
  //define inital keyvalue
  char keyValue = 0;

 //set all columns high 
  digitalWrite(C1, HIGH);
  digitalWrite(C2, HIGH);
  digitalWrite(C3, HIGH);
  digitalWrite(C4, HIGH);

  //set each column low 1 by 1
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
        //map button press to keymap
        keyValue = Keymap[row][col];
        delay(100);
      }
      
    }
    //set column back high for next iteration
    digitalWrite(pin_cols[col], HIGH);
    
  }  
  //return button that was pressed
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


// This function will create a math problem, consisting of a solution and two expressions.
// The user will need to select the correct expression that matches the given solution.
// Inputs to the function will be number of problems, maybe difficulty???
bool mathProblem() {
  randomSeed(analogRead(A0));
  lcd.clear();
// create an expression with 3-4 operands that add or subtract to a number between x and y (probably 1ish and 20ish)
  // use random() to generate the operands, and to determine if + or - between them
  int numOperands = 4;
  int operands[numOperands];

  // create array of random operands
  for (int i = 0; i < numOperands; i++) {
    operands[i] = random(1, 10) * (random(0, 2) == 0 ? -1 : 1);
  }

  // solve the created problem
  int solution;
  for (int i = 0; i < numOperands; i++) {
    solution += operands[i];
  }

  // create string of correct expression
  String correctString = "";
  for (int i = 0; i < numOperands; i++) {
    if (operands[i] > 0) {
      correctString += "+"; // Add a + sign for positive operands
    }
    correctString += String(operands[i]); // Append the operand value
  }


  Serial.print("Correct: ");
  Serial.println(correctString);

  // change two of the operands to create a wrong expression
  // by generating two random indexes
  int index1 = random(0, numOperands);
  int index2;
  do {
    index2 = random(0, numOperands); // Ensure the second index is different
  } 
  
  while (index2 == index1);
  
  // assign new random values to the generated indexes 
  operands[index1] = random(1, 10) * (random(0, 2) == 0 ? -1 : 1);
  operands[index2] = random(1, 10) * (random(0, 2) == 0 ? -1 : 1);

  // Create a string of the updated operands with + for positive numbers
  String wrongString = "";
  for (int i = 0; i < numOperands; i++) {
    if (operands[i] > 0) {
      wrongString += "+"; // Add a + sign for positive operands
    }
    wrongString += String(operands[i]); // Append the operand value
  }

  Serial.print("Wrong: ");
  Serial.println(wrongString);
  Serial.print("Solution: ");
  Serial.println(solution);

  // next, roll random(0,2) for correct expression's cursor row
  int cursorRowCorrect = random(0,2);

  // set wrong expression to opposite of rolled
  int cursorRowWrong = 1 - cursorRowCorrect;
    
  char ansRow;
  char otherRow;
  // indicate which answer is correct for button press on keypad
  if (cursorRowCorrect == 0) {
    ansRow = 'A';
    otherRow = 'B';
  }

  else { 
    otherRow = 'A';
    ansRow = 'B';
  }

  // printing expressions to LCD
    // print correct expression
    lcd.setCursor(0,cursorRowCorrect);
    lcd.print(ansRow);
    lcd.print(":");
    lcd.print(correctString);

    // print wrong expression
    lcd.setCursor(0,cursorRowWrong);
    lcd.print(otherRow);
    lcd.print(":");
    lcd.print(wrongString);

    //print solution in top right corner
    lcd.setCursor(13,0);
    lcd.print(solution);

  // set key to zero initially
  char key = 0;
  char keyAns;

  // wait for a key press
  while (!key) key = keypadListen(); {
    
    // validate key press
    if (key == 'A' || key == 'B') {
      keyAns = key;
    } 
    else {
      key = 0; // If invalid key, repeat the step
    }
  }

  bool correct = 0;
  
  // check for correct answer
  if (keyAns == ansRow) {
    correct = 1;
    Serial.print("Correct: ");
    Serial.println(correct);
  }

  return correct;

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
  pinMode(buzzer_pin, OUTPUT);

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

// void buzzer() {
//   digitalWrite(buzzer_pin, HIGH);
//   delay(500);
//   digitalWrite(buzzer_pin, LOW);
//   delay(250);
// }

void buzzer_ON() {
  digitalWrite(buzzer_pin, HIGH);
}

void buzzer_OFF() {
  digitalWrite(buzzer_pin, LOW);
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

bool alarmExit = 0; // alarm exit flag

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

  while(current_time == alarm_time && alarm_state == 1 && alarmExit == 0){
    lcd.clear();
    lcd.setCursor(0,0);
    bool correctFlag = 0;

    while (correctFlag == 0) {
      buzzer_ON();
    
      correctFlag = mathProblem();
    }

    alarmExit = 1;
    buzzer_OFF();
    lcd.clear();
  }

  if (alarmExit == 1 && current_time != alarm_time) {
    alarmExit = 0; // Reset the flag for the next alarm cycle
  }
}
