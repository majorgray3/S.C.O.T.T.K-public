
void preGameSetup () {


displayClear();
updateDigits(firstCountDownSetpoint);
matrix.writeDisplay();
  oldTime = millis();
  Serial.print("Set the timer: ");
  Serial.println(firstCountDownSetpoint);
while (modeTimer < modeTimeOut) {

 firstCountDownSetpoint = selectSetPointValue(firstCountDownSetpoint, firstCountDownSetpointMinimum);
 
currentTime = millis();
  modeTimer = modeTimer + (currentTime - oldTime);
 oldTime = currentTime;

}


modeTimer = 0;
confirmSetting();

}

void preGame() {                     // After startup both buttons must be pushed to start a 10 minute countdown. After the countdown the buzzer sounds and play can begin


  if (!timerStart) {
    firstCountDown = firstCountDownSetpoint;
    updateDigits(firstCountDown);
    allOff();
timerStart = true;
loopDelay = 0;
currentTime = millis();
  }
Serial.println("Pre-Game. Press button to start. ");

confirmSetting();

bool flopRedandBlue = false;
isochromaticLEDs(purple, 180, 0);
FastLED.show();
screenPrint(blank, r, blank, d, Y);
oldTime = millis(); 
while (digitalRead(interruptBlue)+(digitalRead(interruptRed)) <= 0) { // Do nothing until a button is pressed

RvB_Animation();

}

Serial.println("Pre-Game started! ");

  
  oldTime = millis();  
while (firstCountDown > 50) {              // Do nothing but count down and show the time on the clock


  currentTime = millis();
  loopDelay = currentTime - oldTime;
  firstCountDown = firstCountDown - loopDelay;
updateDigits(firstCountDown);
matrix.writeDisplay();
  oldTime = currentTime;   

 if (newSecond) {                   // Show a visual indicator that a countdown is in place
 
 if (firstCountDown < 5100) {      // trigger a chirp during last 5 seconds
  Serial.println("Chirp Buzzer");  
    digitalWrite(buzzerRelay, HIGH);
}

   Serial.print("blink ");
  
    if (flopRedandBlue) {
      blinkLEDs(redLEDRelay, 150);
      flopRedandBlue = false;
      Serial.println("red. ");
    }
    else if (!flopRedandBlue) {
      blinkLEDs(blueLEDRelay, 150);
      flopRedandBlue = true;
      Serial.println("blue. ");
    }

  digitalWrite(buzzerRelay, LOW);

}
  
    
}

  digitalWrite(buzzerRelay, HIGH);           // Alarm after first delay is finished
  delay(4000);
  digitalWrite(buzzerRelay, LOW);


  oldTime = millis();  
  if (mode == 0) {   // second countdown for King of The Hill Pregame 
while (secondCountDown > 50) {              // Do nothing but count down and show the time on the clock

    
  currentTime = millis();
  loopDelay = currentTime - oldTime;
  secondCountDown = secondCountDown - loopDelay;
updateDigits(secondCountDown);
matrix.writeDisplay();

  oldTime = currentTime; 
}

digitalWrite(buzzerRelay, HIGH);         // Sound the Alarm to signal that the game can begin

  delay(6000);
  //digitalWrite(blueLEDRelay, LOW);
  //digitalWrite(redLEDRelay, LOW);
  digitalWrite(buzzerRelay,LOW);


  mode = 1;
  preGameBuzzer = true;
  modeSelected();
}


}
