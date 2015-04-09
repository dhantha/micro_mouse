#include <stdio.h>
#include <stdarg.h>

#include <SoftwareSerial.h>

#include "mazeModel.h"
#include "driveStateMachine.h"
#include "sensors.h"
#include "motors.h"

double calcForwardSpeedOffset();

enum ESolveState
{
  eSearching = 0,
  eFoundCenter = 1,
  eBacktrack = 2,
  eRerun = 3,
};

enum ESolveState mouseSolveState = eSearching;

void setup()
{
  setupMaze();

  // DEBUG: Used for debugging output to computer
  Serial.begin(9600);
  
  motorSetup();
  encoderSetup();
  

  // irRangeSetup();

  // ledSetup();
  // pinMode(49, OUTPUT);
  // pinMode(43, OUTPUT);
  // pinMode(44, OUTPUT);

  // digitalWrite(49, LOW);
  // digitalWrite(43, LOW);
  // digitalWrite(44, LOW);

  enterDriveState(eCenterBlock);
  mouseSolveState = eSearching;
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

  // State machine
  mouseDriveMachine();
}

