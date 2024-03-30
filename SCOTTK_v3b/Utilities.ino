void loadingScreen(int quickCountDown) {
matrix.writeDisplay();
int quickCountDownFromSetup =  quickCountDown;
int screen = map(quickCountDownFromSetup, loadingScreenCountDownSetpoint, 0, 0, 13);


switch (screen) {
  case 0:
    matrix.writeDigitRaw (0,B00111001); //
    matrix.writeDigitRaw (1,B00001001); //
    matrix.writeDigitRaw (2,B00000010); //
    matrix.writeDigitRaw (3,B00001001); //
    matrix.writeDigitRaw (4,B00001111); //
    break;
  case 1:    
    matrix.writeDigitRaw (1,B00001000); //    
    break;
  case 2:
    matrix.writeDigitRaw (0,B00111000); //
    break;
  case 3:
    matrix.writeDigitRaw (0,B00011000); //
    break;
  case 4:
    matrix.writeDigitRaw (0,B00001000); //
    break;
  case 5:
    matrix.writeDigitRaw (0,B00000000); //
    break;
  case 6:    
    matrix.writeDigitRaw (1,B00000000); //
    break;
  case 7:
    matrix.writeDigitRaw (3,B00000001); //    
    break;
  case 8:
    matrix.writeDigitRaw (4,B00000111); //
    break;
  case 9:
    matrix.writeDigitRaw (4,B00000011); //
    break;
  case 10:
    matrix.writeDigitRaw (4,B00000001); //
    break;
  case 11:
    matrix.writeDigitRaw (4,B00000000); //
    break;
  case 12:

    matrix.writeDigitRaw (3,B00000000); //
    break;
  
}


}

void batteryIndicator() {

//digitalWrite(batteryIndicatorEnable, HIGH);


   matrix.writeDigitRaw (0, B01111100); //   bAtt
   matrix.writeDigitRaw (1, B01110111); //
   matrix.writeDigitRaw (2,B00000000); // 
   matrix.writeDigitRaw (3, B01111000); //
   matrix.writeDigitRaw (4, B01111000); //
   matrix.writeDisplay();

   while (digitalRead(interruptBlue)+(digitalRead(interruptRed)) <= 0) { // Do nothing until a button is pressed

}

setup();
}

void testAllOutputs() {
 allRed();
 delay(1000);
 //digitalWrite(redLEDRelay, LOW);
 delay(1000);
  //digitalWrite(greenLEDRelay, HIGH);
 delay(1000);
 //digitalWrite(greenLEDRelay, LOW);
 delay(1000);
  allBlue();
 delay(1000);
 //digitalWrite(blueLEDRelay, LOW);
 delay(1000);
  
  
  allRed();
  allBlue();
  //digitalWrite(greenLEDRelay, HIGH); 
  //digitalWrite(batteryIndicatorEnable, HIGH);
  delay(3000);
  blinkLEDs(buzzerRelay, 100);

   while (digitalRead(interruptBlue)+(digitalRead(interruptRed)) <= 0) { // Do nothing until a button is pressed

}

setup();
    
}

void checkBatteryCharge() {
  
}

void blinkLEDs(int relayNumber, int interval) {

switch (relayNumber) {       // Pre-game delay (mode 0) needs to start in void setup
    case 5:  
    isochromaticLEDs(blue, 255, 180);
    FastLED.show();
    delay(interval);
    allOff();
    FastLED.show();
    delay(interval/2);     
     break;
    case 6:  
    isochromaticLEDs(red, 255, 180);
    FastLED.show();
    delay(interval);
    allOff();
    FastLED.show();
    delay(interval/2);
      break;   


}
}

void confirmSetting() {
  for (int x = 0; x < 1; x++) {
blinkLEDs(redLEDRelay, 50);
blinkLEDs(blueLEDRelay, 50);
blinkLEDs(blueLEDRelay, 50);
delay(100);
}
}

void strobeRedAndBlue(int howLongInSeconds) {
  for (int x = 0; x < howLongInSeconds; x++) {
  for (int x = 0; x < 2; x++) {
blinkLEDs(redLEDRelay, 50);
blinkLEDs(blueLEDRelay, 50);
blinkLEDs(blueLEDRelay, 50);
delay(100);
}
delay(500);
  }
}

void standbyPurple() {
  for (int x=0; x < NUM_LEDS; x++) {
  leds[x] = CRGB::Purple;
  }
  FastLED.show();
}

