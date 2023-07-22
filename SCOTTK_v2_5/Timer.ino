

void timerSetup () {


displayClear();
updateDigits(timerCountDownSetPoint);
matrix.writeDisplay();
  oldTime = millis();
  Serial.print("Set the timer: ");
  Serial.println(timerCountDownSetPoint);
while (modeTimer < modeTimeOut) {

 timerCountDownSetPoint = selectSetPointValue(timerCountDownSetPoint, timerCountDownSetPointMinimum);
 
currentTime = millis();
  modeTimer = modeTimer + (currentTime - oldTime);
 oldTime = currentTime;

}

confirmSetting();

}


void timer() {
  if (!timerStart) {
    Serial.println("Blue light ");
    timerCountDown = timerCountDownSetPoint;
    updateDigits(timerCountDown);
    lastLightShift = timerCountDown;
     while (!digitalRead(interruptBlue)) { // Do nothing with purple lights on until the red button is pushed
        digitalWrite(buzzerRelay, LOW);
        digitalWrite(redLEDRelay, LOW);
        digitalWrite(blueLEDRelay, HIGH);
        }
Serial.println("Purple light ");
        digitalWrite(redLEDRelay, HIGH);
        digitalWrite(blueLEDRelay, HIGH);
timerStart = true;
loopDelay = 0;
currentTime = millis();
  }

 timerCountDown = timerCountDown - loopDelay; 
 updateDigits(timerCountDown);

  if (timerCountDown <= 0) {
  Serial.println("Time expired! ");
  digitalWrite(buzzerRelay, HIGH);
  digitalWrite(redLEDRelay, HIGH);
  digitalWrite(blueLEDRelay, HIGH);
  delay(4000);
  digitalWrite(buzzerRelay,LOW);
  delay(1000);
  while (true) {
    blinkLEDs(redLEDRelay, 2000);
    blinkLEDs(blueLEDRelay, 2000);

}
  
}
}




long selectSetPointValue(long theSetPoint, long minimumLimit) {
long x = theSetPoint + (digitalRead(interruptBlue) *timerAdjustmentValue) - (digitalRead(interruptRed) *timerAdjustmentValue);

if (theSetPoint < (minimumLimit + 1000)) {x = (minimumLimit + 100);}  // no less than minimum limit (plus 100 milliseconds)
if (x != oldTheSetPoint) {
  updateDigits(x);
  modeTimer = 0;
  }
  delay(selectionSpeed(modeTimer));
  oldTheSetPoint = x;
  return x;

  
  
}
