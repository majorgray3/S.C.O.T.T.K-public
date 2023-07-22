void stopWatch() {
  if (!start) {
    while (digitalRead(interruptBlue) < 1) {
      // do nothing until the blue button is pressed
    }
    start = true;
    attachInterrupt(digitalPinToInterrupt(interruptRed), pauseStopWatch, RISING);
    loopDelay = 0;
    currentTime = millis();
    }
    
    stopWatchTime = stopWatchTime + loopDelay;
    updateDigits(stopWatchTime);
}


void pauseStopWatch() {
  start = false;
}
