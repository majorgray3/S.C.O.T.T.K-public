void bombSetUp() {
  

displayClear();
updateDigits(bombCountDownSetPoint);
matrix.writeDisplay();
  oldTime = millis();
  Serial.print("Set the bomb timer: ");
  Serial.println(bombCountDownSetPoint);
while (modeTimer < modeTimeOut) {                        // Bomb timer

bombCountDownSetPoint = selectSetPointValue(bombCountDownSetPoint, bombCountDownSetPointMinimum);
 
  currentTime = millis();
  modeTimer = modeTimer + (currentTime - oldTime);
  oldTime = currentTime;


}


randomSeed(analogRead(3));          // start random number generator. Tie it to whatever value happens to be on the unused floating pin #3
bombCountDown = bombCountDownSetPoint;
updateDigits(bombCountDown);
confirmSetting();
}

void bomb_HardMode() {

  if (!bombArmed) {
    Serial.println("Purple light ");
    updateDigits(bombCountDown);
    lastLightShift = bombCountDown;
     while (!digitalRead(interruptRed)) { // Do nothing with purple lights on until the red button is pushed
        digitalWrite(buzzerRelay, LOW);
        standbyPurple();
        }
Serial.println("Red light ");
attachInterrupt(digitalPinToInterrupt(interruptBlue), blueButtonBreak, RISING);
digitalWrite(blueLEDRelay, LOW);
digitalWrite(redLEDRelay, HIGH);
bombArmed = true;
pressedWhileRed = false;
pressedWhilePurple = false;
pressedWhileBlue = false;
  oldTime = currentTime;
  currentTime = millis();
  loopDelay = 0;
  }


 bombCountDown = bombCountDown - (loopDelay*purpleSpeedUp); 
 updateDigits(bombCountDown);

  if ((bombCountDown <= bombCountDownSetPoint) && (bombCountDown > (bombCountDownSetPoint-3000))) {
  digitalWrite(buzzerRelay, HIGH);
 }
 else {digitalWrite(buzzerRelay, LOW);}


if ((((bombCountDown/1000) % 3) == 0) && (lastLightShift-bombCountDown > 2100)) {  // shift light color every 3 seconds. Scale with speed ups
   shiftLights_HardMode();
}



  if (bombCountDown <= 0) {
  Serial.println("Boom! ");
  digitalWrite(buzzerRelay, HIGH);
  digitalWrite(redLEDRelay, HIGH);
  digitalWrite(blueLEDRelay, LOW);
  delay(10000);
  digitalWrite(buzzerRelay,LOW);
  while (true);  // end of game, red wins 


}

if (pressedWhileRed) {
  Serial.print("RED: Boom");
  digitalWrite(redLEDRelay, HIGH);
  digitalWrite(blueLEDRelay, LOW); 
  digitalWrite(buzzerRelay, HIGH);
  delay(5000);
  digitalWrite(buzzerRelay, LOW);
  matrix.writeDigitRaw (2,B00000010); //
  while (true) {
    blinkLEDs(redLEDRelay, 2000);// end of game, red wins 
  }
}

if (pressedWhilePurple) {
  Serial.println("PURPLE: Speed up");
  lastInterrupt = bombCountDown;
  purpleSpeedUp = purpleSpeedUp + 5;   // clock speeds up
  pressedWhilePurple = false;
}

if (pressedWhileBlue) {
  Serial.println("Sollenberger Disarm sequence initiated.");
  sollenberger = true;
  pressedWhileBlue = false;
  disarmCountDown = disarmCountDownSetPoint;
}

ledBrightness();

}


void shiftLights_HardMode() {
  lastLightShift = bombCountDown; 
  rgbStatus = random(1, 7);
   switch (rgbStatus) {       

    case 1:
        Serial.println("RED");
        redConstant = true;
        blueConstant = false;
      break;
    case 2: 
        Serial.println("PURPLE");
        redConstant = true;
        blueConstant = true;
      break;
    case 3:
       Serial.println("BLUE");
        redConstant = false;
        blueConstant = true;
      break;
    case 4:
       Serial.println("BLANK");
        redConstant = false;
        blueConstant = false;
      break;
    case 5:
        Serial.println("PURPLE");
        redConstant = true;
        blueConstant = true;
      break;
    case 6:
       Serial.println("BLANK");
        redConstant = false;
        blueConstant = false;
      break;
    
  
}
}

void ledBrightness() {                   // this function should automatically set LED brightness according to the current countdown
  int brightnessIndex = lastLightShift - bombCountDown;
  if (brightnessIndex < 255) {
  analogWrite(redLEDRelay, (redConstant*brightnessIndex));
  analogWrite(blueLEDRelay, (blueConstant*brightnessIndex));
  
}
else if (brightnessIndex > 2745) {
    analogWrite(redLEDRelay, redConstant*(3000-brightnessIndex));
  analogWrite(blueLEDRelay, blueConstant*(3000-brightnessIndex));
}
else {
  digitalWrite(redLEDRelay, redConstant);
  digitalWrite(blueLEDRelay, blueConstant);

}
}




void shiftLights() {
  lastLightShift = bombCountDown; 
  rgbStatus = rgbStatus + 1;
if (rgbStatus == 4) {rgbStatus = 1;}
   switch (rgbStatus) {       

    case 1:
        Serial.println("RED");
        digitalWrite(redLEDRelay, HIGH);
        digitalWrite(blueLEDRelay, LOW);
      break;
    case 2: 
        Serial.println("PURPLE");
        digitalWrite(redLEDRelay, HIGH);
        digitalWrite(blueLEDRelay, HIGH);     
      break;
    case 3:
       Serial.println("BLUE");
       digitalWrite(redLEDRelay, LOW);
       digitalWrite(blueLEDRelay, HIGH);
      break;
  
}
}





void blueButtonBreak() {
 if (rgbStatus == 1) {
  pressedWhileRed = true;
 } 
 else if ((rgbStatus == 2) || (rgbStatus == 5)) {
  pressedWhilePurple = true; 
 }
 
 else if ((rgbStatus == 4) || (rgbStatus == 6)) {   // do nothing if button is pushed while the LED's are blank
  // do nothing; 
 }
 else {
  pressedWhileBlue = true;
 }
  
}

void redButtonBreak() {
  
}

void reArmCountdown() {
  redConstant = false;
  blueConstant = false;
  digitalWrite(blueLEDRelay, HIGH);
  digitalWrite(redLEDRelay, LOW);
 purpleSpeedUp = 1;
 disarmCountDown = disarmCountDown - loopDelay; 
 updateDigits(disarmCountDown);
  sollenberger = (1-digitalRead(interruptRed));
 if (disarmCountDown < 100) {
   Serial.println("BLUE: Disarmed");
   updateDigits(bombCountDown);
   matrix.writeDigitRaw (2,B00000010); //
    while (true) {
    
    blinkLEDs(blueLEDRelay, 2000);  // end of game, blue wins     
  }
 }

if (newSecond) {                   // chirp buzzer once every second
  Serial.println("Chirp Buzzer");
blinkLEDs(buzzerRelay, 100); 
}

if (!sollenberger) {                    // turn off the LED's if returning to bomb arm countdown
  digitalWrite(redLEDRelay, LOW);
  digitalWrite(blueLEDRelay, LOW);
}
else {digitalWrite(blueLEDRelay, HIGH);}
 
}
