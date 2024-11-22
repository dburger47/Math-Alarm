// This function will create a math problem, consisting of a solution and two expressions.
// The user will need to select the correct expression that matches the given solution.
// Inputs to the function will be number of problems, maybe difficulty???
void mathProblem() {
// create an expression with 3-4 operands that add or subtract to a number between x and y (probably 1ish and 20ish)
  // use random() to generate the operands, and to determine if + or - between them
  int numOperands = 4;
  int operands[numOperands];

  // create array of random operands
  for (int i = 0; i < numOperands; i++) {
    operands[i] = random(1, 10); 

    // roll for + or -
    if (random(0, 2) == 0) {
      operands[i] *= -1;
    }

  }

  // solve the created problem
  int solution;
  for (int i = 0; i < numOperands; i++) {
    solution += operands[i];
  }

  // create strong of correct expression
  String correctString = "";
  for (int i = 0; i < numOperands; i++) {
    if (operands[i] > 0) {
      correctString += "+"; // Add a + sign for positive operands
    }
    correctString += String(operands[i]); // Append the operand value
  }


  Serial.print("B: ");
  Serial.println(correctString);

  // change two of the operands to create a wrong expression
  int index1 = random(0, numOperands);
  int index2;
  do {
    index2 = random(0, numOperands); // Ensure the second index is different
  } while (index2 == index1);
  
  // assign new random values using ternary operator
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

  Serial.print("A: ");
  Serial.println(wrongString);

}




void setup() {
  Serial.begin(9600);

}

void loop() {
  mathProblem();
  delay(5000);

}
