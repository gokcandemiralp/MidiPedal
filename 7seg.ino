void shiftOutLSB(uint8_t size, uint8_t val){
	for (uint8_t i = 0; i < size; i++)  {
    digitalWrite(dataPin, val & 1);
    val >>= 1;
		digitalWrite(clockPin, HIGH);
		digitalWrite(clockPin, LOW);
	}
}

void sevenSegOff(){
  for(int i=0 ; i<4 ; ++i ){ //itaretes all 4 seven segment modules
    writeDigit(i, 0b00000000);
  }
}

void writeDigit(int digitNo, int digitValue){
    digitalWrite(latchPin, LOW);
    shiftOutLSB(4, 15-pow(2,(3-digitNo)));
    shiftOutLSB(8, digitValue);
    digitalWrite(latchPin, HIGH);
}