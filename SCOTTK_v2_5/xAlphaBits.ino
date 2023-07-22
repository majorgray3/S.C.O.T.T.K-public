




void screenPrint(byte position0, byte position1, byte position2, byte position3, byte position4) {
   matrix.writeDigitRaw (0, position0); //   
   matrix.writeDigitRaw (1, position1); //
   matrix.writeDigitRaw (2, position2); // 
   matrix.writeDigitRaw (3, position3); //
   matrix.writeDigitRaw (4, position4); //
   matrix.writeDisplay();
  
}
