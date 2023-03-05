uint8_t pinA = 2;
uint8_t pinB = 3;
uint8_t dataPin = 4;
uint8_t latchPin = 5;
uint8_t clockPin = 6;
uint8_t fieldPin = A2;

uint8_t volatile encoderPosCount = 0;
uint8_t midiValues[3] = {0,0,0};

uint8_t letter = 0;
bool encoderState = false;
int aVal;


int digitsArray[10]={0b11111100,0b01100000,0b11011010,0b11110010,0b01100110,0b10110110,0b10111110,0b11100000,0b11111110,0b11110110};
int lettersArray[3]={0b00011110,0b00011010,0b01111010};

void setup() {
  pinMode (pinA,INPUT_PULLUP);
  pinMode (pinB,INPUT);
  attachInterrupt(digitalPinToInterrupt(pinA), detectChange, CHANGE);

  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(fieldPin, OUTPUT);
  Serial.begin(9600);
}
void loop() {
  write7Segment();
  readButtons();
}

void write7Segment(){
    uint8_t fieldVal = midiValues[letter];
    uint8_t tempVal = fieldVal+encoderPosCount;
    writeDigit(0, lettersArray[letter]);
    writeDigit(1, digitsArray[(tempVal/100)%10]);
    writeDigit(2, digitsArray[(tempVal/10)%10]);
    writeDigit(3, digitsArray[tempVal%10]);
}

void writeDigit(int digitNo, int digitValue){
    digitalWrite(latchPin, LOW);
    shiftOutLSB_4(15-pow(2,(3-digitNo)));
    shiftOutLSB_8(digitValue);
    digitalWrite(latchPin, HIGH);
}

void readButtons(){
  bool val = digitalRead(fieldPin);
  if(val && !encoderState){ // field Button
    midiValues[letter] += encoderPosCount;
    letter = (letter+1)%3;
    encoderPosCount = 0;
  }
  encoderState = val;
}

void detectChange(){
  aVal = digitalRead(pinA);
  if (digitalRead(pinB) == aVal) {
    encoderPosCount ++;
  }
  else {
    encoderPosCount--;
  }
}
