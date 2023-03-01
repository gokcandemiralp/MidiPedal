/*
  DigitalReadSerial

  Reads a digital input on pin 2, prints the result to the Serial Monitor

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/DigitalReadSerial
*/

// digital pin 2 has a pushbutton attached to it. Give it a name:
int Z = 2;
int S0 = 3;
int S1 = 4;
int S2 = 5;
int S3 = 6;
int adress = 0;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  // make the pushbutton's pin an input:
  pinMode(Z, INPUT);
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
}

void iterateMUX(){
  adress = (adress + 1) % 3;
  digitalWrite(S0,adress%2);
  digitalWrite(S1,(adress/2)%2);
  digitalWrite(S2,(adress/4)%2);
  digitalWrite(S3,(adress/8)%2);
  // Serial.println(adress);
}

bool readMuxPin(){
  int val = digitalRead(Z);
  if(!val){
      Serial.println(adress);
  }
  return true;
}

// the loop routine runs over and over again forever:
void loop() {
  readMuxPin();
  checkAdress();
  delay(100);  // delay in between reads for stability
}
