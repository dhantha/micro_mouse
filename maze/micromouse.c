#include <stdio.h>
#include <stdarg.h>

#include <mazeModel.h>
#include <driveStateMachine.h>
#include <sensors.h>
#include <motors.h>

enum ESolveState
{
  eSearching = 0,
  eFoundCenter = 1,
  eBacktrack = 2,
  eRerun = 3,
};

enum ESolveState mouseSolveState = eSearching;


//SoftwareSerial mySerial(6, 5); //RX TX

void setup()
{
  setupMaze();


  // TODO: These need to be in drive/io emulation setup
  // Serial.begin(9600);

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

  // State machine
  mouseDriveMachine();
}
