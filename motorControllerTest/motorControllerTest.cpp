// Last Edited by Sean Wallace on 3/14/14  @ 7:39 AM
//This is the most recent operator control code
// Manual load functions are located in PracticeBotOperatorControl
// "CATAPULT FAILSAFE AUTO-REPAIR FUNCTION START" Also located in PracticeBotOperatorControl
// Camera code can't be run as is for autonomous yet, needs a lot of testing
// I reccomend using particleToImagePercent over cameraOrientation as an alternative if it doesnt look like the camera code is working properly or fast enough
// Regardless it would probably be better just to drive forward and shoot -- Risk of missing could be high --

#include "WPILib.h" // Use the WPI Library
#include "Math.h" // Absolute Values Enabled
//#include "string"
class RobotDemo : public SimpleRobot
{
	const static float driveSpeedMultiplier = 0.50;
	const static float driveStickBuffer = 0.4; //sean is leaving me :(
	Joystick logitech; // Logitech Gamepad (Joystick)
	Talon leftFront, leftBack, rightFront, rightBack; // Drive Motors
	Talon retrievalMotor; // Retrieval Motor attached to the wheels located at the top of the retreival arm
	Talon winchMotor; // Winch Motor attached to the Gearbox
	Compressor compressor; // Pneumatics Compressor for the Pneumatics system
	DoubleSolenoid leftArmSolenoid; // Left Retrevial Pneumatic Arm
	DoubleSolenoid rightArmSolenoid; // Right Retrevial Pneumatic Arm
	Solenoid singleDogSolenoid;
	DoubleSolenoid ratchetSolenoid; // The ratchet needs to be engaged first to load. Disengaged second to shoot
	DigitalInput leftLimitSwitch; // Not actual limit switches. This is the left button to stop the catapult
	DigitalInput rightLimitSwitch; // Not actual limit switches. This is the right button to stop the catapult
	DigitalInput leftPositionSwitch;
	DigitalInput rightPositionSwitch;
	Relay ringLight;
	AxisCamera &camera;
public:
	RobotDemo():
		logitech(1),												// Joystick Port [Laptop USB]
		leftFront(1), leftBack(3), rightFront(2), rightBack(4), 	// Tank Drive Ports [PWM]
		retrievalMotor(6), winchMotor(5), 							// Retrieval and Catapult Winch Motor Ports [PWM]
		compressor(5,2), 											// Compressor connection ports [Digital Sidecar, Digital Sidecar]
		leftArmSolenoid(5,6), 										// Left Arm Solenoid ports [cRIO NI 9472, cRIO NI 9472]
		rightArmSolenoid(7,8), 										// Right Arm Solenoid ports [cRIO NI 9472, cRIO NI 9472]
		singleDogSolenoid(7),
		ratchetSolenoid(3,4), 										// Ratchet Solenoid ports [cRIO NI 9472, cRIO NI 9472]
		leftLimitSwitch(8), rightLimitSwitch(9), 					// Catapult Limit Switch ports [Digital Sidecar]
		leftPositionSwitch(6), // Left Autonomous Switch [Digital Sidecar]
		rightPositionSwitch(7), // Right Autonomous Switch [Digital Sidecar]
		ringLight(1, Relay::kBothDirections), // Ringlight in Digital Sidecar
		camera(AxisCamera::GetInstance("10.28.53.11"))
	{
		camera.WriteResolution(AxisCamera::kResolution_320x240);
		singleDogSolenoid.Get()==0;
		ratchetSolenoid.Get()==DoubleSolenoid::kReverse;
		if (leftLimitSwitch.Get()== 1 && rightLimitSwitch.Get()== 1 && singleDogSolenoid.Get()==0 && ratchetSolenoid.Get()==DoubleSolenoid::kReverse)
			{
				winchMotor.Set(0.1);
				singleDogSolenoid.Set(1);
				Wait(0.75);
				ratchetSolenoid.Set(DoubleSolenoid::kForward);
				Wait(0.75);
				winchMotor.Set(1);
			}
		if(leftLimitSwitch.Get()== 0 || rightLimitSwitch.Get()== 0)
			{
				winchMotor.Set(0);
			} 
	}
	void Autonomous()
	{// Panic autonomous that would theoretically drive forward, shoot, drive backward to get another ball, and repeat the process. Time can be cut down if it works (probably wont on the first try)
		ringLight.Set(Relay::kForward); //turns ringlight on
		/**CONTINGENCY CODE**/
		leftArmSolenoid.Set(DoubleSolenoid::kForward);
		rightArmSolenoid.Set(DoubleSolenoid::kForward);
		leftFront.Set(0.75);
		leftBack.Set(0.75);
		rightFront.Set(0.75);
		rightBack.Set(0.75);
		Wait(1.5);
		leftFront.Set(0);
		leftBack.Set(0);
		rightFront.Set(0);
		rightBack.Set(0);
		ratchetSolenoid.Set(DoubleSolenoid::kReverse);
		Wait(0.25);
		singleDogSolenoid.Set(0);
		leftArmSolenoid.Set(DoubleSolenoid::kReverse);
		rightArmSolenoid.Set(DoubleSolenoid::kReverse);
		Wait(1);
		leftFront.Set(-0.75);
		leftBack.Set(-0.75);
		rightFront.Set(-0.75);
		rightBack.Set(-0.75);
		if (leftLimitSwitch.Get()== 1 && rightLimitSwitch.Get()== 1 && singleDogSolenoid.Get()==0 && ratchetSolenoid.Get()==DoubleSolenoid::kReverse)
			{
				winchMotor.Set(0.1);
				singleDogSolenoid.Set(1);
				Wait(0.75);
				ratchetSolenoid.Set(DoubleSolenoid::kForward);
				Wait(0.75);
				winchMotor.Set(1);
			}
		Wait(1);
		retrievalMotor.Set(1);
		leftArmSolenoid.Set(DoubleSolenoid::kForward);
		rightArmSolenoid.Set(DoubleSolenoid::kForward);	
		Wait(1);
		leftFront.Set(0.75);
		leftBack.Set(0.75);
		rightFront.Set(0.75);
		rightBack.Set(0.75);
		Wait(1.5);
		leftFront.Set(0);
		leftBack.Set(0);
		rightFront.Set(0);
		rightBack.Set(0);
		ratchetSolenoid.Set(DoubleSolenoid::kReverse);
		Wait(0.25);
		singleDogSolenoid.Set(0);
		leftArmSolenoid.Set(DoubleSolenoid::kReverse);
		rightArmSolenoid.Set(DoubleSolenoid::kReverse);
	}
	void OperatorControl()
	{
		DriverStationLCD *screen = DriverStationLCD::GetInstance(); // Enables Driver Station Printing
		int autoLoad = 0;
		int counter = 0;
		while (IsOperatorControl())
		{
#if 0			
			/****** TEST ******/
			if (logitech.GetRawButton(1))
				{
					ratchetSolenoid.Set(DoubleSolenoid::kForward);	// Pushes the pneumatic piston forward to lower the retrieval arm
					singleDogSolenoid.Set(1);
				}
			if (logitech.GetRawButton(3))
				{
					ratchetSolenoid.Set(DoubleSolenoid::kReverse);
					singleDogSolenoid.Set(0);
				}
			/****** TEST ******/
			
			/****** AUTOLOAD FUNCTION START *****/
			if(logitech.GetRawButton(10)) 							// Start Button to do autoload
				{
					autoLoad = 1;
					ratchetSolenoid.Set(DoubleSolenoid::kReverse);
					singleDogSolenoid.Set(0);
				}
			if(logitech.GetRawButton(9)) 							// Back Button to do manual load
				{
					autoLoad = 0;	
					ratchetSolenoid.Set(DoubleSolenoid::kReverse);
					singleDogSolenoid.Set(0);
				}
			/****** AUTO LOAD FUNCTION END *****/
			
			/****** COMPRESSOR FUNCTION START *****/
			if(logitech.GetRawButton(5))
				compressor.Start(); 								// The class can run independently. This compressor will start and stop on its own to keep pressure under 120 PSI
			if(logitech.GetRawButton(7))
				compressor.Stop(); 									// Allows the compressor to stop running 
			/****** COMPRESSOR FUNCTION END *****/
#endif
			/****** TANK DRIVE START ******/
			if (fabs(logitech.GetRawAxis(2)) > driveStickBuffer)	// If the left Y Axis on the Joystick is moved beyond the driveStickBuffer
				{
					leftFront.Set((driveSpeedMultiplier) * logitech.GetRawAxis(2)*(-1));	// The left motors are facing in an opposite direction
					leftBack.Set((driveSpeedMultiplier) * logitech.GetRawAxis(2)*(-1));		// The left motors are facing in an opposite direction
				}		
			else
				{
					leftFront.Set(0);								// Turns Left Motor 1 off
					leftBack.Set(0);								// Turns Left Motor 2 off
				}
			if (fabs(logitech.GetRawAxis(4)) > driveStickBuffer) 	// If the right Y Axis on the Joystick is moved beyond the driveStickBuffer
				{
					rightFront.Set((driveSpeedMultiplier) * logitech.GetRawAxis(4));	// Sets motor speed to Joystick position times the driveSpeedMultiplier
					rightBack.Set((driveSpeedMultiplier) * logitech.GetRawAxis(4));		// Sets motor speed to Joystick position times the driveSpeedMultiplier
				}
			else
				{
					rightFront.Set(0);  // Turns Right Motor 2 off
					rightBack.Set(0);  // Turns Right Motor 2 off
				}
			if (logitech.GetRawButton(4))
			{	
				retrievalMotor.Set(1);	
			}
			else
			{
				retrievalMotor.Set(0);
			}	
			/****** TANK DRIVE END ******/
#if 0			
			/****** BALL RETRIEVAL START ******/
			if (logitech.GetRawButton(6)) 										//Press Upper Right Trigger to go down and start ball motor
				{		
					retrievalMotor.Set(1);										// Turns on the motor that spins the wheel to bring in the ball
					leftArmSolenoid.Set(DoubleSolenoid::kForward);				// Pushes the pneumatic piston forward to lower the retrieval arm
					rightArmSolenoid.Set(DoubleSolenoid::kForward);				// Pushes the pneumatic piston forward to lower the retrieval arm
				}
			else if (logitech.GetRawButton(8))									//Press Lower Right Trigger to go up and stop ball motor
				{
					retrievalMotor.Set(1);										// Turns off the motor that spins the wheel to bring in the ball
					leftArmSolenoid.Set(DoubleSolenoid::kReverse);				// Brings the pneumatic piston backward to raise the retrieval arm
					rightArmSolenoid.Set(DoubleSolenoid::kReverse);				// Brings the pneumatic piston backward to raise the retrieval arm
				}
			else
				{
					retrievalMotor.Set(0);										// Turns off the motor that spins the wheel to bring in the ball
					leftArmSolenoid.Set(DoubleSolenoid::kOff);					// Does nothing to the retreival arm piston
					rightArmSolenoid.Set(DoubleSolenoid::kOff);				// Does nothing to the retreival arm piston
				}
			/****** BALL RETRIEVAL END ******/	
			
			/******* AUTOLOAD CATAPULT START ******/							// Limit Buttons not pressed, both dogSolenoid and ratchetSolenoids are disengaged
			if (autoLoad == 1)
				{
					if (leftLimitSwitch.Get()== 1 && rightLimitSwitch.Get()== 1 && singleDogSolenoid.Get()==0 && ratchetSolenoid.Get()==DoubleSolenoid::kReverse)
						{
							winchMotor.Set(0.1);
							singleDogSolenoid.Set(1);
							Wait(0.75);
							ratchetSolenoid.Set(DoubleSolenoid::kForward);
							Wait(0.75);
							winchMotor.Set(1);
							screen -> PrintfLine(DriverStationLCD::kUser_Line6,"Loading!");
						}
					if(leftLimitSwitch.Get()== 0 || rightLimitSwitch.Get()== 0)
						{
							winchMotor.Set(0);
							screen -> PrintfLine(DriverStationLCD::kUser_Line6,"Press A to Fire!");
						} 
					if (winchMotor.Get() == 0 && logitech.GetRawButton(2))
						{
							ratchetSolenoid.Set(DoubleSolenoid::kReverse);
							Wait(0.75);
							singleDogSolenoid.Set(0);
							Wait(0.75);
							screen -> PrintfLine(DriverStationLCD::kUser_Line6,"Fire Complete!");
							Wait(2);
						}
				}
			/****** AUTO LOAD CATAPULT END *****/
#endif
			screen -> PrintfLine(DriverStationLCD::kUser_Line1,"Auto:%d time=%d", autoLoad, counter);
			screen -> PrintfLine(DriverStationLCD::kUser_Line2,"6%d 8%d 9%d 10%d 1%d 3%d", logitech.GetRawButton(6), logitech.GetRawButton(8), logitech.GetRawButton(9), logitech.GetRawButton(10), logitech.GetRawButton(1), logitech.GetRawButton(3));
			screen -> PrintfLine(DriverStationLCD::kUser_Line3,"WinchMotor:%f", winchMotor.Get()); // Print WinchMotor State
			screen -> PrintfLine(DriverStationLCD::kUser_Line4,"Dog%d Ratch%d L%d R%d", singleDogSolenoid.Get(), ratchetSolenoid.Get(), leftArmSolenoid.Get(), rightArmSolenoid.Get());
			screen -> PrintfLine(DriverStationLCD::kUser_Line5,"Lim%d,%d-Flippy%d,%d", leftLimitSwitch.Get(), rightLimitSwitch.Get());
			counter ++;
			Wait(0.005);																											// Waits to run the loop every 0.005 seconds so the cRIO doesn't explode
			screen->UpdateLCD();																									// Updates the Driverstation LCD in the loop
		}
		compressor.Stop();								// Allows the compressor to stop running if the mode is switched outside of OperatorControl
	}
	void Test()
	{
		DriverStationLCD *screen = DriverStationLCD::GetInstance();
		int counter = 0;
		int autoLoad = 0;
		while (IsTest())
		{
#if 0
			/****** COMPRESSOR ON/OFF START *****/
			if(logitech.GetRawButton(5)) //Press ULTrigger
				compressor.Start();						// The class can run independently. This compressor will start and stop on its own to keep pressure under 120 PSI
			if(logitech.GetRawButton(7)) //Press LLTrigger
				compressor.Stop();						// Allows the compressor to stop running 
			/****** COMPRESSOR ON/OFF END *****/
		
			/****** CONFIGURE AUTO LOAD FUNCTION START *****/
#endif
			/******DRIVE MOTOR TEST START******/
			if(logitech.GetRawButton(1)) //Press X
			{
				rightFront.Set(logitech.GetRawAxis(2));
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
			/******DRIVE MOTOR REST END******/
#if 0
			if(logitech.GetRawButton(3))
				{
					autoLoad = 1;
					singleDogSolenoid.Set(0);
					ratchetSolenoid.Set(DoubleSolenoid::kReverse);
				}
			if(logitech.GetRawButton(1))
				{
					autoLoad = 0;	
					singleDogSolenoid.Set(0);
					ratchetSolenoid.Set(DoubleSolenoid::kReverse);
				}
			/****** CONFIGURE AUTO LOAD FUNCTION END *****/
			
			/****** AUTO LOAD FUNCTION START *****/
			/* Loadstates 0 = Unloaded, 1 = Loaded, 2=Loading 3= FIRE 4 = ready for automation*/
			if (autoLoad == 1)
				{
					if (leftLimitSwitch.Get()== 1 && rightLimitSwitch.Get()== 1 && singleDogSolenoid.Get()==0 && ratchetSolenoid.Get()==DoubleSolenoid::kReverse)
						{
							screen -> PrintfLine(DriverStationLCD::kUser_Line6,"Loading Start!");
							winchMotor.Set(0.1);
							singleDogSolenoid.Set(1);
							Wait(1);
							ratchetSolenoid.Set(DoubleSolenoid::kForward);
							Wait(1);
							winchMotor.Set(1);
							screen -> PrintfLine(DriverStationLCD::kUser_Line6,"Loading Continuing!");
						}
					if(leftLimitSwitch.Get()== 0 || rightLimitSwitch.Get()== 0)
						{
							winchMotor.Set(0);
							screen -> PrintfLine(DriverStationLCD::kUser_Line6,"Ready to Fire!");
						} 
					if (winchMotor.Get() == 0 && logitech.GetRawButton(4))
						{
							screen -> PrintfLine(DriverStationLCD::kUser_Line6,"Firing!");
							ratchetSolenoid.Set(DoubleSolenoid::kReverse);
							Wait(1);
							singleDogSolenoid.Set(0);
							Wait(1);
							screen -> PrintfLine(DriverStationLCD::kUser_Line6,"Fire Complete!");
							Wait(2);
							
					/****** CATAPULT FAILSAFE FUNCTION START *****/
					if (leftLimitSwitch.Get()== 1 && rightLimitSwitch.Get()== 1 && singleDogSolenoid.Get()==0 && ratchetSolenoid.Get()==DoubleSolenoid::kReverse)
						{
							ratchetSolenoid.Set(DoubleSolenoid::kForward);
						}
					else if (leftLimitSwitch.Get()== 1 && rightLimitSwitch.Get()== 1 && singleDogSolenoid.Get()==0 && ratchetSolenoid.Get()==DoubleSolenoid::kForward)
						{
							ratchetSolenoid.Set(DoubleSolenoid::kReverse);
							singleDogSolenoid.Set(1);
							ratchetSolenoid.Set(DoubleSolenoid::kForward);
						}
					else if (leftLimitSwitch.Get()== 0 || rightLimitSwitch.Get()== 0 && singleDogSolenoid.Get()==0 && ratchetSolenoid.Get()==DoubleSolenoid::kReverse)
						{
							ratchetSolenoid.Set(DoubleSolenoid::kReverse);
						}
					else
					{
						screen -> PrintfLine(DriverStationLCD::kUser_Line6,"Working As Intended");
					}
					/****** CATAPULT FAILSAFE FUNCTION END *****/
						}
				}
			/****** AUTO LOAD FUNCTION END *****/
			
			//****** MANUAL LOAD START ******/
			if (autoLoad == 0)
				{
				if (logitech.GetRawButton(4) && leftLimitSwitch.Get()== 1 && rightLimitSwitch.Get()== 1)
						{
							winchMotor.Set(0.1);
							singleDogSolenoid.Set(1);
							Wait(2);
							ratchetSolenoid.Set(DoubleSolenoid::kForward);
							Wait(2);
							winchMotor.Set(1);
						}
				
				if (leftLimitSwitch.Get()== 0 && rightLimitSwitch.Get()== 0)
						{
					winchMotor.Set(0);
						}
				if ((leftLimitSwitch.Get()== 0 && rightLimitSwitch.Get()== 0) && logitech.GetRawButton(4))
					{
						winchMotor.Set(0);
						singleDogSolenoid.Set(0);
						Wait(2);
						ratchetSolenoid.Set(DoubleSolenoid::kReverse);
						Wait(2);
					}
			}
			//****** MANUAL LOAD END ******/
			
			/****** TEST ******/
			if (logitech.GetRawButton(9))
				{
					singleDogSolenoid.Set(1);			// Pushes the pneumatic piston forward to lower the retrieval arm
				}
			else if (logitech.GetRawButton(10))
				{
				singleDogSolenoid.Set(0);			// Pushes the pneumatic piston forward to lower the retrieval arm
				}
			/****** TEST ******/
#endif				
			/****** MANUAL LOAD FUNCTION END *****/
			screen -> PrintfLine(DriverStationLCD::kUser_Line1,"Auto?:%d t=%d", autoLoad, counter);
			screen -> PrintfLine(DriverStationLCD::kUser_Line2,"B2%d B4%d B9%d B10%d", logitech.GetRawButton(2), logitech.GetRawButton(4), logitech.GetRawButton(9), logitech.GetRawButton(10));
			screen -> PrintfLine(DriverStationLCD::kUser_Line3,"WinchMotor:%f", winchMotor.Get());
			screen -> PrintfLine(DriverStationLCD::kUser_Line4,"Dog%d Ratch%d L%d R%d", singleDogSolenoid.Get(), ratchetSolenoid.Get(), leftArmSolenoid.Get(), rightArmSolenoid.Get());
			screen -> PrintfLine(DriverStationLCD::kUser_Line5,"Lim%d,%d-Flippy%d,%d", leftLimitSwitch.Get(), rightLimitSwitch.Get());
			counter ++;
			Wait(0.005);
			screen->UpdateLCD();
			}
		}
	};
START_ROBOT_CLASS(RobotDemo);
