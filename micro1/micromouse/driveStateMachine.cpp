#include "mazeModel.h"
#include "sensors.h"
#include "motors.h"
#include "driveStateMachine.h"

// Mouse maze state
short mouseDir = 2;
short mouseBlock = 0;
short nextBlock = -1;

enum EDriveState mouseDriveState = eCenterBlock;

float mouseHeading = 0;
void resetHeading()
{
  mouseHeading = getCompassHeading();
}

double maintainHeadingOffset()
{
  const double alpha = 0.1;
  double headingDelta = (getCompassHeading() - mouseHeading);
  
  return alpha*headingDelta;
}

void enterDriveState(enum EDriveState nextState)
{
	mouseDriveState = nextState;
}

int inDriveState(enum EDriveState checkState)
{
	return (mouseDriveState == checkState);
}

// double calcForwardSpeedOffset()
// {
// 	// IR range constants for block wall distances
// 	const int centerVal = 250; // 255;

// 	// Overly simplistic control system constants
// 	// tries to maintain straight forward movement
// 	const double alphaEnc = 2.0;
// 	const double alpha = -0.04;

// 	int bWallLeft = checkWallLeft();
// 	int bWallRight = checkWallRight();
// 	int bWallFront = checkWallFront();

// 	double deltaLeft = bWallLeft * (irRangeLeft - centerVal);
// 	double deltaRight = bWallRight * (irRangeRight - centerVal);

// 	double straightDelta = ((leftEncVelocity - rightEncVelocity) - 0.5);
// 	double meanWallDelta = (deltaLeft-deltaRight);

// 	if ( !bWallLeft && !bWallRight )
// 	{
// 		meanWallDelta = 0;
// 	}
// 	else
// 	{
// 		meanWallDelta = meanWallDelta / (bWallLeft + bWallRight);
// 	}

// 	return (alpha*meanWallDelta + alphaEnc*straightDelta);
// }


void mouseDriveMachine()
{
	// double forwardOffset = calcForwardSpeedOffset();
	double forwardOffset = 0.0;

	// CenterBlock is the state where the mouse decides what to do next
	if ( inDriveState(eCenterBlock) )
	{
                resetHeading();
		encoderResetDistanceCounters();

		// Depending on solve-state
		findNextBlock();
		int nextDir = getNextTurn();

		if ( nextDir == 0 )
			enterDriveState(eLeaveBlock);
		else if ( nextDir == -1 )
			enterDriveState(eTurnLeft);
		else if ( nextDir == 1 )
			enterDriveState(eTurnRight);
		else
			enterDriveState(eTurnAround);
	}
	// Leave block drives to the edge of a block, updates current block properties on leaving the state
	else if ( inDriveState(eLeaveBlock) )
	{
		// This is an error state (try to correct for it)
		if ( stopWallFront() )
			enterDriveState(eCenterBlock);

		if ( nextBlock < 0 )
			enterDriveState(eCenterBlock);

                forwardOffset = maintainHeadingOffset();
		motorSetForwardSpeed(forwardOffset);

		if ( encoderForwardBlockFinished() )
		{
			enterDriveState(eEnterBlock);

			encoderResetDistanceCounters();
			enterMazeBlock(nextBlock);
		}
	}
	// Enter block handles driving from the "edge" to center of a block
	else if ( inDriveState(eEnterBlock) )
	{
                forwardOffset = maintainHeadingOffset();
		motorSetForwardSpeed(forwardOffset);

		if ( encoderForwardBlockFinished() )
		{
			enterDriveState(eCenterBlock);

			encoderResetDistanceCounters();
			updateAdjacentBlockInfo();
		}

		if ( stopWallFront() )
		{
			enterDriveState(eCenterBlock);

			encoderResetDistanceCounters();
			updateAdjacentBlockInfo();
		}
	}
	// Turn 180 degrees and leave the way we came
	else if ( inDriveState(eTurnAround) )
	{
		motorSetTurnSpeed(+1);

		if ( encoderTurn180Finished() )
		{
			enterDriveState(eLeaveBlock);

                        resetHeading();
			encoderResetDistanceCounters();
			updateMouseDir(-2);
		}
	}
	// Turn 90 degrees to the right to leave the block
	else if ( inDriveState(eTurnRight) )
	{
		motorSetTurnSpeed(+1);

		if ( encoderTurn90Finished() )
		{
			enterDriveState(eLeaveBlock);

                        resetHeading()
			encoderResetDistanceCounters();
			updateMouseDir(+1);
		}
	}
	// Turn 90 degrees to the left to leave the block
	else if ( inDriveState(eTurnLeft) )
	{
		motorSetTurnSpeed(-1);

		if ( encoderTurn90Finished() )
		{
			enterDriveState(eLeaveBlock);

                        resetHeading();
			encoderResetDistanceCounters();
			updateMouseDir(-1);
		}
	}
	else if ( inDriveState(eStopped) )
	{
                resetHeading();
		motorSetStopSpeed();
	}

}
