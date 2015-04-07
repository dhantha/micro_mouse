#include <Wire.h>
#include <SoftwareSerial.h>
#include <LSM303.h>

const int velocityCalcTicks = 50;

// Encoder GPIO pin IDs
const int encoderPinLeftA = 26;
const int encoderPinLeftB = 27;
const int encoderPinRightA = 28;
const int encoderPinRightB = 29;

// Compass Constants
const int compassI2CAddr = 0x1E;
const int compassRegConfigA = 0x00;
const int compassRegConfigB = 0x01;
const int compassRegMode = 0x02;
const int compassRegDataX = 0x03;
const int compassRegDataZ = 0x05;
const int compassRegDataY = 0x07;
const int compassRegStatus = 0x09;
const int compassRegIDA = 0x0A;
const int compassRegIDB = 0x0B;
const int compassRegIDC = 0x0C;

// Edge detection values
int prevLeftEncoder = 0;
int prevRightEncoder = 0;

// Encoder counters for distance calculations
int leftEncCount = 0;
int rightEncCount = 0;

// Velocity in encoder ticks between update calls
// TODO: Should really do this using an actual timing element
int updateTicks = 0;

int leftEncVelCount = 0;
int rightEncVelCount = 0;

double leftEncVelocity = 0.0;
double rightEncVelocity = 0.0;

// compass variables
LSM303 compass;

// Read raw encoder hi/lo values and combine to get raw double-res value
int encoderReadRaw(int pinA, int pinB)
{
	// Direct encoder readouts (high/low)
	int encLineA = digitalRead(pinA);
	int encLineB = digitalRead(pinB);

	// Combine the encoder lines to double the resolution (rate) of encoder counts
	return ( !(encLineA && encLineB) && ( encLineA || encLineB ) );
}

void encoderSetup()
{
	pinMode(encoderPinLeftA, INPUT);
	pinMode(encoderPinLeftB, INPUT);

	pinMode(encoderPinRightA, INPUT);
	pinMode(encoderPinRightB, INPUT);

	prevLeftEncoder = encoderReadRaw(encoderPinLeftA, encoderPinLeftB);
	prevRightEncoder = encoderReadRaw(encoderPinRightA, encoderPinRightB);

	leftEncVelCount = 0;
	rightEncVelCount = 0;

	leftEncVelocity = 0.0;
	rightEncVelocity = 0.0;
}

void compassSetup()
{
  /*
  int ret;
  
  Wire.beginTransmission(compassI2CAddr);
  Wire.write(0);
  ret = Wire.endTransmission(false);
  
  ret = Wire.requestFrom(compassI2CAddr, 1, 1);
  unsigned char IDA = Wire.read();
  
  Serial.println(IDA);
  
  Wire.beginTransmission(compassI2CAddr);
  Wire.write(compassRegIDB);
  ret = Wire.endTransmission(false);
  
  ret = Wire.requestFrom(compassI2CAddr, 1, 1);
  unsigned char IDB = Wire.read();
  
  Serial.println(IDB);
  
  Wire.beginTransmission(compassI2CAddr);
  Wire.write(compassRegIDC);
  ret = Wire.endTransmission(false);
  
  ret = Wire.requestFrom(compassI2CAddr, 1, 1);
  unsigned char IDC = Wire.read();
  
  Serial.println(IDC);
  */
  Wire.begin();
  compass.init();
  compass.enableDefault();  
  
//  // 15Hz sample rate, 8 averaged samples
//  Wire.beginTransmission(compassI2CAddr);
//  Wire.write(compassRegConfigA);
//  Wire.write(0x70);
//  ret = Wire.endTransmission(false);
//  
//  // Gain set to 5. 
//  Wire.beginTransmission(compassI2CAddr);
//  Wire.write(compassRegConfigB);
//  Wire.write(0xA0);
//  ret = Wire.endTransmission(false);
//  
//  // Continuous read mode
//  Wire.beginTransmission(compassI2CAddr);
//  Wire.write(compassRegMode);
//  Wire.write(0x00);
//  ret = Wire.endTransmission(false);
//  
//  // Read status byte
//  unsigned char status = 0;
//  while ( !(status & 0x01) )
//  {
//    Wire.beginTransmission(compassI2CAddr);
//    Wire.write(compassRegStatus);
//    ret = Wire.endTransmission(false);
//    ret = Wire.requestFrom(compassI2CAddr, 1, 1);
//    status = Wire.read();
//    Serial.println(status);
//  }
}

