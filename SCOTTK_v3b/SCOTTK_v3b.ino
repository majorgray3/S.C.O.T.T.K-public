String currentVersion = "SCOTTK_V3b";

/*   SCOTTK_V3b
  Added new WS2812B LED strip upgrade. 
  KoTH now starts with one minute delay by default
  Set display brightness proportional with initial FastLED brightness
  
  Ready for play testing

  Known issues:
  While adjusting time limits (such as pregame delay) if the user reduces the setpoint to the minimum then the setpoint will freeze at the minimum value and cannot be raised again. 
  The pregame delay for Griffball does not match the pre game chirp pattern for other games have a pre game delay. 
  Griffball chirping might not work at all. 



 */
// Libraries
#include <Wire.h> // Enable this line if using Arduino Uno, Mega, etc.
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

Adafruit_7segment matrix = Adafruit_7segment(); 


String string = "";
String clockOwnership = "None: ";

// _________________________________ FastLED  _______________________________________
#define FASTLED_ALLOW_INTERRUPTS 5
#define FASTLED_INTERRUPT_RETRY_COUNT 1
#include <FastLED.h>

FASTLED_USING_NAMESPACE

#define DATA_PIN    9

#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS    (64 + 62) +1  // 64 on the front. 62 on the back
CRGB leds[NUM_LEDS];

#define BRIGHTNESS          255
#define FRAMES_PER_SECOND  128
uint8_t green = 96;
uint8_t blue = 160;
uint8_t red = 0;
uint8_t purple = 192;


boolean chadPlaying = true;
boolean taylorPlaying = true;
long chadRedorBlue;
long taylorRedorBlue;
long chadHidingSpot; 
long taylorHidingSpot; 

// _________________________________ Global variables declared for each game mode here  _______________________________________
//   Mode = 0: Pregame delay
long firstCountDownSetpoint = 60100;  
long firstCountDownSetpointMinimum = 10100;  

long firstCountDown = firstCountDownSetpoint;   // Variable for first countdown delaying when a game can start (1 minute given in milliseconds)
long secondCountDown = 600100;   // Variable for second countdown when a game can start (10 minutes given in milliseconds)
boolean preGameBuzzer = false;  // Buzzer is turned on at the end of preGame and then turned off at the start of KOTH

// Mode = 1: King of The Hill
long hillTimerSetPoint = 600100; // variable for total time needed to win the game (10 minutes given in milliseconds)
long hillTimer = hillTimerSetPoint; 
long timeOut = 15000; // variable for when a refresh button push is needed (15 seconds given in milliseconds)
long warning = 2000; // variable for when to sound the timeout warning

// Mode = 2: Timer mode
long timerCountDownSetPoint = 600100;
long timerCountDownSetPointMinimum =  10000;
long oldTheSetPoint;
long timerCountDown = timerCountDownSetPoint;  // variable used in timer mode. Initially set to 10 minutes given in milliseconds
long timerAdjustmentValue = 1000; // variable used when setting the timer
boolean timerStart = false;

// Mode = 3: Sniper (one minute delay)


// Mode = 4: Interval Timer
boolean rest = true;
long restTimeSetPoint = 30100;
long restTimeSetPointMinimum = 10000;
long restTime = restTimeSetPoint;
long roundTimeSetPoint = 180100;
long roundTimeSetPointMinimum = 10000;
long roundTime = roundTimeSetPoint; 

// Mode = 5: Bomb Hard Mode
boolean bombTimeSet = false;
boolean bombArmed = false;
long bombCountDownSetPoint = 600100;
long bombCountDownSetPointMinimum = 60000;
long bombCountDown;
long rgbStatus = 1;
long purpleSpeedUp = 1;
long lastInterrupt = 0;
long interruptCoolDown = 5000;
boolean pressedWhileRed = false;
boolean pressedWhilePurple = false;
boolean pressedWhileBlue = false;
long lastLightShift;
boolean redConstant = false;
boolean blueConstant = false;
boolean purpleConstant = false;
boolean nullConstant = false;
boolean sollenberger = false;
long disarmCountDownSetPoint = 30100;
long disarmCountDown = disarmCountDownSetPoint;


