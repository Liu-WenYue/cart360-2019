/********************** PROTOTYPE CART 360 2019 *******************************
 * Liu WenYue - November 07 2019
 * Professor: Elio Bidinost & Sabine Rosenberg
 * This code is for the prototype of the project “THIS = THEN = THAT”.
 * 
 * In this prototype, I will be testing the plushie's musical response function.
 * When user touch/press on the stomache of the plushie, audio generates using the pressure input.
 * 
 * Library used: Mozzi Audio Synthesis Library for Arduino
 * URL for Mozzi: https://sensorium.github.io/Mozzi/
 * 
 */

#include <MozziGuts.h>
#include <Oscil.h> // oscillator
#include <tables/cos2048_int8.h> // table for Oscils to play
#include <AutoMap.h> // maps unpredictable inputs to a range

// desired carrier frequency max and min, for AutoMap
const int MIN_CARRIER_FREQ = 22;
const int MAX_CARRIER_FREQ = 440;

// desired intensity max and min, for AutoMap, note they're inverted for reverse dynamics
const int MIN_INTENSITY = 700;
const int MAX_INTENSITY = 10;

AutoMap kMapCarrierFreq(0,1023,MIN_CARRIER_FREQ,MAX_CARRIER_FREQ);
AutoMap kMapIntensity(0,1023,MIN_INTENSITY,MAX_INTENSITY);


// Define variables for the five touch pins.
#define TOUCH_PIN_M A2
#define TOUCH_PIN_LU A4
#define TOUCH_PIN_LL A5
#define TOUCH_PIN_RU A1
#define TOUCH_PIN_RL A0

//int touchpins[4] = {A4, A5, A1, A0};
//int touchpressure[4] = {0};

// Define variables for the button pins.
#define BUTTON_L 5
#define BUTTON_R 6

Oscil<COS2048_NUM_CELLS, AUDIO_RATE> aCarrier(COS2048_DATA);
Oscil<COS2048_NUM_CELLS, AUDIO_RATE> aModulator(COS2048_DATA);

int mod_ratio = 3; // harmonics
long fm_intensity; // carries control info from updateControl() to updateAudio()


void setup() {
  // Serial.begin(9600); // for Teensy 3.1, beware printout can cause glitches
  Serial.begin(115200); // set up the Serial output so we can look at the piezo values // set up the Serial output for debugging

  // Set the pin mode for the button pins.
  pinMode(BUTTON_L, INPUT);
  pinMode(BUTTON_R, INPUT);

  startMozzi(); // :))
}


void updateControl(){
//  test code for touch array
//  for (int i = 0; i < 4; i++) {
//    touchpressure[i] = mozziAnalogRead(touchpins[i]);
//    delay(5);
//    Serial.println(touchpressure[i]);
//  }
//
//  int touchValue = touchpressure[random(0, 4)];

//  test code for averaging
//  for (int i = 0; i < MeasurementsToAverage; i++) {
//    AverageTouchPressure1 += mozziAnalogRead(TOUCH_PIN_LU);
//    delay(1);
//  }
//
//  touchValue1 = AverageTouchPressure1 /= MeasurementsToAverage;

  int touchValue = mozziAnalogRead(TOUCH_PIN_RU);

  // map the knob to carrier frequency
  int carrier_freq = kMapCarrierFreq(touchValue);

  // print the value to the Serial monitor for debugging
  Serial.print("carrier_freq = ");
  Serial.print(carrier_freq);
  Serial.print("\t"); // prints a tab

  //calculate the modulation frequency to stay in ratio
  int mod_freq = carrier_freq * mod_ratio;

  // set the FM oscillator frequencies to the calculated values
  aCarrier.setFreq(carrier_freq);
  aModulator.setFreq(mod_freq);

  // read the middle_press on the Analog input pin
  int middle_press= mozziAnalogRead(TOUCH_PIN_M); // value is 0-1023

  // print the value to the Serial monitor for debugging
  Serial.print("middle_press = ");
  Serial.print(middle_press);
  Serial.print("\t"); // prints a tab

  fm_intensity = kMapIntensity(middle_press);

  Serial.print("fm_intensity = ");
  Serial.print(fm_intensity);
  Serial.println(); // print a carraige return for the next line of debugging info
}


int updateAudio(){
  long modulation = fm_intensity * aModulator.next();
  // Print the word in the Serial monitor
  Serial.print("Working!"); // updateAudio() is only called when this line is here.
  return aCarrier.phMod(modulation); // phMod does the FM
 
}


void loop() {
  audioHook();
}
