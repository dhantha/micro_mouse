sensors.ino: 
const int frontTooCloseRange = 650;

// Good IR value when centered to the front wall
const int enterBlockStopRange = 530;
// Guess at whether the next block has a wall
const int enterBlockWallRange = 120;

// Comment out the if statement as below and it will ignore IR for testing the encoder block length
// if ( !bHasWallFront )
		return (meanEncCount >= halfBlockEncCount);

// This is the encoder block size!!!!!
const int halfBlockEncCount = 43; // 40;

// Only if turning isn't working
const int turn90EncCount = 22;


driveStateMachine.cpp:
// THis is left-right IR range to center!!!!!
const double centerVal = 250.0; // 255.0;


mazeModel.h:
const int mazeSize = 6;

mazeModel.cpp
// Maze rerun speed multiplier
const double rerunMult = 2.0

// Comment out lines as shown below to disable edge prioritization
for ( i=-1; i <= 1; ++i )
{
	int edgeIdx = getAbsoluteEdgeIdx(i);
	if ( mazeVerts[mouseBlock].adjVertIds[edgeIdx] < 0 )
		continue;

	int nextVert = mazeVerts[mouseBlock].adjVertIds[edgeIdx];
	if ( mazeVerts[nextVert].explored > 0 )
		continue;

	//edgeCosts[i+1] = calcBlockCost(nextVert);

	availableEdges[i+1] = nextVert;
}

// Short unrolled bubble-sort loop
//bubbleSwap(edgeCosts, edgePriority, 0, 1);
//bubbleSwap(edgeCosts, edgePriority, 1, 2);
//bubbleSwap(edgeCosts, edgePriority, 0, 1);

