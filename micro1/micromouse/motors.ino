#include <SoftwareSerial.h>
#include <PololuQik.h>

// Connections:
// motor RX->5
// motor TX->6
PololuQik2s9v1 qik(6, 5, 7);


double speedMult = 1.0;

const int forwardMotorSpeed = 15;
const int turnMotorSpeed = 10;

void motorSetForwardSpeed(double offset)
{
        if ( offset < -speedMult*forwardMotorSpeed*0.2 )
          offset = -speedMult*forwardMotorSpeed*0.2;
        else if ( offset > speedMult*forwardMotorSpeed*0.2 )
          offset = speedMult*forwardMotorSpeed*0.2;
            
        //Serial.println("Forward");
	qik.setM0Speed(-(speedMult*forwardMotorSpeed + offset));
	qik.setM1Speed(speedMult*forwardMotorSpeed - offset);
}

void motorSetTurnSpeed(int turnDir)
{
        //Serial.println("Turn");
	qik.setM0Speed(turnDir * turnMotorSpeed);
	qik.setM1Speed(turnDir * turnMotorSpeed);
}

void motorSetStopSpeed()
{
  //Serial.println("Stop");
	qik.setM0Speed(0);
	qik.setM1Speed(0);
}

void motorSetup()
{
	qik.init();
	motorSetStopSpeed();
        //Serial.begin(9600);
}
