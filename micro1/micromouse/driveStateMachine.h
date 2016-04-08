
// Mouse drive state machine
enum EDriveState
{
  eStopped = 0,
  
  eCenterBlock = 1,
  eLeaveBlock = 2,
  eEnterBlock = 3,
  
  eTurnLeft = 4,
  eTurnRight = 5,
  eTurnAround = 6,
  
  eDebugMazePrint = 7,
};

// Mouse maze state
extern short mouseDir;
extern short mouseBlock;
extern short nextBlock;

extern enum EDriveState mouseDriveState;

void printMazeState();

int inDriveState(enum EDriveState checkState);
void enterDriveState(enum EDriveState nextState);
void mouseDriveMachine();
void updateMazeSolverState();
int nextBlockWallFront();


//Debug only!!!
double maintainHeadingOffset();
double getGoalHeading();
