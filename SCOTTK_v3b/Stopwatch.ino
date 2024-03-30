void stopWatch() {
  if (!start) {
    while (digitalRead(interruptBlue) < 1) {
      // do nothing until the blue button is pressed
    }

    
    start = true;
    
    loopDelay = 0;
    currentTime = millis();
    }
    RvB_Animation();
    stopWatchTime = stopWatchTime + loopDelay;
    updateDigits(stopWatchTime);
}




void pauseStopWatch() {
  start = false;
}