void showOffLEDs() {
  fadeAmount = 10;
  screenPrint(r,E,blank,d,blank);   // print the word rEd
  while (repeatRed!=0) {
  analogWrite(redLEDRelay, brightness);
  brightness = brightness + fadeAmount;
  if (brightness <= 0 || brightness >= 155) {
    fadeAmount = -fadeAmount;
  }
   delay(5);
   if (brightness==0) {
    repeatRed--;
   }
   mode = mode + digitalRead(interruptBlue);
  }

  //digitalWrite(redLEDRelay, LOW);
  fadeAmount = 2;
  screenPrint(b,L,blank,U,E);   // print the word bLUE
  while (repeatBlue!=0) {
  analogWrite(blueLEDRelay, brightness);
  brightness = brightness + fadeAmount;
  if (brightness <= 0 || brightness >= 155) {
    fadeAmount = -fadeAmount;
  }
   delay(8);
   if (brightness==0) {
    repeatBlue--;
   }
   mode = mode + digitalRead(interruptBlue);
  }
  //digitalWrite(blueLEDRelay, LOW);
  displayClear();
  fadeAmount = 2;
  screenPrint(b,E,blank,L,L);   // print the word bLUE
    while (repeatPurple!=0) {
  analogWrite(redLEDRelay, brightness);
  analogWrite(blueLEDRelay, brightness);
  brightness = brightness + fadeAmount;
  if (brightness <= 0 || brightness >= 255) {
    fadeAmount = -fadeAmount;
  }
  if (fadeAmount < 2) {
    delay(1);
  }
   else {delay(8);}
   if (brightness==0) {
    repeatPurple--;
   }
   mode = mode + digitalRead(interruptBlue);
  }  
 
}



void allRed() {
  for (int x=0; x < NUM_LEDS; x++) {
  leds[x] = CRGB::Red;
  }
  FastLED.show();
}

void allOrange() {

  for (int x=0; x < NUM_LEDS; x++) {
  leds[x] = CRGB::OrangeRed;
  }
  FastLED.show();
}

void allBlue() {
  for (int x=0; x < NUM_LEDS; x++) {
  leds[x] = CRGB::Blue;
  }
  FastLED.show();
}

void allGreen() {
  for (int x=0; x < NUM_LEDS; x++) {
  leds[x] = CRGB::Green;
  }
  FastLED.show();
}

void allDarkOliveGreen() {
  for (int x=0; x < NUM_LEDS; x++) {
  leds[x] = CRGB::DarkOliveGreen;
  }
  FastLED.show();
}

void allDarkSlateGrey() {
  for (int x=0; x < NUM_LEDS; x++) {
  leds[x] = CRGB::DarkSlateGrey;
  }
  FastLED.show();
}

void allDarkTurquoise() {
  for (int x=0; x < NUM_LEDS; x++) {
  leds[x] = CRGB::DarkTurquoise;
  }
  FastLED.show();
}


void allDarkViolet() {
  for (int x=0; x < NUM_LEDS; x++) {
  leds[x] = CRGB::DarkViolet;
  }
  FastLED.show();
}



void allOff() {
  for (int x=0; x < NUM_LEDS; x++) {
  leds[x] = CRGB::Black;
  }
  FastLED.show();
}


void isochromaticLEDs(uint8_t theColor, int8_t sat, int8_t brightness) {
   for (int x=0; x < NUM_LEDS; x++) {
    leds[x] = CHSV(theColor, sat, brightness);
    }
    FastLED.show();
}


void RvB_Animation() {
fadeToBlackBy( leds, NUM_LEDS, 60);
    uint8_t dothue = 171;
  for( int i = 0; i < numberOfPlayers - (1 * chadPlaying) - (1 * taylorPlaying); i++) {
    leds[beatsin16( i+7, 0, NUM_LEDS-1 )] |= CHSV(dothue, 255, 90);
    if (dothue == 171) { dothue = 255;} else dothue = 171;
  }

  if (chadPlaying) {
    if (chadRedorBlue < 128) {
      leds[chadHidingSpot] = CRGB::Red;
    } 
    else {
      leds[chadHidingSpot] = CRGB::Blue;
    }
  }

  if (taylorPlaying) {
    if (taylorRedorBlue < 128) {
      leds[taylorHidingSpot] = CRGB::Red;
    } 
    else {
      leds[taylorHidingSpot] = CRGB::Blue;
    }
  }

  FastLED.show();
FastLED.delay(1000/FRAMES_PER_SECOND); 
}
