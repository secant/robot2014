#include "WPILib.h"	// WPILibrary.h
#include "Math.h"	// Math.h required for fabs function
/**
 * This is a demo program showing the use of the RobotBase class.
 * The SimpleRobot class is the base of a robot application that will automatically call your
 * Autonomous and OperatorControl methods at the right time as controlled by the switches on
 * the driver station or the field controls.
 */ 
class RobotDemo : public SimpleRobot
{
	Talon frontLeft;	//  Talon Speed Controllers
	Talon frontRight;
	Talon backLeft;
	Talon backRight;
	Victor roller;
	Joystick logitech; 	// Logitech F310 Gamepad/Controller 

public:
	RobotDemo(void):
		frontLeft(1),			// 
		frontRight(2),			//
		backLeft(3),			// 
		backRight(4),			//
		roller(5),				//
		logitech(1)				// 
		// Set motor expiration to prevent unwarranted movement if connection lost or disabled
	{
		frontLeft.SetExpiration(0.1),
		frontRight.SetExpiration(0.1),
		backLeft.SetExpiration(0.1),
		backRight.SetExpiration(0.1);
	}
	/**
	 * Drive left & right motors for 2 seconds then stop
	 */
	void Autonomous(void)
	{
	}

	/**
	 * Runs the motors with arcade steering. 
	 */
	void OperatorControl(void)
	{
		DriverStationLCD *screen = DriverStationLCD::GetInstance();
		int counter = 0;
		while (IsOperatorControl())
		{
			if(fabs(logitech.GetRawAxis(2)) > 0.2)				//left joystick, forward & back
			{
				frontLeft.Set(logitech.GetRawAxis(2) * -.65);
				backLeft.Set(logitech.GetRawAxis(2) * -.65);
			}
			else
			{
				frontLeft.Set(0);
				backLeft.Set(0);
			}
			if(fabs(logitech.GetRawAxis(4)) > 0.2)				//right joystick, forward & back
			{
				frontRight.Set(logitech.GetRawAxis(4) * .65);
				backRight.Set(logitech.GetRawAxis(4) * .65);
			}
			else
			{
				frontRight.Set(0);
				backRight.Set(0);
			}
			if(logitech.GetRawButton(6))						//RB
				roller.Set(1.0);
			else if(logitech.GetRawButton(8))					//RT
				roller.Set(-1.0);
			else
				roller.Set(0);
							screen -> PrintfLine(DriverStationLCD::kUser_Line1, "%d", counter);
							screen -> UpdateLCD();
				Wait(0.005);				// wait for a motor update time
		}
	}
	
	/**
	 * Runs during test mode
	 */
	void Test() {
		while(IsTest())
		{
		}
	}
};

START_ROBOT_CLASS(RobotDemo);

