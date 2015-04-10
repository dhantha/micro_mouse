sensors.ino: 
const int frontTooCloseRange = 650;

// This is the encoder block size!!!!!
const int halfBlockEncCount = 42; // 40;

// Only if turning isn't working
const int turn90EncCount = 23;


driveStateMachine.cpp:
// THis is IR range to center!!!!!
const double centerVal = 250.0; // 255.0;


mazeModel.h:
const int mazeSize = 6;


