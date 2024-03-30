void grifball_timer_setup() {


displayClear();
updateDigits(gameTimeSetPoint);
  oldTime = millis();
while (modeTimer < modeTimeOut) {                             // Game timer. A game is a full set out rounds
gameTimeSetPoint = selectSetPointValue(gameTimeSetPoint, gameTimeSetPointMinimum);

  currentTime = millis();
  modeTimer = modeTimer + (currentTime - oldTime);
  oldTime = currentTime;
  
}

confirmSetting();
displayClear();
updateDigits(regroupTimeSetPoint); 
modeTimer = 0;
  while (modeTimer < modeTimeOut) {                        // Regroup between rounds timer
regroupTimeSetPoint = selectSetPointValue(regroupTimeSetPoint, regroupTimeSetPointMinimum);  
    
  currentTime = millis();
  modeTimer = modeTimer + (currentTime - oldTime);
  oldTime = currentTime;
  
}
regroupTime = regroupTimeSetPoint; 
gameTime = gameTimeSetPoint; 
updateDigits(gameTime);
confirmSetting();
}


void grifball_timer() {
  if (!timerStart) {
    Serial.println(" ");
    Serial.println("Blue light ");
    gameTime = gameTimeSetPoint;
    nextRoundStarting = gameTime - firstRoundReGroupSetpoint;
    updateDigits(gameTime);
    lastLightShift = timerCountDown;
    Serial.println(" ");
    Serial.print("Round Time set to: ");
    Serial.println(gameTime);
    Serial.print("The first game starts at: ");
    Serial.println(nextRoundStarting);


    
    Serial.println("Waiting for trigger to start the game ");
    
        isochromaticLEDs(blue, 180, 150);
        FastLED.show();
     while (!digitalRead(interruptBlue) + !digitalRead(interruptRed) == 2) { // Do nothing with blue lights on until either button is pushed
        digitalWrite(buzzerRelay, LOW);
        }
Serial.println("Purple light ");
        allOrange();
        FastLED.show();
timerStart = true;
loopDelay = 0;
currentTime = millis();
nextRoundStarting = gameTime - firstRoundReGroupSetpoint;
regroupTime = true;  // chirp every second at launch
newRoundTrigger = false;
  }




  
   gameTime = gameTime - loopDelay; 
   updateDigits(gameTime);




  
  if (newRoundTrigger && !firstRoundTrigger) {               // Button was pushed signaling the end of the last round. Time to regroup. 
    newRoundTrigger = false;
    regroupTime = true; 
    nextRoundStarting = gameTime - regroupTimeSetPoint; //Set time that the new games starts
   
  }
  
  if (regroupTime && (nextRoundStarting > gameTime)) {
    regroupTime = false;  // new game has started
    inPlay = true;
    newRoundStarted = gameTime; 
        allOrange();
        FastLED.show();
  }

  if (inPlay && (newRoundStarted - gameTime < 5000)) {  // 5 second buzzer signaling that a new game has started
     Serial.println("Buzzer on! ");
    digitalWrite(buzzerRelay, HIGH); 
  }

  if ((newRoundStarted - gameTime > 5000) && (newRoundStarted - gameTime < 7000)) {
    Serial.println("Buzzer off! ");
    digitalWrite(buzzerRelay, LOW); 
    firstRoundTrigger = false;
    
  }

 
 if (!inPlay && regroupTime) {      // trigger a chirp if less than new game setpoint
  if (newSecond) {                   // chirp buzzer once every second
  Serial.println("Chirp Buzzer");
  
blinkLEDs(buzzerRelay, 100); 
confirmSetting();
}
}

if (digitalRead(interruptRed) + digitalRead(interruptBlue) == 1) {
  Serial.println("Round over ");
  newRoundTrigger = true;
  inPlay = false;
  
        isochromaticLEDs(purple, 180, 0);
        FastLED.show();
  
}
  
  if (gameTime <= 50) {                // trigger a long buzzer for 10 seconds after end of game 
  Serial.println("Time expired! ");
  digitalWrite(buzzerRelay, HIGH);
  strobeRedAndBlue(10);
  digitalWrite(buzzerRelay,LOW);
  delay(100);
  while (true) {
    strobeRedAndBlue(10);

}
  
}


  
}





void newRoundInterrupt() {
  
  if (digitalRead(interruptRed) + digitalRead(interruptBlue) == 2) {return;}
  
  newRoundTrigger = true;
  inPlay = false;
  //digitalWrite(blueLEDRelay, LOW);
  allRed();
  
  
}
