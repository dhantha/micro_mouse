
// Encoder general functions
void encoderSetup();
void encoderUpdateCounts();
void encoderResetDistanceCounters();

double getLeftEncoderVelocity();
double getRightEncoderVelocity();

// Encoder block-specific functions
int encoderForwardBlockFinished();
int encoderTurn90Finished();
int encoderTurn180Finished();


// IR Ranging functions
void readIRRanges();

double getWallRangeLeft();
double getWallRangeRight();
double getWallRangeFront();

int checkWallLeft();
int checkWallRight();
int checkWallFront();

int stopWallFront();


// Compass functions
void compassSetup();
void readCompassHeading();
float getCompassHeading();
double calcHeadingDelta(double headingA, double headingB);
