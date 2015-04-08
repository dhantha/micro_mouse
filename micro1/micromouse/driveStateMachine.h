
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
};

// Mouse maze state
extern short mouseDir;
extern short mouseBlock;
extern short nextBlock;

extern enum EDriveState mouseDriveState;


void enterDriveState(enum EDriveState nextState);
void mouseDriveMachine();


//Debug only!!!
double maintainHeadingOffset();
double getGoalHeading();