// Mode 6: Stopwatch
boolean start = false;
long stopWatchTime = 0;

// Mode = 7: Grifball Timer
long nextRoundStarting = 0;
long newRoundStarted = 0;
long gameTime;
long gameTimeSetPoint = 2400100;
long gameTimeSetPointMinimum = 10000;
long regroupTime;
long firstRoundReGroupSetpoint = 60100;
long regroupTimeSetPoint = 120100;
long regroupTimeSetPointMinimum = 10000;
boolean newRoundTrigger = false;
boolean firstRoundTrigger = true;
boolean newGame = false;
boolean inPlay = false;

// Mode = 8: Pre Game delay
int numberOfPlayers = 10;

// Mode = xx: Objective and Buzzer mode

// _________________________________ Global variables end  _______________________________________

int modeTimer = 0;
int modeTimeOut = 5100; 

long loadingScreenCountDown = 3010;
long loadingScreenCountDownSetpoint = loadingScreenCountDown;

long loopDelay = 0; 
unsigned long currentTime = 0;
unsigned long oldTime = 0;

long redTime = hillTimer;    // initially set both red ...
long blueTime = hillTimer;   // ... and blue hill timers the same

long redTimeOut = timeOut;  // initally set both red .... 
long blueTimeOut = timeOut;   //   ..... and timeout count downs the same.

long oldsones = 0;
long deltaseconds = 0;
boolean newSecond = false;  // variable to flip each time there is a new second. Usefull for triggering something every second.



long mode = 0;   // game mode or phase  (0 King of the hill with pregame delay) (1 king of the hill) (2 Timer) (3 Bomb)
long oldMode = mode;
const byte interruptRed = 2;
const byte interruptBlue = 3;
unsigned int blueButton = 0;
unsigned int redButton = 0;
byte buzzerRelay = 7;   // output pin for the alarm buzzer
const byte redLEDRelay = 6;
const byte blueLEDRelay = 5;
const byte greenLEDRelay = 4;
const byte batteryIndicatorEnable = 8;
int brightness = 0;    // how bright the LED is
int fadeAmount = 2;    // how many points to fade the LED by
int repeatRed = 10;
int repeatBlue=3;
int repeatPurple=1;
byte timesLoopedWhileButtonHeld = 0;
float natrualE = 2.71828;
int selectionSpeed(int loopedWhileButtonHeld) {    // function returns a smaller number the longer the button is held; Limit 50
if (loopedWhileButtonHeld < 150) {   // threshold for resetting the delay amount after the button is released
  timesLoopedWhileButtonHeld++;
  }
  else {timesLoopedWhileButtonHeld = 0;}
  float coEff = 1/(1+pow(natrualE,(-(timesLoopedWhileButtonHeld-4))));
  int result = ((300)-(250*coEff))*1.1;
  timerAdjustmentValue = 1000;
  if (result < 60) {
    timerAdjustmentValue = 10000;
  }
  return result;
}


// ******************************* AlphaBits

const byte A = B01110111; 
const byte b = B01111100;
const byte C = B00111001;
const byte d = B01011110;
const byte E = B01111001;
const byte F = B01110001;
const byte G = B00111101;
const byte H = B01110110;
const byte I = B00000110;
const byte J = B00001110;

const byte L = B00111000;

const byte m0 = B01010100;
const byte m1 = B01000100;


const byte n = B01010100;
const byte o = B01011100;
const byte P = B01110011;

const byte r = B01010000;
const byte S = B01101101;
const byte t = B01111000;

const byte U = B00111110; 



const byte Y = B01101110;
const byte blank = B00000000;
// ****************************** End AlphaBits


