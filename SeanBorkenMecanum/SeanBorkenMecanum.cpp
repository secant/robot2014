#include "WPILib.h"
#include "Math.h"
class RobotDemo : public SimpleRobot
{
	Joystick joyStick;
	Joystick joyStickTwo;
	Talon topLeft, topRight;
	Talon bottomLeft, bottomRight;
public:
	RobotDemo():
		joyStick(1),
		joyStickTwo(2),
		topLeft(1), topRight(2), bottomLeft(3), bottomRight(4)
	{
	}
	
	void driveForward()
	{
		topLeft.Set(1);
		topRight.Set(1);
		bottomLeft.Set(1);
		bottomRight.Set(1);
	}
	void turnLeft()
	{
		topLeft.Set(1);
		topRight.Set();
		bottomRight.Set(1);
	}
	void moveForwardLeft()
	{
		topLeft.Set(1);
		topRight.Set(1);
		bottomLeft.Set(-1);
		bottomRight.Set(1);
	}
	void moveForwardRight()
	{
		topLeft.Set(1);
		topRight.Set(1);
		bottomLeft.Set(1);
		bottomRight.Set(-1);
	}
	void moveRight()
	{
		topLeft.Set(0);
		topRight.Set(1);
		bottomLeft.Set(0);
		bottomRight.Set(1);
	}
	void moveLeft()
	{
		topLeft.Set(1);
		topRight.Set(0);
		bottomLeft.Set(1);
		bottomRight.Set(0);
	}
	void Autonomous()
	{
	}
	void OperatorControl()
	{
		while (IsOperatorControl())
		{
			MechanumDrive(x,y,rot);
			
		}
	}
	void Test() {

	}
};

START_ROBOT_CLASS(RobotDemo);

