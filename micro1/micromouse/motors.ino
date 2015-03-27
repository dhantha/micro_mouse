#include <PololuQik.h>

PololuQik2s9v1 qik(6, 5, 7);


const int forwardMotorSpeed = -30;
const int turnMotorSpeed = 20;

void motorSetForwardSpeed(double offset)
{
        //Serial.println("Forward");
	qik.setM0Speed(-(forwardMotorSpeed + offset));
	qik.setM1Speed(forwardMotorSpeed - offset);
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
