void kingOfTheHillSetup(){
  modeTimer = 0;
Serial.print("Clock Ownership before setup: ");
Serial.println(clockOwnership);

timerCountDownSetPoint = hillTimerSetPoint;
timerSetup();
 hillTimerSetPoint = timerCountDownSetPoint;
 hillTimer = hillTimerSetPoint;



 redTime = hillTimer;    // initially set both red ...
 blueTime = hillTimer;   // ... and blue hill timers the same
 

}




void kingOfTheHill(){
 if (preGameBuzzer) {
  allOff();
  digitalWrite(buzzerRelay,LOW);
  preGameBuzzer = false;
}

  
    // if we get a valid byte, read analog ins:
  if (Serial.available() > 0) {
    string = Serial.readString();
    if (string.indexOf("Red") != -1) {
    Serial.println("Red Time: ");
    redControl();
    }
  else if (string.indexOf("Blue") != -1) {
    Serial.println("Blue Time: ");
    blueControl();
  }
}

Serial.flush();
delay(10);


if (clockOwnership.indexOf("Red") != -1) {
  redTime = redTime - loopDelay; 
  redTimeOut = redTimeOut - loopDelay;
  updateDigits(redTime);
  }
else if (clockOwnership.indexOf("Blue") != -1) {
  blueTime = blueTime - loopDelay; 
  blueTimeOut = blueTimeOut - loopDelay;
  updateDigits(blueTime);
  }
  else {
   Serial.println("nOPE ");
   matrix.writeDigitRaw (0,B00110111); //n
   matrix.writeDigitRaw (1,B00111111); //O
   matrix.writeDigitRaw (2,B00000000); // 
   matrix.writeDigitRaw (3,B01110011); //P
   matrix.writeDigitRaw (4,B01111001); //E
  }


matrix.writeDisplay();
if (redTime <= 0) {
  redWins(6000);
}
if (blueTime <= 0) {
  blueWins(6000);
}

if (((redTimeOut <= warning) && (redTimeOut >= (warning - 100))) || ((blueTimeOut <= warning) && (blueTimeOut >= (warning - 100)))) {        // buzzer warning when only "warning" seconds remain before time out
  digitalWrite(buzzerRelay, HIGH);
  Serial.println("BUZZER ON!!! ");
}
else {
  digitalWrite(buzzerRelay, LOW);
}




if (redTimeOut < 1 || blueTimeOut < 1) {    // turn off LED's and set ownership to "nOPE" if button is not refreshed before timeout expires
  redTimeOut = 1;
  blueTimeOut = 1;
  clockOwnership = "None: ";
  allOff();
  allOff();
  }


}

void redControl(){
  clockOwnership = "Red: ";
  redTimeOut = timeOut;  
  //allOff();
  allRed();

}

void blueControl(){  
  clockOwnership = "Blue: ";
  blueTimeOut = timeOut;  
  allBlue();
}


void redWins(int buzzerOnTime) {                                 // *****************Red Wins!!
  Serial.println("Red Wins! ");
  digitalWrite(buzzerRelay, HIGH);  
  allRed();
  delay(buzzerOnTime);
  digitalWrite(buzzerRelay,LOW);
  delay(5000);
  while (true) {
    allBlue();
    updateDigits(blueTime);
    matrix.writeDigitRaw (2,B00000010); // 
    matrix.writeDisplay();
    Serial.println(blueTime);
    delay(5000);
    allRed();
    updateDigits(redTime);
    matrix.writeDigitRaw (2,B00000010); // 
    matrix.writeDisplay();
    Serial.println(redTime);
    delay(5000);
  }
}

void blueWins(int buzzerOnTime) {                              // *****************Blue Wins!!
  Serial.println("Blue Wins! ");
  digitalWrite(buzzerRelay, HIGH);  
  allOff();
  allBlue();
  delay(buzzerOnTime);
  digitalWrite(buzzerRelay,LOW);
  delay(5000);
   while (true) {
    allOff();
    allRed();    
     updateDigits(redTime);
    matrix.writeDigitRaw (2,B00000010); // 
    matrix.writeDisplay();
    Serial.println(redTime);
    delay(10000);
    allOff();
    allBlue();
     updateDigits(blueTime);
    matrix.writeDigitRaw (2,B00000010); // 
    matrix.writeDisplay();
    Serial.println(blueTime);
    delay(5000);
  }
}



void updateDigits(long tRaw) {

   long t = tRaw/1000;
   int mtens = (t/600);  // the minute tens position is not limited to base 6. 
   int mones = (t - (600*mtens))/60;                                 
   int stens = ((t-(((10*mtens)+mones)*60))/10)%6;
   int sones = t - ((((10*mtens)+mones)*60)+(10*stens));
   int sonesHalf = tRaw - (t*1000);
   if (sones == oldsones) {
   newSecond = false;
   }
   else {
    newSecond = true;
    oldsones = sones;
   } 
   if (newSecond) {
  Serial.print(mtens);
  Serial.print(mones);
  Serial.print(":");
  Serial.print(stens);
  Serial.println(sones);
   }

  // Commented out for bench testing at work
  if (mtens == 0) {
    matrix.writeDigitRaw (0,B00000000); // Just leave the digit blank if it is zero
  }
  else {
  matrix.writeDigitNum(0, mtens);
  }
  matrix.writeDigitNum(1, mones);
  
  matrix.writeDigitNum(3, stens);
  matrix.writeDigitNum(4, sones);


 if ((sonesHalf < 250) || ((sonesHalf > 750))) {   //blink the colon each second
  matrix.writeDigitRaw (2,B00000010); // 
}
else {
  matrix.writeDigitRaw (2,B00000000); // 
}
matrix.writeDisplay();

}