void setup() {
  
// *********** FastLED
  
  
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  FastLED.setBrightness(BRIGHTNESS);

  randomSeed(analogRead(A14));
  chadRedorBlue = random(0, 255);
  chadHidingSpot = random(3, NUM_LEDS -3);

  randomSeed(analogRead(A13));
  taylorRedorBlue = random(0, 255);
  taylorHidingSpot = random(3, NUM_LEDS -3);



// ********* end FastLED
//  start serial port at 9600 bps:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
   delay(1300); // 3 second delay for recovery
  Serial.println("Synchronizing Chronometer of Tactical Troop Kinetics ");
  Serial.println("S.C.O.T.T.K. ");
  Serial.print("Software Version: ");
  Serial.println(currentVersion);
  
  
  Serial.println(" ");
  Serial.print("By the way Chad's color is: ");
  Serial.println(chadRedorBlue);
  Serial.print("And he is hiding at: ");
  Serial.println(chadHidingSpot);
  Serial.println(" ");



  pinMode(interruptRed, INPUT);        // Buttons are low until pushed. 
  pinMode(interruptBlue, INPUT);     
  pinMode(buzzerRelay, OUTPUT);
  pinMode(redLEDRelay, OUTPUT);
  pinMode(blueLEDRelay, OUTPUT);
  pinMode(greenLEDRelay, OUTPUT);
  pinMode(batteryIndicatorEnable, OUTPUT);
  
  digitalWrite(buzzerRelay, LOW);
  digitalWrite(redLEDRelay, LOW);
  digitalWrite(blueLEDRelay, LOW);
  digitalWrite(greenLEDRelay, LOW); 
  //digitalWrite(batteryIndicatorEnable, LOW);
Serial.println("matrix starting");
  matrix.begin(0x70);
  matrix.setBrightness(15);
Serial.println("modeSelect starting");



if (digitalRead(interruptBlue)) {
buzzerRelay = 13;  
for (int x = 0; x < 5; x++) {
confirmSetting();

}
Serial.println("Buzzer silenced ");
delay(2000);
}

if (digitalRead(interruptRed)) {

for (int x = 0; x < 5; x++) {
confirmSetting();
}
Serial.println("Mode Select option enabled. ");
delay(2000);
modeSelect();
}
else {
  mode = 8;
  modeSelected();
  }


Serial.println("Game starts in: ");
 displayClear();
 currentTime = millis();
while (loadingScreenCountDown > 1) {
 
  loadingScreen(loadingScreenCountDown);
  
  oldTime = currentTime;
  currentTime = millis();
  loopDelay = (currentTime - oldTime);
  loadingScreenCountDown = loadingScreenCountDown - loopDelay;
}
delay(100);
Serial.println(" ");
Serial.println("Display Zeros Setup ");

displayZeros();
delay(1000);

 currentTime = millis();

}

void loop() {          //************************************************************************ Main Loop Starts
  oldTime = currentTime;
  currentTime = millis();
  loopDelay = currentTime - oldTime;
  matrix.writeDisplay();
  //FastLED.show();

 switch (mode) {       // Pre-game delay (mode 0) needs to start in void setup
    case 0:
     preGame();      
     break;
    case 1:
      kingOfTheHill();  
      break;   
    case 2:
      timer();
      break;
    case 3:
      timer();
      break;  
    case 4:
      boxing_timer();
      break; 
    case 5:
      if (sollenberger) {
        reArmCountdown();
      }
      else {bomb_HardMode();}
      
      break; 
    case 6:
      stopWatch();
      break; 

    case 7:
      grifball_timer();
      break; 

    case 8:
      stopWatch();
      break; 

       
    } 
}                                                       //************************************************************************ Main Loop Ends


