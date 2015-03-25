#include <mazeModel.h>

const int mazeSize = 16;
struct SMazeVert mazeVerts[mazeSize*mazeSize];

void setupMaze()
{
  int i = 0;
  int j = 0;
  int k = 0;

  int mazeOffsetPairs[] = {-1,0,0,-1,1,0,0,1};

  curEdgeId = 0;
  for ( j=0; j < mazeSize; ++j )
  {
    for ( i=0; i < mazeSize; ++i )
    {
      mazeVerts[i+j*mazeSize].explored = 0;
      mazeVerts[i+j*mazeSize].dist = 65535;

      mazeVerts[i+j*mazeSize].bestPred = -1;
      mazeVerts[i+j*mazeSize].searchPred = -1;

      for ( k=0; k < 4; ++k )
      {
        int iAdjVert = i+mazeOffsetPairs[2*k];
        int jAdjVert = j+mazeOffsetPairs[2*k+1];

		mazeVerts[i+j*mazeSize].adjVertIds[k] = -1;

        if ( iAdjVert < 0 || iAdjVert >= mazeSize )
          continue;

        if ( jAdjVert < 0 || jAdjVert >= mazeSize )
          continue;

      	mazeVerts[i+j*mazeSize].adjVertIds[k] = iAdjVert + jAdjVert*mazeSize;
      }
    }
  }
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
	curDist = mazeVerts[mouseBlock].dist;
	if ( curDist+1 < mazeVerts[nextBlock].dist )
	{
		mazeVerts[nextBlock].dist = curDist + 1;
		mazeVerts[nextBlock].bestPred = mouseBlock;
	}

	if ( mazeVerts[nextBlock].explored < 0 )
	{
		mazeVerts[nextBlock].explored = 1;
		mazeVerts[nextBlock].searchPred = mouseBlock;
	}

	mouseBlock = nextBlock;
	nextBlock = -1;
}

int getAbsoluteEdgeIdx(int relativeDir)
{
    int edgeIdx = mouseDir + relativeDir;

    if ( edgeIdx < 0 )
      edgeIdx += 4;

    if ( edgeIdx >= 4 )
      edgeIdx -= 4;

  return edgeIdx;
}


void setWalls()
{
	int bWallLeft = checkWallLeft();
	int bWallRight = checkWallRight();
	int bWallFront = checkWallFront();

	if ( bWallLeft )
	{
		int edgeIdx = getAbsoluteEdgeIdx(-1);
		mazeVerts[mouseBlock].adjVertIds[edgeIdx] = -1;
	}

	if ( bWallRight )
	{
		int edgeIdx = getAbsoluteEdgeIdx(+1);
		mazeVerts[mouseBlock].adjVertIds[edgeIdx] = -1;
	}

	if ( bWallFront )
	{
		int edgeIdx = getAbsoluteEdgeIdx(0);
		mazeVerts[mouseBlock].adjVertIds[edgeIdx] = -1;
	}
}

void updateAdjacentBlockInfo()
{
	setWalls();

	curDist = mazeVerts[mouseBlock].dist;

	for ( int i=0; i < 4; ++i )
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
	int nextDir = 0
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
const edgePriority[4] = {1, 0, 2, 3};
void findNextBlock()
{
	int availableEdges[4];
	for ( int i=-1; i < 1; ++i )
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

	takeEdge = edgePriority[3];
	for ( int i=0; i < 3; ++i )
	{
		if ( availableEdges[edgePriority[i]] >= 0 )
		{
			takeEdge = edgePriority[i];
			break;
		}
	}

	nextBlock = availableEdges[takeEdge];
}
