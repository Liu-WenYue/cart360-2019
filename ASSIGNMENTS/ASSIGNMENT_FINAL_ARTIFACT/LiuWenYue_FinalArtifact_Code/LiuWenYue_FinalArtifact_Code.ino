  /* 
  CART 360 Project: “THIS = THEN = THAT”
  Student: Liu WenYue
  Professor: Sabine Rosenberg & Elio Bidinost
  Date: 29 November 2019 

  The source code is from https://www.instructables.com/id/Using-Mozzi-Library-with-5-potentiometers/,
  I modified the source code to control the five touch sensors of my plushie.
  
  Tim Barrass 2013 (The author of the original source code).
  This example code is in the public domain.
*/

// Libraries needed for this project.
#include <MozziGuts.h>
#include <Oscil.h> // oscillator 
#include <tables/cos2048_int8.h> // table for Oscils to play
#include <Smooth.h>
#include <AutoMap.h> // maps unpredictable inputs to a range
 

// desired carrier frequency max and min, for AutoMap
const int MIN_CARRIER_FREQ = 22;
const int MAX_CARRIER_FREQ = 440;

const int MIN = 1;
const int MAX = 10;

const int MIN_2 = 1;
const int MAX_2 = 15;

// desired intensity max and min, for AutoMap, note they're inverted for reverse dynamics
const int MIN_INTENSITY = 700;
const int MAX_INTENSITY = 10;

// desired mod speed max and min, for AutoMap, note they're inverted for reverse dynamics
const int MIN_MOD_SPEED = 10000;
const int MAX_MOD_SPEED = 1;

AutoMap kMapCarrierFreq(0,1023,MIN_CARRIER_FREQ,MAX_CARRIER_FREQ);
AutoMap kMapIntensity(0,1023,MIN_INTENSITY,MAX_INTENSITY);
AutoMap kMapModSpeed(0,1023,MIN_MOD_SPEED,MAX_MOD_SPEED);
AutoMap mapThis(0,1023,MIN,MAX);
AutoMap mapThisToo(0,1023,MIN_2,MAX_2);

Oscil<COS2048_NUM_CELLS, AUDIO_RATE> aCarrier(COS2048_DATA);
Oscil<COS2048_NUM_CELLS, AUDIO_RATE> aModulator(COS2048_DATA);
Oscil<COS2048_NUM_CELLS, CONTROL_RATE> kIntensityMod(COS2048_DATA);

// Define variables for the five touch pins.
#define TOUCH_PIN_M A2
#define TOUCH_PIN_LU A4
#define TOUCH_PIN_LL A5
#define TOUCH_PIN_RU A3
#define TOUCH_PIN_RL A1

int mod_ratio = 5; // brightness (harmonics)
long fm_intensity; // carries control info from updateControl to updateAudio

// smoothing for intensity to remove clicks on transitions
float smoothness = 0.95f;
Smooth <long> aSmoothIntensity(smoothness);


// Set up function
void setup(){
  Serial.begin(115200); // set up the Serial output so we can look at the light level
  startMozzi(); // :))
}


// Update control function
void updateControl(){ 
  // Read the input value on touch pin middle.
  int freqVal = mozziAnalogRead(TOUCH_PIN_M); // value is 0-1023
  int FRQ = mapThis(freqVal);

  // Read the input value on touch pin left lower.
  int knob2 = mozziAnalogRead(TOUCH_PIN_LL); // value is 0-1023
  int knob2Val = mapThis(knob2);
  
  // Read the input value on touch pin right lower
  int knob_value = mozziAnalogRead(TOUCH_PIN_RL); // value is 0-1023
  
  // map the knob to carrier frequency
  int carrier_freq = kMapCarrierFreq(knob_value);
  
  //calculate the modulation frequency to stay in ratio
  int mod_freq = carrier_freq * mod_ratio * FRQ;
  
  // set the FM oscillator frequencies
  aCarrier.setFreq(carrier_freq); 
  aModulator.setFreq(mod_freq);
  
  // Read the input value on touch pin left upper.
  int Ctrl1_value= mozziAnalogRead(TOUCH_PIN_LU); // value is 0-1023
  // print the value to the Serial monitor for debugging
  Serial.print("Ctrl1 = "); 
  Serial.print(Ctrl1_value);
  Serial.print("\t"); // prints a tab

  int Ctrl1_calibrated = kMapIntensity(Ctrl1_value);
  Serial.print("Ctrl1_calibrated = ");
  Serial.print(Ctrl1_calibrated);
  Serial.print("\t"); // prints a tab
  
 // calculate the fm_intensity
  fm_intensity = ((long)Ctrl1_calibrated * knob2Val * (kIntensityMod.next()+128))>>8; // shift back to range after 8 bit multiply
  Serial.print("fm_intensity = ");
  Serial.print(fm_intensity);
  Serial.print("\t"); // prints a tab
  
  // Read the input value on touch pin right upper.
  int Ctrl2_value= mozziAnalogRead(TOUCH_PIN_RU); // value is 0-1023
  Serial.print("Ctrl2 = "); 
  Serial.print(Ctrl2_value);
  Serial.print("\t"); // prints a tab
  
  // use a float here for low frequencies
  float mod_speed = (float)kMapModSpeed(Ctrl2_value)/1000;
  Serial.print("   mod_speed = ");
  Serial.print(mod_speed);
  kIntensityMod.setFreq(mod_speed);
  
  Serial.println(); // finally, print a carraige return for the next line of debugging info
}


int updateAudio(){
  long modulation = aSmoothIntensity.next(fm_intensity) * aModulator.next();
  return aCarrier.phMod(modulation);
}


void loop(){
  audioHook();
}