void modeSelect() {
    
displayClear();
allOff();
//screenPrint(S, n, blank, o, P);
while (digitalRead(interruptRed)) { // Do nothing until red is released
}
  
Serial.println("Select Game Mode ");
int modeTimeOut = 4100;
int modeTimer = 0;
int counter = 0;
currentTime = 0;
oldTime = 0;


    
 //  showOffLEDs();
   matrix.writeDigitRaw (0, B00001000); //   __:__
   matrix.writeDigitRaw (1, B00001000); //
   matrix.writeDigitRaw (2,B00000000); // 
   matrix.writeDigitRaw (3, B00001000); //
   matrix.writeDigitRaw (4, B00001000); //
   matrix.writeDisplay();
   Serial.println("modeSelect ");
   while (!digitalRead(interruptBlue) && !digitalRead(interruptRed)) { // Do nothing until either button is pushed
}


displayClear();

oldTime = millis(); 
while (modeTimer < modeTimeOut) {
mode = mode + digitalRead(interruptBlue) - digitalRead(interruptRed);
if (mode < 1) {mode = 0;}  // no negative numbers
if (mode != oldMode) {
  modeTimer = 0;
  numberToWord(mode);
  
  delay(300);
  }
 
  counter = millis() - oldTime;
  modeTimer = modeTimer + (counter);
  oldTime = millis();
oldMode = mode;
}
Serial.print("Mode selected: ");
Serial.println(mode);


confirmSetting();

delay(100);
modeSelected();
}

void modeSelected() {
 // _____________________________Switch Case for setup of the selected game mode
 

  switch (mode) {       
    case 0:
      preGameSetup();
      modeSelect();
      break;
    case 1:
      Serial.println("KotH with Pregame Delay ");
      
      secondCountDown = 0;
      modeTimer = 0;
      kingOfTheHillSetup();
      preGame();      
      attachInterrupt(digitalPinToInterrupt(interruptRed), redControl, RISING);
      attachInterrupt(digitalPinToInterrupt(interruptBlue), blueControl, RISING);
      clockOwnership = "None: ";
      allOff();
      Serial.print("Clock Ownership ending Mode Selected: ");
      Serial.println(clockOwnership);
      break;
    case 2: 
      Serial.println("Timer Setup ");
      timerSetup();
      break;
    case 3:
      Serial.println("Sniper Setup ");
      // firstCountDown = 60100;   // Variable for first countdown delaying when a game can start (1 minute given in milliseconds)
      // secondCountDown = 0;   // Second countdown. Set to zero if not needed. 
      timerSetup();           
      preGame();
      timerStart = true;
      timerCountDown = timerCountDownSetPoint;
      break;
    case 4:
      Serial.println("Interval Timer ");
      interval_timer_setup();
      break;
    case 5:
      Serial.println("Bomb Hard Mode Setup ");
      bombSetUp();
      break;       
    case 6:    
      Serial.println("Stopwatch ");
      
      break;
    case 7:    
      Serial.println("Grifball Timer ");
      grifball_timer_setup(); 
      break;    
    case 8:
      Serial.println("Pregame Delay and Stopwatch ");
      firstCountDown = 60100;   // Variable for first countdown delaying when a game can start (1 minute given in milliseconds)
      secondCountDown = 0;   // Second countdown. Set to zero if not needed. 
      preGame();  
      
      attachInterrupt(digitalPinToInterrupt(interruptRed), pauseStopWatch, RISING); 
      start = true;
      Serial.print("start = ");
      Serial.println(start);
            
      Serial.print("mode = ");
      Serial.println(mode);
      
      break;
    case 10:    
      Serial.println("Battery Life Indicator ");
      batteryIndicator();
      break;  
    case 11:    
      Serial.println("Test all outputs ");
      testAllOutputs();
      break;  
      
}
//_______________________________ end game mode setup

}


void displayZeros() {     // turns on all the LED's in the display. ( :88:88)
  Serial.println("Display Zeros ");
   matrix.writeDigitNum (0, 8); //
   matrix.writeDigitNum (1, 8); //
   matrix.writeDigitRaw (2,B00001111); // 
   matrix.writeDigitNum (3, 8); //
   matrix.writeDigitNum (4, 8); //
   matrix.writeDisplay();
   Serial.println("Display Zeros done ");
}

void displayClear() {     // turns on all the LED's in the display. ( :88:88)
  
   matrix.writeDigitRaw (0, B00000000); //
   matrix.writeDigitRaw (1, B00000000); //
   matrix.writeDigitRaw (2, B00000000); // 
   matrix.writeDigitRaw (3, B00000000); //
   matrix.writeDigitRaw (4, B00000000); //
   matrix.writeDisplay();
}
