


void preGame() {                     // After startup both buttons must be pushed to start a 10 minute countdown. After the countdown the buzzer sounds and play can begin

Serial.println("Pre-Game. Press button to start. ");
displayClear();
confirmSetting();

while (digitalRead(interruptBlue)+(digitalRead(interruptRed)) <= 0) { // Do nothing until a button is pressed

standbyPurple();
}

Serial.println("Pre-Game started! ");

  
  oldTime = millis();  
while (firstCountDown > 50) {              // Do nothing but count down and show the time on the clock
digitalWrite(redLEDRelay, HIGH);
digitalWrite(blueLEDRelay, HIGH);


  currentTime = millis();
  loopDelay = currentTime - oldTime;
  firstCountDown = firstCountDown - loopDelay;
updateDigits(firstCountDown);
matrix.writeDisplay();
  oldTime = currentTime;   
    
}

  digitalWrite(buzzerRelay, HIGH);           // Chirp after first delay is finished
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
  digitalWrite(blueLEDRelay, LOW);
  digitalWrite(redLEDRelay, LOW);
  digitalWrite(buzzerRelay,LOW);


  mode = 1;
  preGameBuzzer = true;
  modeSelected();
}


}