//unsigned char getCompassStatus()
//{
//  int ret;
//  
//  Wire.beginTransmission(compassI2CAddr);
//  Wire.write(compassRegStatus);
//  ret = Wire.endTransmission(false);
//  
//  ret = Wire.requestFrom(compassI2CAddr, 1, 1);
//  unsigned char status = Wire.read();
//  
//  return status;
//}

void readCompassHeading()
{
  compass.read();
}

float getCompassHeading()
{
  return compass.heading();
}


void encoderUpdateCounts()
{
	// Combine the encoder lines to double the resolution (rate) of encoder counts
	int leftEncoder = encoderReadRaw(encoderPinLeftA, encoderPinLeftB);
	int rightEncoder = encoderReadRaw(encoderPinRightA, encoderPinRightB);


	// Detect encoder edges +/- to count ticks for distance
	if ( leftEncoder != prevLeftEncoder )
	{
		leftEncCount += 1;
		leftEncVelCount += 1;
	}

	if ( rightEncoder != prevRightEncoder )
	{
		rightEncCount += 1;
		rightEncVelCount += 1;
	}

	// Use averaged tick count over #updates (loops) to calculate velocity
	// TODO: Fix this!
	if ( updateTicks >= velocityCalcTicks )
	{
		leftEncVelocity = ((double) leftEncVelCount) / updateTicks;
		rightEncVelocity = ((double) rightEncVelCount) / updateTicks;

		updateTicks = 0;
		leftEncVelCount = 0;
		rightEncVelCount = 0;
	}
	else
	{
		updateTicks += 1;
	}


	prevLeftEncoder = leftEncoder;
	prevRightEncoder = rightEncoder;
}

void encoderResetDistanceCounters()
{
	leftEncCount = 0;
	rightEncCount = 0;
}


// Encoder count constants for block size/rotation
const int halfBlockEncCount = 45; // 40;
const int turn90EncCount = 20;

int encoderForwardBlockFinished()
{
	int meanEncCount = (leftEncCount + rightEncCount) / 2;

	return (meanEncCount >= halfBlockEncCount);
}

int encoderTurn90Finished()
{
	return ((leftEncCount >= turn90EncCount) && (rightEncCount >= turn90EncCount));
}

int encoderTurn180Finished()
{
	return ((leftEncCount >= 2*(turn90EncCount + 2)) && (rightEncCount >= 2*(turn90EncCount + 2)));
}



// IR Range pins
const int irPinFront = 8;
const int irPinLeft = 9;
const int irPinRight = 10;

// IR globals
int irRangeFront;
int irRangeLeft;
int irRangeRight;

const int sideWallRange = 150;
const int frontWallRange = 200;
const int frontTooCloseRange = 430;

void readIRRanges()
{
	irRangeFront = analogRead(irPinFront);
	irRangeLeft = analogRead(irPinLeft);
	irRangeRight = analogRead(irPinRight);
}

double getWallRangeLeft()
{
  return (double) irRangeLeft;
}

double getWallRangeRight()
{
  return (double) irRangeRight;
}

double getWallRangeFront()
{
  return (double) irRangeFront;
}

int checkWallLeft()
{
	return (irRangeLeft > sideWallRange);
}

int checkWallRight()
{
	return (irRangeRight > sideWallRange);
}

int checkWallFront()
{
	return (irRangeFront > frontWallRange);
}

int stopWallFront()
{
	return (irRangeFront > frontTooCloseRange);
}
