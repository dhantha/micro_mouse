#include "mazeModel.h"
#include "sensors.h"

#include "driveStateMachine.h"

struct SMazeVert mazeVerts[mazeSize*mazeSize];

int bMouseOriented = 0;


void setWalls();

void resetMazeVert(int iVert, int jVert)
{
	const int mazeOffsetPairs[] = {-1,0, 0,-1, 1,0, 0,1};

	mazeVerts[iVert+jVert*mazeSize].explored = 0;
	mazeVerts[iVert+jVert*mazeSize].dist = 65535;

	mazeVerts[iVert+jVert*mazeSize].bestPred = -1;
	mazeVerts[iVert+jVert*mazeSize].searchPred = -1;

	for ( int k=0; k < 4; ++k )
	{
		int iAdjVert = iVert+mazeOffsetPairs[2*k];
		int jAdjVert = jVert+mazeOffsetPairs[2*k+1];

		mazeVerts[iVert+jVert*mazeSize].adjVertIds[k] = -1;

		if ( iAdjVert < 0 || iAdjVert >= mazeSize )
			continue;

		if ( jAdjVert < 0 || jAdjVert >= mazeSize )
			continue;

		mazeVerts[iVert+jVert*mazeSize].adjVertIds[k] = iAdjVert + jAdjVert*mazeSize;
	}
}

void setupMaze()
{
  int i = 0;
  int j = 0;

  int curEdgeId = 0;
  for ( j=0; j < mazeSize; ++j )
  {
    for ( i=0; i < mazeSize; ++i )
    {
    	resetMazeVert(i,j);
    }
  }

  setWalls();

  // Specialized setup for starting node.
  mazeVerts[0].explored = 1;
  mazeVerts[0].dist = 0;
  mazeVerts[0].adjVertIds[0] = -1;
  mazeVerts[0].adjVertIds[1] = -1;

  // Mouse doesn't actually know what direction it's pointing
  bMouseOriented = 0;
}

//TODO: These updates all belong somewhere else
void updateMouseDir(int incr)
{
	mouseDir += incr;
	if ( mouseDir >= 4 )
		mouseDir -= 4;

	if ( mouseDir < 0 )
		mouseDir += 4;
}

void enterMazeBlock(int nextBlock)
{
	int curDist = mazeVerts[mouseBlock].dist;
	if ( curDist+1 < mazeVerts[nextBlock].dist )
	{
		mazeVerts[nextBlock].dist = curDist + 1;
		mazeVerts[nextBlock].bestPred = mouseBlock;
	}

	if ( !mazeVerts[nextBlock].explored )
	{
		mazeVerts[nextBlock].explored = 1;
		mazeVerts[nextBlock].searchPred = mouseBlock;
	}

	mouseBlock = nextBlock;
	nextBlock = -1;
}

int offsetEdgeIdx(int edgeIdx, int offset)
{
	int offsetIdx = edgeIdx + offset;

	if ( offsetIdx < 0 )
		offsetIdx += 4;

	if ( offsetIdx >= 4 )
		offsetIdx -= 4;

	return offsetIdx;
}

int getAbsoluteEdgeIdx(int relativeDir)
{
	return offsetEdgeIdx(mouseDir, relativeDir);
}


// Correct maze in case assumed direction was wrong.
void fixupOrientation()
{
	// Position is transposed
	int jMouse = mouseBlock % mazeSize;
	int iMouse = mouseBlock / mazeSize;

	int actualMouseBlock = iMouse + mazeSize*jMouse;

	for ( int j=0; j <= jMouse; ++j )
	{
		// Haven't actually explored these nodes:
		resetMazeVert(j,iMouse);
		resetMazeVert(j,iMouse+1);

		// Fixup this node, guaranteed to have walls in all but the 
		int curVert = iMouse + j*mazeSize;
		int prevVert = mazeVerts[iMouse + j*mazeSize].adjVertIds[1];

		mazeVerts[curVert].explored = 1;
		mazeVerts[curVert].dist = j;
		mazeVerts[curVert].bestPred = prevVert;
		mazeVerts[curVert].searchPred = prevVert;

                if ( j < jMouse )
                {
                      // East side must have a wall
        		int eastVert = mazeVerts[curVert].adjVertIds[2];
        		mazeVerts[curVert].adjVertIds[2] = -1;
        		mazeVerts[eastVert].adjVertIds[0] = -1;
                }
	}

	mouseDir = 3;
	mouseBlock = actualMouseBlock;
}

