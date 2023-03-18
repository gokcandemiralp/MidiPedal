void midiAction(){
  MIDI.sendControlChange(midiValues[selectedSwitch][0], 
                         midiValues[selectedSwitch][2], 
                         midiValues[selectedSwitch][1]);
}