void shiftOutLSB_4(uint8_t val){
	for (uint8_t i = 0; i < 4; i++)  {
    digitalWrite(dataPin, val & 1);
    val >>= 1;
		digitalWrite(clockPin, HIGH);
		digitalWrite(clockPin, LOW);		
	}
}

void shiftOutLSB_8(uint8_t val){
	for (uint8_t i = 0; i < 8; i++)  {
    digitalWrite(dataPin, val & 1);
    val >>= 1;
		digitalWrite(clockPin, HIGH);
		digitalWrite(clockPin, LOW);		
	}
}