
struct SMazeVert
{
  unsigned char explored;
  short adjVertIds[4];

  unsigned short dist;
  short bestPred;

  short searchPred;
};

#define MAZE_SIZE (16)
extern struct SMazeVert mazeVerts[MAZE_SIZE*MAZE_SIZE];

// Mouse maze state
extern int mouseDir;
extern short mouseBlock;
extern short nextBlock;

void setupMaze();
void updateMouseDir(int incr);
void enterMazeBlock(int nextBlock);
int getAbsoluteEdgeIdx(int relativeDir);
void updateAdjacentBlockInfo();
int getNextTurn();
void findNextBlock();
