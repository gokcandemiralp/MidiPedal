uint8_t pinA = 2;
uint8_t pinB = 3;
uint8_t dataPin = 4;
uint8_t latchPin = 5;
uint8_t clockPin = 6;
uint8_t fieldPin = 7;
uint8_t editPin = 8;
uint8_t LED0Pin = 9;
uint8_t LED1Pin = 10;
uint8_t LED2Pin = 11;
uint8_t LED3Pin = 12;
uint8_t switchPin0 = A2;
uint8_t switchPin1 = A3;
uint8_t switchPin2 = A4;
uint8_t switchPin3 = A5;

uint8_t volatile encoderModifier = 0;
uint8_t midiValues[4][3] = {{0,0,0},{0,0,0},{0,0,0},{0,0,0}};

uint8_t switchPins[4] = {switchPin0,switchPin1,switchPin2,switchPin3};
uint8_t LEDPins[4] = {LED0Pin,LED1Pin,LED2Pin,LED3Pin};
uint8_t switchStates[4];
uint8_t selectedSwitch = 0;

uint8_t letter = 0;
bool encoderState = true;
int aVal;


int digitsArray[10]={0b11111100,0b01100000,0b11011010,0b11110010,0b01100110,0b10110110,0b10111110,0b11100000,0b11111110,0b11110110};
int lettersArray[3]={0b00011110,0b00011010,0b01111010};

void initPins(){
  pinMode (pinA,INPUT);
  pinMode (pinB,INPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode (fieldPin,INPUT);
  pinMode (editPin,INPUT);
  pinMode(LED0Pin, OUTPUT);
  pinMode(LED1Pin, OUTPUT);
  pinMode(LED2Pin, OUTPUT);
  pinMode(LED3Pin, OUTPUT);
}

void setup() {
  Serial.begin(9600);
  initPins();
  attachInterrupt(digitalPinToInterrupt(pinA), pinA_ISR, RISING);
}
void loop() {
    uint8_t tempVal = midiValues[selectedSwitch][letter]+encoderModifier;    
    writeDigit(0, lettersArray[letter]);
    readButtons();
    writeDigit(1, digitsArray[(tempVal/100)%10]);
    readButtons();
    writeDigit(2, digitsArray[(tempVal/10)%10]);
    readButtons();
    writeDigit(3, digitsArray[tempVal%10]);
    readButtons();
}

void writeDigit(int digitNo, int digitValue){
    digitalWrite(latchPin, LOW);
    shiftOutLSB_4(15-pow(2,(3-digitNo)));
    shiftOutLSB_8(digitValue);
    digitalWrite(latchPin, HIGH);
}

void readButtons(){
  bool val = digitalRead(fieldPin);
  if(val && !encoderState){ // read field Button
    midiValues[selectedSwitch][letter] += encoderModifier;
    letter = (letter+1)%3;
    encoderModifier = 0;
  }
  encoderState = val;

  for(int i=0 ; i<4 ; ++i ){ //read all 4 switches
    val = digitalRead(switchPins[i]);
    if(switchStates[i] != val){
      midiValues[selectedSwitch][letter] += encoderModifier;
      encoderModifier = 0;
      selectedSwitch = i;
      for(int i=0 ; i<4 ; ++i ){
        digitalWrite(LEDPins[i], selectedSwitch==i);
        if(selectedSwitch==i){Serial.println(LEDPins[i]);}
      }
      switchStates[i] = val;
    }
  }
}

void pinA_ISR(){
  if (digitalRead(pinB)) {
    encoderModifier++;
  }
  else {
    encoderModifier--;
  }
}
