#include <MIDI.h>

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

uint8_t encoderModifier = 0;
uint8_t midiValues[4][3] = {{0,0,0},{0,0,0},{0,0,0},{0,0,0}};

uint8_t switchPins[4] = {switchPin0,switchPin1,switchPin2,switchPin3};
uint8_t LEDPins[4] = {LED0Pin,LED1Pin,LED2Pin,LED3Pin};
uint8_t switchStates[4];
uint8_t selectedSwitch = 0;

uint8_t letter = 0;
bool encoderButtonState = true;
bool encoderRotaryState = true;
int aVal;
bool systemState = false;
bool editButtonState = false;
uint8_t tempVal;
uint8_t fieldModula = 8;
MIDI_CREATE_DEFAULT_INSTANCE();


int charsArray[10]={0b11111100,0b01100000,0b11011010,0b11110010,0b01100110,0b10110110,0b10111110,0b11100000,0b11111110,0b11110110}; // digits as 7 segment
int lettersArray[3]={0b00011110,0b00011010,0b01111010}; // "t","c","d"

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
  MIDI.begin();
  Serial.begin(115200);
  initPins();
}
void loop() {
    tempVal = midiValues[selectedSwitch][letter]+encoderModifier;    
    if(!systemState){writeDigit(0, lettersArray[letter]);}
    readButtons();
    rotaryCheck();
    if(!systemState){writeDigit(1, charsArray[(tempVal/100)%10]);}
    readButtons();
    rotaryCheck();
    if(!systemState){writeDigit(2, charsArray[(tempVal/10)%10]);}
    readButtons();
    rotaryCheck();
    if(!systemState){writeDigit(3, charsArray[tempVal%10]);}
    readButtons();
    rotaryCheck();
    //Serial.println(millis());
}

void readButtons(){
  bool val = digitalRead(editPin);
  if(!val && editButtonState){
    saveEncoderChange();
    sevenSegOff();
    systemState = !systemState;
  }
  editButtonState = val; 


  if(!systemState){                                           // if in play mode fields cannot be changed
    val = digitalRead(fieldPin);
    if(val && !encoderButtonState){                           // read Field Changing Button on the encoder
      saveEncoderChange();
      if(letter==0){fieldModula = 8;}
      if(letter==1){fieldModula = 16;}
      if(letter==2){fieldModula = 128;}
      letter = (letter+1)%3;                                  // iterates to the next letter thus the next field
    }
    encoderButtonState = val;                                 // sets the button state to detect if the state of the switch is changed   
  }

  for(int i=0 ; i<4 ; ++i ){ //read all 4 switches
    val = digitalRead(switchPins[i]);
    if(switchStates[i] != val){                                 // detects if the state of the switch is changed
      midiAction();
      if(!systemState){                                         // if in edit mode switches are used to choose which switch to edit
        saveEncoderChange();                                    // resets the modifier by the encoder
      }
      selectedSwitch = i;                                       // updates the selected switch for other functions
      for(int i=0 ; i<4 ; ++i ){                                // switches LEDs on and of regarding the selected LED
        digitalWrite(LEDPins[i], selectedSwitch==i);  
      }
      switchStates[i] = val;                                    // saves the new state to detect any further changes
    }
  }
}
