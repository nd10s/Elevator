#pragma region VEXcode Generated Robot Configuration
// Make sure all required headers are included.
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
 
 
#include "vex.h"
 
using namespace vex;
 
// Brain should be defined by default
brain Brain;
 
 
// START V5 MACROS
#define waitUntil(condition)                                                   \
 do {                                                                         \
   wait(5, msec);                                                             \
 } while (!(condition))
 
#define repeat(iterations)                                                     \
 for (int iterator = 0; iterator < iterations; iterator++)
// END V5 MACROS
 
 
// Robot configuration code.
led firstFloorLED = led(Brain.ThreeWirePort.E);
led secondFloorLED = led(Brain.ThreeWirePort.F);
led thirdFloorLED = led(Brain.ThreeWirePort.G);
bumper secondFloorButton = bumper(Brain.ThreeWirePort.B);
bumper thirdFloorButton = bumper(Brain.ThreeWirePort.C);
limit lowerLimit = limit(Brain.ThreeWirePort.H);
bumper firstFloorButton = bumper(Brain.ThreeWirePort.A);
motor carriageMotor = motor(PORT21, ratio18_1, false);
#pragma endregion VEXcode Generated Robot Configuration
 
/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Nicholas D Thoennes                                       */
/*    Created:      1/19/22                                                   */
/*    Description:  Elevator Project                                          */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "vex.h"
 
// Allows for easier use of the VEX Library
using namespace vex;
 
/*
 things to do
   - queue for button presses
   - determind encoder counts for each floor
   - move timoutCounter into the movement function
*/
 
 
//define variables
int currentEncoderCount = 0; //initial encoder
int firstFloorEncoderCount = 100; //position of the first floor
int secondFloorEncoderCount = 1000; //second floor
int thirdFloorEncoderCount = 2500; //third floor
int nextFloor;
int currentFloor = 0;
int dir = 1; //initial direction of motion is positive because you start at floor1
int timeoutCounter = 0; //initially set the timeout counter to 0 seconds
 
//home the carriage -- this is done
void homing() {
 carriageMotor.setVelocity(10, percent);
 //i'm assuming reverse is down...
 carriageMotor.spin(reverse);
 //keep moving down until the lower limit switch is pressed
 while(true){
   if(lowerLimit.pressing()){
     //stop the motor
     carriageMotor.stop();
     //user feedback
     Brain.Screen.print("The carriage has reached the first floor.");
     //set the reference position (0)
     carriageMotor.setPosition(0, degrees);
     //we done homing :)
     break;
   }
 }
}
 
//determine which direction the motor needs to spin to get to nextFloor
int determineDirection(int currentFloor) {
 //if currently at first floor, you can only go up so return positive 1
 //if currently at second floor, check nextFloor
   //if nextFloor equals first, then return a -1 for down
   //otherwise you're going to the third floor and have to go up so positive 1
 //otherwise, you're at the third and can only go down so -1
 if(currentFloor == 1){
   dir = 1;
 }
 else if (currentFloor == 2) {
   if (nextFloor == 1){
     dir = -1;
   }
   else {
     dir = 1;
   }
 }
 else {
   dir = -1;
 }
 return dir;
}
 
//changeFloors function
void changeFloors(int nextFloor) {
 int nextFloorPos;
 //floor stuff
 if(nextFloor == 1){
   nextFloorPos = firstFloorEncoderCount;
 }
 else if(nextFloor == 2){
   nextFloorPos = secondFloorEncoderCount;
 }
 else{
   nextFloorPos = thirdFloorEncoderCount;
 }
 
 //motor stuff
 int motorDirection = determineDirection(currentFloor);
 carriageMotor.setVelocity((motorDirection*50), percent);
 carriageMotor.spinToPosition(nextFloorPos, degrees);
 currentFloor = nextFloor;
 
 //LED stuff -- make this more concise, should work for now
 if(currentFloor == 1){
   firstFloorLED.on();
   secondFloorLED.off();
   thirdFloorLED.off();
 }
 else if(currentFloor == 2){
   firstFloorLED.off();
   secondFloorLED.on();
   thirdFloorLED.off();
 }
 else{
   firstFloorLED.off();
   secondFloorLED.off();
   thirdFloorLED.on();
 }
}
 
// "when started" hat block
int whenStarted1() {
 //reset all LED states
 firstFloorLED.off();
 secondFloorLED.off();
 thirdFloorLED.off();
  //homing
 homing();
  while(true){
   //moving the thing
   //if it's NOT in motion, lock the user out. turn this into a queue at some point?
   if(firstFloorButton.pressing()){
     timeoutCounter = 0;
     Brain.Screen.clearScreen();
     Brain.Screen.setCursor(0,0);
     Brain.Screen.print("REACHED FIRST FLOOR");
     changeFloors(1);
   }
   else if(secondFloorButton.pressing()){
     timeoutCounter = 0;
     Brain.Screen.clearScreen();
     Brain.Screen.setCursor(0,0);
     Brain.Screen.print("REACHED SECOND FLOOR");
     changeFloors(2);
   }
   else if(thirdFloorButton.pressing()){
     timeoutCounter = 0;
     Brain.Screen.clearScreen();
     Brain.Screen.setCursor(0,0);
     Brain.Screen.print("REACHED THIRD FLOOR");
     changeFloors(3);
   }
   else{ //timeout counter
     wait(1, seconds);
     timeoutCounter+=1;
     if(timeoutCounter >= 10){
       timeoutCounter = 0;
       changeFloors(1);
     }
   }
 }
}
 
 
//don't need to touch this, it runs when you hit run code.
int main() {
 // post event registration
 
 // set default print color to black
 printf("\033[30m");
 
 // wait for rotation sensor to fully initialize
 wait(30, msec);
 
 whenStarted1();
}
