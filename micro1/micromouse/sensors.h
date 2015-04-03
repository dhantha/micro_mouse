
// Encoder general functions
void encoderSetup();
void encoderUpdateCounts();
void encoderResetDistanceCounters();

// Encoder block-specific functions
int encoderForwardBlockFinished();
int encoderTurn90Finished();
int encoderTurn180Finished();


// IR Ranging functions
void readIRRanges();

int checkWallLeft();
int checkWallRight();
int checkWallFront();

int stopWallFront();


// Compass functions
void compassSetup();
unsigned char getCompassStatus();
int getCompassResults();
