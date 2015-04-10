
struct SMazeVert
{
  unsigned char explored;
  short adjVertIds[4];

  unsigned short dist;
  short bestPred;

  short searchPred;
};

const int mazeSize = 6;
extern struct SMazeVert mazeVerts[mazeSize*mazeSize];

extern int pathLength;
extern short mazeSolvePath[mazeSize*mazeSize];

// Mouse maze state
extern short mouseDir;
extern short mouseBlock;
extern short nextBlock;

// Solver state machine
enum EMazeSolveState
{
  eSearching = 0,
  eFoundCenter = 1,
  eBacktrack = 2,
  eRerun = 3,
  eReturn = 4,
  eFinished = 5,
};

void updateMazeSolverState();

void setupMaze();
void updateMouseDir(int incr);
void enterMazeBlock(int nextBlock);
int getAbsoluteEdgeIdx(int relativeDir);
void updateAdjacentBlockInfo();
int getNextTurn();
void findNextBlock();

void debugUpdateMazeString();
void debugResetMazePrint();
char* debugMazeStringRow();
