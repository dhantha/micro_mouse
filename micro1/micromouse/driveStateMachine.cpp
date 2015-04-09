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

double getGoalHeading()
{
  return mouseHeading;
}

double maintainHeadingOffset()
{
  const double alpha = 0.0;
  double headingDelta = calcHeadingDelta(getCompassHeading(),mouseHeading);
  
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

double calcForwardSpeedOffset()
{
	// IR range constants for block wall distances
	const double failCentering = 175.0; // 
	const double centerVal = 250.0; // 255.0;

	// Overly simplistic control system constants
	// tries to maintain straight forward movement
	const double encOffset = 0.02;
	const double alphaEnc = -5.0;
	
	const double alphaWall = -0.04;

	double leftRange = getWallRangeLeft();
	double rightRange = getWallRangeRight();

	int bCenterLeft = (leftRange > failCentering);
	int bCenterRight = (rightRange > failCentering);

	double deltaLeft = bCenterLeft * (leftRange - centerVal);
	double deltaRight = bCenterRight * (rightRange - centerVal);

	double straightDelta = ((getLeftEncoderVelocity() - getRightEncoderVelocity()) + encOffset);
	double meanWallDelta = (deltaLeft-deltaRight);

	if ( bCenterLeft + bCenterRight > 0 )
	{
		meanWallDelta = meanWallDelta / (bCenterLeft + bCenterRight);
	}
        else
		meanWallDelta = 0.0;

	return (alphaWall*meanWallDelta + alphaEnc*straightDelta);
}


void mouseDriveMachine()
{
	double forwardOffset = calcForwardSpeedOffset();
	//double forwardOffset = 0.0;

	// CenterBlock is the state where the mouse decides what to do next
	if ( inDriveState(eCenterBlock) )
	{
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

			encoderResetDistanceCounters();
			updateMouseDir(-1);
		}
	}
	else if ( inDriveState(eStopped) )
	{
		motorSetStopSpeed();
	}

}
