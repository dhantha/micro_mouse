#include <stdio.h>
#include <stdarg.h>

#include <SoftwareSerial.h>

#include "mazeModel.h"
#include "driveStateMachine.h"
#include "sensors.h"
#include "motors.h"

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
  compassSetup();

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
  
//  int test = getCompassResults();
//  Serial.print("DataX: ");
//  Serial.println(test);

  // State machine
  mouseDriveMachine();
}