void setVertWall(int vertIdx, int edgeIdx)
{
	int opWallVert = mazeVerts[vertIdx].adjVertIds[edgeIdx];

	// Wall has already been set.
	if ( opWallVert < 0 )
		return;

	int opEdgeIdx = offsetEdgeIdx(edgeIdx,2);

	mazeVerts[vertIdx].adjVertIds[edgeIdx] = -1;
	mazeVerts[opWallVert].adjVertIds[opEdgeIdx] = -1;
}


void setWalls()
{
	int bWallLeft = checkWallLeft();
	int bWallRight = checkWallRight();
	int bWallFront = checkWallFront();

	if ( bWallLeft )
	{
		int edgeIdx = getAbsoluteEdgeIdx(-1);
		setVertWall(mouseBlock, edgeIdx);
	}

	if ( bWallRight )
	{
		int edgeIdx = getAbsoluteEdgeIdx(+1);
		setVertWall(mouseBlock, edgeIdx);
	}

	if ( bWallFront )
	{
		int edgeIdx = getAbsoluteEdgeIdx(0);
		setVertWall(mouseBlock, edgeIdx);
	}

	if ( !bMouseOriented )
	{
		if ( !bWallRight )
			bMouseOriented = 1;

		if ( !bWallLeft )
		{
			fixupOrientation();
			bMouseOriented = 1;

			// We changed orientation, so reset current walls
			setWalls();
		}
	}
}

void updateAdjacentBlockInfo()
{
	int i = 0;

	setWalls();

	int curDist = mazeVerts[mouseBlock].dist;

	for ( i=0; i < 4; ++i )
	{
		if ( mazeVerts[mouseBlock].adjVertIds[i] < 0 )
			continue;

		int adjVert = mazeVerts[mouseBlock].adjVertIds[i];
		if ( curDist + 1 < mazeVerts[adjVert].dist )
		{
			mazeVerts[adjVert].dist = curDist + 1;
			mazeVerts[adjVert].bestPred = mouseBlock;
		}
	}
}

int getNextTurn()
{ 
	int nextDir = 0;
	for ( nextDir=0; nextDir < 4; ++nextDir )
	{
		if ( nextBlock == mazeVerts[mouseBlock].adjVertIds[nextDir] )
			break;
	}

	int deltaDir = nextDir - mouseDir;
	if ( deltaDir < -1 )
		deltaDir += 4;

	if ( deltaDir > 2 )
		deltaDir -= 4;

	return deltaDir;
}

// Prioritized order (straight,left,right,backtrack)
const int edgePriority[4] = {1, 0, 2, 3};
void findNextBlock()
{ 
        int i = 0;
	int availableEdges[4] = {-1,-1,-1,-1};
	for ( i=-1; i <= 1; ++i )
	{
		int edgeIdx = getAbsoluteEdgeIdx(i);
		if ( mazeVerts[mouseBlock].adjVertIds[edgeIdx] < 0 )
			continue;

		int nextVert = mazeVerts[mouseBlock].adjVertIds[edgeIdx];
		if ( mazeVerts[nextVert].explored > 0 )
			continue;

		availableEdges[i+1] = nextVert;
	}

	availableEdges[3] = mazeVerts[mouseBlock].searchPred;

	int takeEdge = edgePriority[3];
	for ( i=0; i < 3; ++i )
	{
		if ( availableEdges[edgePriority[i]] >= 0 )
		{
			takeEdge = edgePriority[i];
			break;
		}
	}

	nextBlock = availableEdges[takeEdge];
}

