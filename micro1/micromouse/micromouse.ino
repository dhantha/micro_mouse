#include <stdio.h>
#include <stdarg.h>

#include <SoftwareSerial.h>

#include "mazeModel.h"
#include "driveStateMachine.h"
#include "sensors.h"
#include "motors.h"

double calcForwardSpeedOffset();

void setup()
{
  // DEBUG: Used for debugging output to computer
  Serial.begin(9600);
  
  motorSetup();
  encoderSetup();
  readIRRanges();
  
  setupMaze();
  

  // irRangeSetup();

  // ledSetup();
  // pinMode(49, OUTPUT);
  // pinMode(43, OUTPUT);
  // pinMode(44, OUTPUT);

  // digitalWrite(49, LOW);
  // digitalWrite(43, LOW);
  // digitalWrite(44, LOW);

  enterDriveState(eCenterBlock);
}

void loop()
{
  encoderUpdateCounts();
  readIRRanges();

  // double leftVel = getLeftEncoderVelocity();
  // double rightVel = getRightEncoderVelocity();

  // Serial.print(leftVel);
  // Serial.print(", ");
  // Serial.println(rightVel);
  
//  double headingDelta = maintainHeadingOffset();
//  double goalHeading = getGoalHeading();
//  double curHeading = getCompassHeading();
//  Serial.print(curHeading);
//  Serial.print(", ");
//  Serial.print(goalHeading);
//  Serial.print(", ");
//  Serial.println(headingDelta);
  
//  float test = getGoalHeading();
//  Serial.print("Heading: ");
//  Serial.println(test);
  
  double irLeft = getWallRangeLeft();
  double irRight = getWallRangeRight();
  double irFront = getWallRangeFront();

  // double offset = calcForwardSpeedOffset();
  // Serial.println(offset);
  
//  Serial.print("IR: ");
//  Serial.print(irLeft);
//  Serial.print(", ");
//  Serial.print(irRight);
//  Serial.print(", ");
//  Serial.println(irFront);

//  if ( inDriveState(eDebugMazePrint) )
//  {
//    Serial.println(mouseBlock);
//    Serial.println(mouseDir);
//    
//    Serial.println(nextBlock);
//    Serial.println(getNextTurn());
//    
//    debugUpdateMazeString();
//    debugResetMazePrint();
//    
//    for ( int i=0; i < 2*mazeSize+1; ++i )
//      Serial.println(debugMazeStringRow());
//  }

  // State machine
  mouseDriveMachine();
}

