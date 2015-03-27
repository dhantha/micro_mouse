
struct SMazeVert
{
  unsigned char explored;
  short adjVertIds[4];

  unsigned short dist;
  short bestPred;

  short searchPred;
};

const int mazeSize = 16;
extern struct SMazeVert mazeVerts[mazeSize*mazeSize];

// Mouse maze state
extern short mouseDir;
extern short mouseBlock;
extern short nextBlock;

void setupMaze();
void updateMouseDir(int incr);
void enterMazeBlock(int nextBlock);
int getAbsoluteEdgeIdx(int relativeDir);
void updateAdjacentBlockInfo();
int getNextTurn();
void findNextBlock();
