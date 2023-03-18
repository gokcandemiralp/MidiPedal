void rotaryCheck(){
  uint8_t val = digitalRead(pinA);
  if(!encoderRotaryState && val){
    if (val == digitalRead(pinB) ) {
      encoderModifier--;
    }
    else {
      encoderModifier++;
    }
  }
  encoderRotaryState = val;
}