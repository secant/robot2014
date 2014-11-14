// Last Edited by Sean Wallace on 3/26/14 @ 2:19 pm
// This is the last resort final code if we do not get time to test autonomous.
// ***** CHECKLIST ***** //
/***
 * Precursory: Find the ideal distance that the robot can accurately score from
 * 1. Begin in in front of a goal. Use 

 */

#include "WPILib.h" // Use the WPI Library
#include "Math.h" // Absolute Values Enabled
class RobotDemo : public SimpleRobot //all electrical components declared here
{
	const static float driveSpeedMultiplier = 0.5; 		// Sets the max power of the drive
	const static float driveStickBuffer = 0.25; 		// Sets the threshold for actually moving the drive
	const static float autonomousSlowDriveSpeedMultiplier = 0.5;
	const static float justIncaseMyPolarityDunGoofed = -1;
	Joystick logitech; 									// Logitech Gamepad (Joystick)
	Talon leftFront, leftBack, rightFront, rightBack; 	// Drive Motors
	Talon retrievalMotor; 								// Retrieval Motor attached to the wheels located at the top of the retreival arm
	Talon winchMotor; 									// Winch Motor attached to the Gearbox
	Compressor compressor; 								// Pneumatics Compressor for the Pneumatics system
	DoubleSolenoid leftArmSolenoid; 					// Left Retrevial Pneumatic Arm
	DoubleSolenoid rightArmSolenoid; 					// Right Retrevial Pneumatic Arm
	DoubleSolenoid dogSolenoid; 						// Disengaged first to shoot
	DoubleSolenoid ratchetSolenoid; 					// Disengaged second to shoot
	DigitalInput leftLimitSwitch; 						// Left button to stop the catapult
	DigitalInput rightLimitSwitch; 						// Right button to stop the catapult
	DigitalInput leftPositionSwitch;					// Starting position in autonomous
	DigitalInput rightPositionSwitch;					//////////////////////////////////
	Relay ringLight;									// Camera light
	AxisCamera &camera;									// Camera
public: //all ports declared here
	RobotDemo():
		logitech(1), // Joystick Port [Laptop USB]
		leftFront(1), leftBack(3), rightFront(2), rightBack(4), // Tank Drive Ports [PWMOUT]
		retrievalMotor(6), winchMotor(5), // Retrieval and Catapult Winch Motor Ports [PWMOUT]
		compressor(5,2), // Compressor connection ports [Relay, Digital IO]
		leftArmSolenoid(5,6), // Left Arm Solenoid ports [Solenoid Breakout, Solenoid Breakout]
		rightArmSolenoid(7,8), //  Arm Solenoid ports [Solenoid Breakout, Solenoid Breakout]
		dogSolenoid(1,2), // Dog Solenoid ports [Solenoid Breakout, Solenoid Breakout]
		ratchetSolenoid(3,4), // Ratchet Solenoid ports [Solenoid Breakout, Solenoid Breakout]
		leftLimitSwitch(8), rightLimitSwitch(9), // Catapult Limit Switch ports [Digital IO]
		/****** AUTONOMOUS ******/
		leftPositionSwitch(6), // Left Autonomous Switch [Digital IO]
		rightPositionSwitch(7), // Right Autonomous Switch [Digital IO]
		ringLight(1, Relay::kBothDirections), // Ringlight [Relay]
		camera(AxisCamera::GetInstance("10.28.53.11")) //IP address of the camera
		{
			//Expirations can be put here. WindRiver gets mad if you remove these braces.
			camera.WriteResolution(AxisCamera::kResolution_320x240); // sets the camera's resolution
			rightFront.SetExpiration(0.1);
			rightBack.SetExpiration(0.1);
			leftFront.SetExpiration(0.1);
			leftBack.SetExpiration(0.1);
			winchMotor.SetExpiration(0.1);
			retrievalMotor.SetExpiration(0.1);
		}
	/****** AUTO FUNCTIONS START *******/
	/**** Yo yo yo the right side motors are inverted, so a positive left motor moves forward and a positive right motor moves back ****/
	void stopDriving() // Stops drive motors
	{
		leftFront.Set(0);
		leftBack.Set(0);
		rightFront.Set(0);
		rightBack.Set(0);
	}
	void driveForward() //Drives forward
	{
		leftFront.Set(1); // Probably needs to be positive
		leftBack.Set(1); // Probably needs to be positive
		rightFront.Set(-1); // Probably needs to be negative
		rightBack.Set(-1); // Probably needs to be negative
	}

