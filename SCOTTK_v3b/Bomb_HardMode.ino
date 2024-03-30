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


randomSeed(analogRead(A3));          // start random number generator. Tie it to whatever value happens to be on the unused floating pin #3
bombCountDown = bombCountDownSetPoint;
updateDigits(bombCountDown);
confirmSetting();
}

void bomb_HardMode() {

  if (!bombArmed) {
    Serial.println("Purple light ");
    updateDigits(bombCountDown);
    lastLightShift = bombCountDown;
    digitalWrite(buzzerRelay, LOW);
        isochromaticLEDs(purple, 180, 150);
        FastLED.show();
     while (!digitalRead(interruptRed)) { // Do nothing with purple lights on until the red button is pushed
        
        }
Serial.println("Red light ");
attachInterrupt(digitalPinToInterrupt(interruptBlue), blueButtonBreak, RISING);

isochromaticLEDs(red, 255, 180);
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
  isochromaticLEDs(red, 255, 180);
  //allOff();
  delay(10000);
  digitalWrite(buzzerRelay,LOW);
  while (true);  // end of game, red wins 


}

if (pressedWhileRed) {
  Serial.print("RED: Boom");
  isochromaticLEDs(red, 255, 180);
  //allOff(); 
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
        purpleConstant = false;
        nullConstant = false;
      break;
    case 2: 
        Serial.print("PURPLE: ");
        redConstant = false;
        blueConstant = false;
        purpleConstant = true;
        nullConstant = false;
        Serial.println((red*redConstant) + (blue*blueConstant) + (purple*purpleConstant));
      break;
    case 3:
       Serial.println("BLUE");
        redConstant = false;
        blueConstant = true;
        purpleConstant = false;
        nullConstant = false;
      break;
    case 4:
       Serial.println("BLANK");
        redConstant = false;
        blueConstant = false;
        purpleConstant = false;
        nullConstant = true;
      break;
    case 5:
        Serial.print("PURPLE: ");
        redConstant = false;
        blueConstant = false;
        purpleConstant = true;
        nullConstant = false;
        Serial.println((red*redConstant) + (blue*blueConstant) + (purple*purpleConstant));
      break;
    case 6:
       Serial.println("BLANK");
        redConstant = false;
        blueConstant = false;
        purpleConstant = false;
        nullConstant = true;
      break;
    
  
}
}

void ledBrightness() {                   // this function should automatically set LED brightness according to the current countdown
   int brightnessIndex = lastLightShift - bombCountDown;
   if (brightnessIndex < 255) {
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i].nscale8(brightnessIndex);
    }
    FastLED.show();
    
}
else if ((brightnessIndex > 255) && (brightnessIndex < 2745)) {
  isochromaticLEDs((red*redConstant) + (blue*blueConstant) + (purple*purpleConstant), 255, (!nullConstant * 180));
}
else {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].nscale8(3000 - brightnessIndex);
    }
  FastLED.show();

}
}




void shiftLights() {
  lastLightShift = bombCountDown; 
  rgbStatus = rgbStatus + 1;
if (rgbStatus == 4) {rgbStatus = 1;}
   switch (rgbStatus) {       

    case 1:
        Serial.println("RED");
        isochromaticLEDs(red, 255, 180);
        //allOff();
      break;
    case 2: 
        Serial.println("PURPLE");
        isochromaticLEDs(red, 255, 180);
        isochromaticLEDs(blue, 255, 180);     
      break;
    case 3:
       Serial.println("BLUE");
       //allOff();
       isochromaticLEDs(blue, 255, 180);
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
  
  unsigned long buzzerOnTime; 
  unsigned long buzzerChirpLimit = 100;
  redConstant = false;
  blueConstant = false;
  isochromaticLEDs(blue, 255, 180);
 
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

  Serial.println((buzzerOnTime - disarmCountDown));
if (newSecond) {                   // chirp buzzer once every second

  Serial.println("Chirp Buzzer");
digitalWrite(buzzerRelay, HIGH);
buzzerOnTime = disarmCountDown;
}
if ((buzzerOnTime - disarmCountDown) > buzzerChirpLimit) { digitalWrite(buzzerRelay,LOW);}



if (!sollenberger) {                    // turn off the LED's if returning to bomb arm countdown
 
}
else {isochromaticLEDs(blue, 255, 180);
}
 
}
