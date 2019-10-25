// Etude 4 - Controllers for URDL
// URL for UDRL: https://itch.io/queue/c/632428/altctrlworkshop-clab?game_id=166720
//
// 22th October 2019
// Cassie & WenYue

#include <Keyboard.h>

int photoSensorPin = A0;
int rotationSensorPin = A1;
// Values for photosensor and rotation sensor
int pValue = 0;
int rValue = 0;
// Threshold for photo sensor
int pThreshold = 400;
// Controls for rotation sensor
int prevRValue;
int interval = 200;
// Photosensor input controls arrow key direction
// of rotation sensor
bool upDown = false;

void setup() {
  Keyboard.begin();
  prevRValue = analogRead(rotationSensorPin);                                                            
}

void loop() {
  // If photo sensor is covered, rotation sensor
  // controls up/down arrow keys
  pValue = analogRead(photoSensorPin);
  if (pValue < pThreshold) {
    upDown = true;                                                          
  }    
  // If photo sensor uncovered, rotation sensor
  // controls left/right arrow keys                                                                                                                                                                                          
  else {
    upDown = false;                                                                                                                                        
  }

  rValue = analogRead(rotationSensorPin);
  // If rotation sensor controlling up down arrow keys
  if (upDown) {
    // Upward change in rotation sensor value presses UP arrow
    if (rValue > prevRValue + interval) {
      Serial.println("Up");
      Keyboard.press(218);                                                                                                                                         
      prevRValue = rValue;
      Keyboard.releaseAll();
    }
    // Downward change in rotation sensor value presses DOWN arrow                                                                                                                                                                                                                                                           
    else if (rValue < prevRValue - interval) {
      Serial.println("Down");
      Keyboard.press(217);
      prevRValue = rValue;                                 
      Keyboard.releaseAll();
    }   
    else {
      Keyboard.releaseAll();
    }
    
  } 
  // If rotation sensor controlling left/right arrow keys
  else if (!upDown) {
    // Upward change in rotation sensor value presses RIGHT arrow   
    if (rValue > prevRValue + interval) {
      Serial.println("Right");
      Keyboard.press(215);                                                                                                                                         
      prevRValue = rValue;
      Keyboard.releaseAll();
    }    
    // Downward change in rotation sensor value presses LEFT arrow                                                                                                                                                                                                                                                        
    else if (rValue < prevRValue - interval) {
      Serial.println("Left");
      Keyboard.press(216);
      prevRValue = rValue;                                 
      Keyboard.releaseAll();
    }   
    else {
      Keyboard.releaseAll();
    }
    
  }

  delay(50);
}