	void autonomousCatapultRelease()
	{
		if ((leftLimitSwitch.Get()== 0 || rightLimitSwitch.Get()== 0) && winchMotor.Get() == 0)
		{
			stopDriving(); // Stops the drive so the robot doesn't flip on itself or something
			winchMotor.Set(0); // Redundant line for extra safety that can be removed after testing (The winch should already be off)
			dogSolenoid.Set(DoubleSolenoid::kReverse); // Brings the pneumatic piston backward to disengage the dog gear
			Wait(0.2); // Giving the pistons time to disengage properly
			ratchetSolenoid.Set(DoubleSolenoid::kForward); // Brings the pneumatic piston backward to disengage the ratchet
			Wait(5); // Waits 5 seconds after shooting before starting to load the catapult
		}
	}
	/****** AUTO FUNCTIONS END *******/
	void Autonomous() //YO BRING IT UP IF ITS DOWN
	{
		/**
		compressor.Start(); //starts compressor class
		rightArmSolenoid.Set(DoubleSolenoid::kReverse); //brings the arms down
		leftArmSolenoid.Set(DoubleSolenoid::kReverse);  //brings the arms down
		retrievalMotor.Set(-1);
		if (leftLimitSwitch.Get() == 1 && rightLimitSwitch.Get() == 1)
		{
			winchMotor.Set(0.1); // Gears need to be moving slowly to allow the dog gear to engage properly
			dogSolenoid.Set(DoubleSolenoid::kReverse); // Pushes the pneumatic piston forward to engage the dog gear
			Wait(0.2); // Giving the pistons time to engage properly
			winchMotor.Set(0); // Now that the dog gear is engaged, the gears do not have to move
			ratchetSolenoid.Set(DoubleSolenoid::kForward); // Pushes the pneumatic piston forward to engage the ratchet
			Wait(0.2); // Giving the pistons time to engage properly
		}
		while (leftLimitSwitch.Get() == 1 && rightLimitSwitch.Get() == 1) // If Limit Switch Buttons are not pressed
			{
			winchMotor.Set(1); //Now starts the winch motor to load the catapult
			}
		// If the Catapult Left &  Limit Switches are (0,0), (0,1), (1,0)
		{
			winchMotor.Set(0); // Stops the Winch Motor since one or more buttons are pressed
			if ((dogSolenoid.Get() == DoubleSolenoid::kForward) && (ratchetSolenoid.Get() == DoubleSolenoid::kForward)) // If the Dog Gear is disengaged but the ratchet is engaged
				{
					winchMotor.Set(0.05); // Gears need to be moving slowly to allow the dog gear to engage properly. Might want to test this since the catapult's already loaded.
					dogSolenoid.Set(DoubleSolenoid::kReverse); // Engages the dog gear so both dog gear and ratchet are engaged before shooting for safety
					Wait(0.1); // Giving the pistons time to engage properly
					winchMotor.Set(0); // Now that the dog gear is engaged, the gears do not have to move
				}
			else if ((dogSolenoid.Get() == DoubleSolenoid::kReverse) && (ratchetSolenoid.Get() == DoubleSolenoid::kReverse)) // If the dog gear is engaged but the ratchet is disengaged
				{
					ratchetSolenoid.Set(DoubleSolenoid::kForward); // Engages the ratchet so that both dog gear and ratchet are engaged before shooting for safety
					Wait(0.1); // Giving the pistons time to engage properly
				}
		}
		**/
		/*** DONE LOADING THE CATAPULT ***/

			//ringLight.Set(Relay::kForward); //turns ringlight on
			rightFront.Set(0.5);
			rightBack.Set(0.5);
			leftFront.Set(-0.5);
			leftBack.Set(-0.5);
			Wait(2);
			driveForward();
			Wait(2);
			rightFront.Set(0);
			rightBack.Set(0);
			leftFront.Set(0);
			leftBack.Set(0);
			//stopDriving();
			//autonomousCatapultRelease();
			compressor.Stop();
	}
	void OperatorControl()
	{
		compressor.Start(); // The class can run independently. This compressor will start and stop on its own to keep pressure under 120 PSI
		DriverStationLCD *screen = DriverStationLCD::GetInstance(); // Enables Driver Station Printing
		bool autoLoad = false; //autoLoad function
		retrievalMotor.Set(1); //Starts with retrieval wheels on
		while (IsOperatorControl())
		{
			/****** AUTO LOAD TOGGLE START *****/
			if(logitech.GetRawButton(10))
			{
				if(autoLoad)
				{
					autoLoad = false;
					Wait(0.2);	
				}
				
				else if(!autoLoad)
				{
					autoLoad = true;
					Wait(0.2);
				}
			}
			/****** AUTO LOAD TOGGLE END *****/
			/** RETRIEVAL WHEELS: RED button (B) turns the arm wheels off, GREEN button (A) turns arm wheels on, BLUE button (X) turns the arm wheels in reverse**/
			if(logitech.GetRawButton(1))
			{
				retrievalMotor.Set(-1);
			}
			if(logitech.GetRawButton(3))
			{
				retrievalMotor.Set(1);
			}
			if(logitech.GetRawButton(3))
			{
				retrievalMotor.Set(0);
			}
			/** OPTIONS ON DEM WHEELZ END **/
			
			/****** TANK DRIVE START ******/
			if (fabs(logitech.GetRawAxis(2)) > driveStickBuffer) // If the left Y Axis on the Joystick is moved beyond the driveStickBuffer
				{
					if (logitech.GetRawButton(11))
					{
						leftFront.Set(logitech.GetRawAxis(2)*(-1));
						leftBack.Set(logitech.GetRawAxis(2)*(-1));
					}
					else
					{
						leftFront.Set((driveSpeedMultiplier) * logitech.GetRawAxis(2)*(-1)); // The left motors are facing in an opposite direction
						leftBack.Set((driveSpeedMultiplier) * logitech.GetRawAxis(2)*(-1)); // The left motors are facing in an opposite direction
					}
				}		
			else
			{
				leftFront.Set(0); // Turns Left Motor 1 off
				leftBack.Set(0); // Turns Left Motor 2 off
			}
			if (fabs(logitech.GetRawAxis(4)) > driveStickBuffer) // If the  Y Axis on the Joystick is moved beyond the driveStickBuffer
			{
				if (logitech.GetRawButton(12))
				{
					rightFront.Set(logitech.GetRawAxis(4));
					rightBack.Set(logitech.GetRawAxis(4));
				}
				else
				{
					rightFront.Set((driveSpeedMultiplier) * logitech.GetRawAxis(4)); // Sets motor speed to Joystick position times the driveSpeedMultiplier
					rightBack.Set((driveSpeedMultiplier) * logitech.GetRawAxis(4)); // Sets motor speed to Joystick position times the driveSpeedMultiplier
				}
			}
			else
			{
				rightFront.Set(0);  // Turns Right Motor 1 off
				rightBack.Set(0);  // Turns Right Motor 2 off
			}
			/****** TANK DRIVE END ******/
			
			/****** BALL RETRIEVAL START ******/ /*CHECK THE POLARITY OF YOUR SOLENOIDS*/
			if (logitech.GetRawButton(8)) 										//Press Lower Right Trigger to go down and STOP ball motor
			{		
				leftArmSolenoid.Set(DoubleSolenoid::kReverse);				// Pushes the pneumatic piston forward to lower the retrieval arm
				rightArmSolenoid.Set(DoubleSolenoid::kReverse);				// Pushes the pneumatic piston forward to lower the retrieval arm
				if(retrievalMotor.Get()==1)
				{
					retrievalMotor.Set(0);									// Toggles the motor that spins the wheel to bring in the ball
				}
				else
				{
					retrievalMotor.Set(1);
				}
			}
			else if (logitech.GetRawButton(6))								//Press Upper  Trigger to go up and STOP ball motor
			{
				leftArmSolenoid.Set(DoubleSolenoid::kForward);				// Brings the pneumatic piston backward to raise the retrieval arm
				rightArmSolenoid.Set(DoubleSolenoid::kForward);				// Brings the pneumatic piston backward to raise the retrieval arm
				if(retrievalMotor.Get()==1)
				{
					retrievalMotor.Set(0);									// Toggles the motor that spins the wheel to bring in the ball
				}
				else
				{
					retrievalMotor.Set(1);
				}
			}
			else
			{
				leftArmSolenoid.Set(DoubleSolenoid::kOff);					// Does nothing to the retreival arm piston //
				rightArmSolenoid.Set(DoubleSolenoid::kOff);					//////////////////////////////////////////////
			}
			/****** BALL RETRIEVAL END ******/		
			
			/******* AUTOLOAD CATAPULT START ******/ // Limit Buttons not pressed, both dogSolenoid and ratchetSolenoids are disengaged
			if (autoLoad)
			{	
				dogSolenoid.Set(DoubleSolenoid::kForward);		// Brings the pneumatic piston backward to raise the retrieval arm
				winchMotor.Set(0.1);
				Wait(0.3);
				winchMotor.Set(0);
				ratchetSolenoid.Set(DoubleSolenoid::kForward);	// Pushes the pneumatic piston forward to lower the retrieval arm
				Wait(0.1);
				while(leftLimitSwitch.Get()==1 && rightLimitSwitch.Get()==1)
				{
					winchMotor.Set(1);
					/**ALLOW FOR WHILE LOOP TANK DRIVING**/
					if (fabs(logitech.GetRawAxis(2)) > driveStickBuffer) // If the left Y Axis on the Joystick is moved beyond the driveStickBuffer
					{
						leftFront.Set((driveSpeedMultiplier) * logitech.GetRawAxis(2)*(-1)); // The left motors are facing in an opposite direction
						leftBack.Set((driveSpeedMultiplier) * logitech.GetRawAxis(2)*(-1)); // The left motors are facing in an opposite direction
					}		
					else
					{
						leftFront.Set(0); // Turns Left Motor 1 off
						leftBack.Set(0); // Turns Left Motor 2 off
					}
					if (fabs(logitech.GetRawAxis(4)) > driveStickBuffer) // If the  Y Axis on the Joystick is moved beyond the driveStickBuffer
					{
						rightFront.Set((driveSpeedMultiplier) * logitech.GetRawAxis(4)); // Sets motor speed to Joystick position times the driveSpeedMultiplier
						rightBack.Set((driveSpeedMultiplier) * logitech.GetRawAxis(4)); // Sets motor speed to Joystick position times the driveSpeedMultiplier
					}
					else
					{
						rightFront.Set(0);  // Turns  Motor 2 off
						rightBack.Set(0);  // Turns  Motor 2 off
					}
					/**END ALLOW FOR WHILE LOOP TANK DRIVING **/
					if(logitech.GetRawButton(9)) //kills the winch
					{
						winchMotor.Set(0);
					}
					else
					{
						winchMotor.Set(1);
					}
				}
				winchMotor.Set(0); // Stops the Winch Motor since one or more buttons are pressed
			}
			/****** AUTOLOAD CATAPULT END ******/
			
			/****** MANUAL LOAD CATAPULT START ******/
			if (!autoLoad)
			{
				if (logitech.GetRawButton(5)) //pressing Upper Left Trigger loads
				{
					dogSolenoid.Set(DoubleSolenoid::kForward);		// Brings the pneumatic piston backward to raise the retrieval arm
					winchMotor.Set(0.1);
					Wait(0.3);
					ratchetSolenoid.Set(DoubleSolenoid::kForward);	// Pushes the pneumatic piston forward to lower the retrieval arm
					while(leftLimitSwitch.Get()==1 && rightLimitSwitch.Get()==1)
					{
						winchMotor.Set(1);
						/**ALLOW FOR WHILE LOOP TANK DRIVING**/
						if (fabs(logitech.GetRawAxis(2)) > driveStickBuffer) // If the left Y Axis on the Joystick is moved beyond the driveStickBuffer
						{
							leftFront.Set((driveSpeedMultiplier) * logitech.GetRawAxis(2)*(-1)); // The left motors are facing in an opposite direction
							leftBack.Set((driveSpeedMultiplier) * logitech.GetRawAxis(2)*(-1)); // The left motors are facing in an opposite direction
						}		
						else
						{
							leftFront.Set(0); // Turns Left Motor 1 off
							leftBack.Set(0); // Turns Left Motor 2 off
						}
						if (fabs(logitech.GetRawAxis(4)) > driveStickBuffer) // If the  Y Axis on the Joystick is moved beyond the driveStickBuffer
						{
							rightFront.Set((driveSpeedMultiplier) * logitech.GetRawAxis(4)); // Sets motor speed to Joystick position times the driveSpeedMultiplier
							rightBack.Set((driveSpeedMultiplier) * logitech.GetRawAxis(4)); // Sets motor speed to Joystick position times the driveSpeedMultiplier
						}
						else
						{
							rightFront.Set(0);  // Turns  Motor 2 off
							rightBack.Set(0);  // Turns  Motor 2 off
						}
						if(logitech.GetRawButton(9)) //kills the winch
						{
							winchMotor.Set(0);
						}
						else
						{
							winchMotor.Set(1);
						}
					}
					winchMotor.Set(0); //shuts the winch motor off
				}
			}
			/****** MANUAL LOAD CATAPULT END ******/
			/****** CATAPULT RELEASE START ******/ // Limit Buttons pressed, winchMotor is inactive, and lower left trigger on the joystick is pressed
			if ((leftLimitSwitch.Get()== 0 || rightLimitSwitch.Get()== 0) && winchMotor.Get() == 0 &&  logitech.GetRawButton(7))
			{
				stopDriving(); // Stops the drive so the robot doesn't flip on itself or something
				winchMotor.Set(0); // Redundant line for extra safety that can be removed after testing (The winch should already be off)
				dogSolenoid.Set(DoubleSolenoid::kReverse); // Brings the pneumatic piston backward to disengage the dog gear
				Wait(0.05); // Giving the pistons time to disengage properly
				ratchetSolenoid.Set(DoubleSolenoid::kReverse); // Brings the pneumatic piston backward to disengage the ratchet
				if(autoLoad)
				{
					Wait(3); // Waits 3 seconds after shooting before starting to load the catapult
				}
				else
				{
					
				}
			}
			/****** CATAPULT RELEASE END ******/
			screen -> PrintfLine(DriverStationLCD::kUser_Line1,"Pressure:%f", compressor.GetPressureSwitchValue()); // Print Pneumatics Pressure
			screen -> PrintfLine(DriverStationLCD::kUser_Line2,"LimSwi:%d and %d", leftLimitSwitch.Get(), rightLimitSwitch.Get()); // Print Left& Buttons
			screen -> PrintfLine(DriverStationLCD::kUser_Line3,"WinchMotor:%f", winchMotor.Get()); // Print WinchMotor State
			screen -> PrintfLine(DriverStationLCD::kUser_Line4,"Autoload:%s", autoLoad? "On" : "Off" ); // Print state of autoLoad
			//screen -> PrintfLine(DriverStationLCD::kUser_Line5,""); // Free Line
			screen -> PrintfLine(DriverStationLCD::kUser_Line6,"LDri:%f RDri:%f", leftFront.Get(), rightFront.Get()); // Print Tank Drive Motor State
			Wait(0.005); // Waits to run the loop every 0.005 seconds so the cRIO doesn't explode
			screen->UpdateLCD(); // Updates the Driverstation LCD in the loop
		}
		compressor.Stop(); // Allows the compressor to stop running if the mode is switched outside of OperatorControl
	}
	void Test() // DONT TOUCH THIS AREA. I KEEL YOU.
	{
		DriverStationLCD *screen = DriverStationLCD::GetInstance();
		
		int counter = 0;
		bool solenoidTest=0;
		while (IsTest())
		{
			if(logitech.GetRawButton(9)) //press rightBack
			{
				solenoidTest=1;
				compressor.Start();
			}
			if(logitech.GetRawButton(10)) //press Start
			{
				solenoidTest=0;
				compressor.Stop();
			}
			if(solenoidTest)
			{
				if(logitech.GetRawButton(1)) //press X
				{
					rightArmSolenoid.Set(DoubleSolenoid::kForward);
					leftArmSolenoid.Set(DoubleSolenoid::kForward);
				}
				else if(logitech.GetRawButton(2)) //press A
				{
					rightArmSolenoid.Set(DoubleSolenoid::kReverse);
					leftArmSolenoid.Set(DoubleSolenoid::kReverse);
				}
				else
				{
					leftArmSolenoid.Set(DoubleSolenoid::kOff);
					rightArmSolenoid.Set(DoubleSolenoid::kOff);
				}
				if(logitech.GetRawButton(3)) //PRess URTrigger
				{
					retrievalMotor.Set(logitech.GetRawAxis(2));
				}
				else
				{
					retrievalMotor.Set(0);
				}
				if(logitech.GetRawButton(4))
				{
					winchMotor.Set(logitech.GetRawAxis(2));
				}
				else
				{
					winchMotor.Set(0);
				}
				if(logitech.GetRawButton(5))
				{
					ratchetSolenoid.Set(DoubleSolenoid::kForward);
				}
				else if(logitech.GetRawButton(7))
				{
					ratchetSolenoid.Set(DoubleSolenoid::kReverse);
				}
				else
				{
					ratchetSolenoid.Set(DoubleSolenoid::kOff);
				}
				if(logitech.GetRawButton(6))
				{
					dogSolenoid.Set(DoubleSolenoid::kForward);
				}
				else if(logitech.GetRawButton(8))
				{
					dogSolenoid.Set(DoubleSolenoid::kReverse);
				}
				else
				{
					dogSolenoid.Set(DoubleSolenoid::kOff);
				}
			}
			else
			{
				if(logitech.GetRawButton(1)) //Press X
				{
					rightFront.Set(logitech.GetRawAxis(2)); //Press left joystick
				}
				else
				{
					rightFront.Set(0);
				}
				if(logitech.GetRawButton(2)) //Press A
				{
					rightBack.Set(logitech.GetRawAxis(2));
				}
				else
				{
					rightBack.Set(0);
				}
				if(logitech.GetRawButton(3)) //Press B
				{
					leftFront.Set(logitech.GetRawAxis(2));
				}
				else
				{
					leftFront.Set(0);
				}
				if(logitech.GetRawButton(4)) //Press Y
				{
					leftBack.Set(logitech.GetRawAxis(2));
				}
				else
				{
					leftBack.Set(0);
				}
				if(logitech.GetRawButton(5)) //Press ULTrigger
				{
					retrievalMotor.Set(logitech.GetRawAxis(2));
				}
				else
				{
					retrievalMotor.Set(0);
				}
				if(logitech.GetRawButton(6)) //PRess URTrigger
				{
						winchMotor.Set(logitech.GetRawAxis(2));
				}
				else
				{
					winchMotor.Set(0);
				}
				if(logitech.GetRawButton(7)) //Press LLTrigger
				{
					ringLight.Set(Relay::kForward);
				}
				else
				{
					ringLight.Set(Relay::kOff);
				}
				if(logitech.GetRawButton(8)) //Press LRTrigger
				{
					compressor.Start();
				}
				else
				{
					compressor.Stop();
				}
			}
			/****** MANUAL LOAD FUNCTION END *****/
			screen -> PrintfLine(DriverStationLCD::kUser_Line1,"LeftJoystick: %f", logitech.GetRawAxis(2));
			screen -> PrintfLine(DriverStationLCD::kUser_Line2,"RF:%f RB:%f LF:%f LB:%f", rightFront.Get(), rightBack.Get(), leftFront.Get(), leftBack.Get()); // Print WinchMotor State
			screen -> PrintfLine(DriverStationLCD::kUser_Line3,"Solenoid Testing:%d", solenoidTest);
			screen -> PrintfLine(DriverStationLCD::kUser_Line4,"rightArmSolenoid:%d", rightArmSolenoid.Get());
			screen -> PrintfLine(DriverStationLCD::kUser_Line5,"time:%d", counter);
			counter ++;
			
			Wait(0.005);	// Waits to run the loop every 0.005 seconds so the cRIO doesn't explode
			screen->UpdateLCD();
		}
	}
};
START_ROBOT_CLASS(RobotDemo);