const int lenX = 4*mazeSize;
const int lenY = 2*mazeSize;

const int padX = 2;
const int padY = 1;

char topRow[lenX+padX];
char mazeStr[lenY*(lenX+padX)];

void debugUpdateMazeString()
{
	for ( int i=0; i < mazeSize; ++i )
	{
		for ( int k=0; k < 4; ++k )
			topRow[4*i+k] = '_';

		topRow[lenX] = ' ';
		topRow[lenX+1] = '\0';
	}

	for ( int j=0; j < mazeSize; ++j )
	{
		for ( int i=0; i < mazeSize; ++i )
		{
			mazeStr[(2*j)*(lenX+padX)+4*i] = '|';
			mazeStr[(2*j+1)*(lenX+padX)+4*i] = '|';

			mazeStr[(2*j)*(lenX+padX)+4*i+1] = ' ';
			mazeStr[(2*j+1)*(lenX+padX)+4*i+1] = '_';
			
			mazeStr[(2*j)*(lenX+padX)+4*i+2] = '?';
			mazeStr[(2*j+1)*(lenX+padX)+4*i+2] = '_';
			
			mazeStr[(2*j)*(lenX+padX)+4*i+3] = ' ';
			mazeStr[(2*j+1)*(lenX+padX)+4*i+3] = '_';
		}
		mazeStr[(2*j)*(lenX+padX)+lenX] = '|';
		mazeStr[(2*j+1)*(lenX+padX)+lenX] = '|';

		mazeStr[(2*j)*(lenX+padX)+lenX+1] = '\0';
		mazeStr[(2*j+1)*(lenX+padX)+lenX+1] = '\0';
	}

	for ( int j=0; j < mazeSize; ++j )
	{
		for ( int i=0; i < mazeSize; ++i )
		{
			int chkNode = i+j*mazeSize;
			if ( !mazeVerts[chkNode].explored )
				continue;

			mazeStr[(2*j)*(lenX+padX)+4*i+2] = '*';

			if ( mazeVerts[chkNode].adjVertIds[0] >= 0 )
			{
				mazeStr[(2*j)*(lenX+padX)+4*i] = ' ';
				mazeStr[(2*j+1)*(lenX+padX)+4*i] = ' ';
			}

			if ( mazeVerts[chkNode].adjVertIds[1] >= 0 )
			{
				mazeStr[(2*(j-1)+1)*(lenX+padX)+4*i+1] = ' ';
				mazeStr[(2*(j-1)+1)*(lenX+padX)+4*i+2] = ' ';
				mazeStr[(2*(j-1)+1)*(lenX+padX)+4*i+3] = ' ';
			}

			if ( mazeVerts[chkNode].adjVertIds[2] >= 0 )
			{
				mazeStr[(2*j)*(lenX+padX)+4*(i+1)] = ' ';
				mazeStr[(2*j+1)*(lenX+padX)+4*(i+1)] = ' ';
			}

			if ( mazeVerts[chkNode].adjVertIds[3] >= 0 )
			{
				mazeStr[(2*j+1)*(lenX+padX)+4*i+1] = ' ';
				mazeStr[(2*j+1)*(lenX+padX)+4*i+2] = ' ';
				mazeStr[(2*j+1)*(lenX+padX)+4*i+3] = ' ';
			}

			if ( chkNode == mouseBlock )
				mazeStr[(2*j)*(lenX+padX)+4*i+2] = '^';
		}
	}
}

int debugMazePrintRow = -1;
void debugResetMazePrint()
{
	debugMazePrintRow = -1;
}

char* debugMazeStringRow()
{
	if ( debugMazePrintRow < 0 )
	{
		++debugMazePrintRow;
		return topRow;
	}

	if ( debugMazePrintRow > lenY )
		return ((char*) 0);

	++debugMazePrintRow;
	return &(mazeStr[(debugMazePrintRow-1)*(lenX+padX)]);
}
