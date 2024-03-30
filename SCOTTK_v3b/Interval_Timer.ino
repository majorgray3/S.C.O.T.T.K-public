void interval_timer_setup() {


displayClear();
updateDigits(roundTimeSetPoint);
  oldTime = millis();
while (modeTimer < modeTimeOut) {                             // Work timer
roundTimeSetPoint = selectSetPointValue(roundTimeSetPoint, roundTimeSetPointMinimum);

  currentTime = millis();
  modeTimer = modeTimer + (currentTime - oldTime);
  oldTime = currentTime;
  
}

confirmSetting();
displayClear();
updateDigits(restTimeSetPoint);
modeTimer = 0;
  while (modeTimer < modeTimeOut) {                        // Rest timer
restTimeSetPoint = selectSetPointValue(restTimeSetPoint, restTimeSetPointMinimum);  
    
  currentTime = millis();
  modeTimer = modeTimer + (currentTime - oldTime);
  oldTime = currentTime;
  
}
restTime = restTimeSetPoint;
roundTime = roundTimeSetPoint;
confirmSetting();
}



void boxing_timer() {


if (rest) {
 restTime = restTime - loopDelay; 
 updateDigits(restTime);
  isochromaticLEDs(red+15, 255, 150);
  FastLED.show();
}


if (!rest) {
 roundTime = roundTime - loopDelay; 
 updateDigits(roundTime);
  isochromaticLEDs(green+5, 255, 150);
  FastLED.show();
}


if (restTime < 10) {
restTime = restTimeSetPoint;
rest = false;
//digitalWrite(redLEDRelay, LOW);
  
}

if (roundTime < 10) {
roundTime = roundTimeSetPoint;
rest = true;
//digitalWrite(blueLEDRelay, LOW);
}

if (((restTime < 150)||(roundTime < 150)) || ((roundTime < 30500) && (roundTime > 30400))) {        // chirp at the end of each interval; Chirp when 30 seconds of work remains
  digitalWrite(buzzerRelay, HIGH);
  Serial.println("BUZZER ON!!! ");
}
else {
  digitalWrite(buzzerRelay, LOW);
}


}
