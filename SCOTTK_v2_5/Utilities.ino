void loadingScreen(int quickCountDown) {
matrix.writeDisplay();
int quickCountDownFromSetup =  quickCountDown;
int screen = map(quickCountDownFromSetup, loadingScreenCountDownSetpoint, 0, 0, 13);




switch (screen) {
  case 0:
    matrix.writeDigitRaw (0,B00111001); //
    matrix.writeDigitRaw (1,B00001001); //
    matrix.writeDigitRaw (2,B00000010); //
    matrix.writeDigitRaw (3,B00001001); //
    matrix.writeDigitRaw (4,B00001111); //
    break;
  case 1:    
    matrix.writeDigitRaw (1,B00001000); //    
    break;
  case 2:
    matrix.writeDigitRaw (0,B00111000); //
    break;
  case 3:
    matrix.writeDigitRaw (0,B00011000); //
    break;
  case 4:
    matrix.writeDigitRaw (0,B00001000); //
    break;
  case 5:
    matrix.writeDigitRaw (0,B00000000); //
    break;
  case 6:    
    matrix.writeDigitRaw (1,B00000000); //
    break;
  case 7:
    matrix.writeDigitRaw (3,B00000001); //    
    break;
  case 8:
    matrix.writeDigitRaw (4,B00000111); //
    break;
  case 9:
    matrix.writeDigitRaw (4,B00000011); //
    break;
  case 10:
    matrix.writeDigitRaw (4,B00000001); //
    break;
  case 11:
    matrix.writeDigitRaw (4,B00000000); //
    break;
  case 12:

    matrix.writeDigitRaw (3,B00000000); //
    break;
  
}


}

void batteryIndicator() {

digitalWrite(batteryIndicatorEnable, HIGH);


   matrix.writeDigitRaw (0, B01111100); //   bAtt
   matrix.writeDigitRaw (1, B01110111); //
   matrix.writeDigitRaw (2,B00000000); // 
   matrix.writeDigitRaw (3, B01111000); //
   matrix.writeDigitRaw (4, B01111000); //
   matrix.writeDisplay();

   while (digitalRead(interruptBlue)+(digitalRead(interruptRed)) <= 0) { // Do nothing until a button is pressed

}

setup();
}

void testAllOutputs() {
 digitalWrite(redLEDRelay, HIGH);
 delay(1000);
 digitalWrite(redLEDRelay, LOW);
 delay(1000);
  digitalWrite(greenLEDRelay, HIGH);
 delay(1000);
 digitalWrite(greenLEDRelay, LOW);
 delay(1000);
  digitalWrite(blueLEDRelay, HIGH);
 delay(1000);
 digitalWrite(blueLEDRelay, LOW);
 delay(1000);
  
  
  digitalWrite(redLEDRelay, HIGH);
  digitalWrite(blueLEDRelay, HIGH);
  digitalWrite(greenLEDRelay, HIGH); 
  digitalWrite(batteryIndicatorEnable, HIGH);
  delay(3000);
  blinkLEDs(buzzerRelay, 100);

   while (digitalRead(interruptBlue)+(digitalRead(interruptRed)) <= 0) { // Do nothing until a button is pressed

}

setup();
    
}

void checkBatteryCharge() {
  
}

void blinkLEDs(int relayNumber, int interval) {
  digitalWrite(relayNumber, HIGH);
  delay(interval);
  digitalWrite(relayNumber, LOW);
  delay(interval/2);
}

void confirmSetting() {
  for (int x = 0; x < 1; x++) {
blinkLEDs(redLEDRelay, 50);
blinkLEDs(blueLEDRelay, 50);
blinkLEDs(blueLEDRelay, 50);
delay(100);
}
}

void strobeRedAndBlue(int howLongInSeconds) {
  for (int x = 0; x < howLongInSeconds; x++) {
  for (int x = 0; x < 2; x++) {
blinkLEDs(redLEDRelay, 50);
blinkLEDs(blueLEDRelay, 50);
blinkLEDs(blueLEDRelay, 50);
delay(100);
}
delay(500);
  }
}

void standbyPurple() {
  analogWrite(blueLEDRelay, 30);
  analogWrite(redLEDRelay, 30);
}

void showOffLEDs() {
  fadeAmount = 10;
  screenPrint(r,E,blank,d,blank);   // print the word rEd
  while (repeatRed!=0) {
  analogWrite(redLEDRelay, brightness);
  brightness = brightness + fadeAmount;
  if (brightness <= 0 || brightness >= 155) {
    fadeAmount = -fadeAmount;
  }
   delay(5);
   if (brightness==0) {
    repeatRed--;
   }
   mode = mode + digitalRead(interruptBlue);
  }

  digitalWrite(redLEDRelay, LOW);
  fadeAmount = 2;
  screenPrint(b,L,blank,U,E);   // print the word bLUE
  while (repeatBlue!=0) {
  analogWrite(blueLEDRelay, brightness);
  brightness = brightness + fadeAmount;
  if (brightness <= 0 || brightness >= 155) {
    fadeAmount = -fadeAmount;
  }
   delay(8);
   if (brightness==0) {
    repeatBlue--;
   }
   mode = mode + digitalRead(interruptBlue);
  }
  digitalWrite(blueLEDRelay, LOW);
  displayClear();
  fadeAmount = 2;
  screenPrint(b,E,blank,L,L);   // print the word bLUE
    while (repeatPurple!=0) {
  analogWrite(redLEDRelay, brightness);
  analogWrite(blueLEDRelay, brightness);
  brightness = brightness + fadeAmount;
  if (brightness <= 0 || brightness >= 255) {
    fadeAmount = -fadeAmount;
  }
  if (fadeAmount < 2) {
    delay(1);
  }
   else {delay(8);}
   if (brightness==0) {
    repeatPurple--;
   }
   mode = mode + digitalRead(interruptBlue);
  }  
 
}
