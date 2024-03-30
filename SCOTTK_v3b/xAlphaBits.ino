




void screenPrint(byte position0, byte position1, byte position2, byte position3, byte position4) {
   matrix.writeDigitRaw (0, position0); //   
   matrix.writeDigitRaw (1, position1); //
   matrix.writeDigitRaw (2, position2); // 
   matrix.writeDigitRaw (3, position3); //
   matrix.writeDigitRaw (4, position4); //
   matrix.writeDisplay();
  
}


void numberToWord(long modeToDisplay) {

 // _____________________________Switch Case for setup of the selected game mode
 
  switch (mode) {       // Pregame delay (mode 0) needs to start in void setup
    case 0:
    screenPrint(n, o, blank, P, E);
      break;
    case 1:
    screenPrint(H, I, blank, L, L);
      break;
    case 2: 
      screenPrint(t, I, blank, m0, m1);
      // updateDigits((modeToDisplay*60)*1000);
      break;
    case 3: 
    screenPrint(S, n, blank, I, P);
      break;
    case 4:
    screenPrint(I, n, blank, t, blank);
      break;
    case 5:
      updateDigits((modeToDisplay*60)*1000);
      break;       
    case 6:     
    screenPrint(S, t, blank, o, P);
      
      break;
    case 7:    
    screenPrint(G, r, blank, I, F);
      break;    
    case 8:
      updateDigits((modeToDisplay*60)*1000);
      break;
    case 10:    
      updateDigits((modeToDisplay*60)*1000);
      break;  
    case 11:    
      updateDigits((modeToDisplay*60)*1000);
      break;  
      
}
//_______________________________ end game mode setup




}
