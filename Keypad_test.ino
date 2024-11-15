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
  // put your setup code here, to run once:
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
  //start serial at 9600
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
char val = keypadListen();
Serial.println(val);
}

