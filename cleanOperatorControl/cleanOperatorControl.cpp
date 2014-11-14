// Last Edited by Sean Wallace on 2/17/14  @ 2:22 PM
#include "WPILib.h" // Use the WPI Library
#include "Math.h" // Absolute Values Enabled
//#include "string"
class RobotDemo : public SimpleRobot
{
	const static float driveSpeedMultiplier = 0.50;
	const static float driveStickBuffer = 0.4;
	Joystick logitech; // Logitech Gamepad (Joystick)
	Talon leftFront, leftBack, rightFront, rightBack; // Drive Motors
	Talon retrievalMotor; // Retrieval Motor attached to the wheels located at the top of the retreival arm
	Talon winchMotor; // Winch Motor attached to the Gearbox
	Compressor compressor; // Pneumatics Compressor for the Pneumatics system
	DoubleSolenoid leftArmSolenoid; // Left Retrevial Pneumatic Arm
	DoubleSolenoid rightArmSolenoid; // Right Retrevial Pneumatic Arm
	DoubleSolenoid dogSolenoid; // The dog gear needs to be engaged second to load. Disengaged first to shoot
	DoubleSolenoid ratchetSolenoid; // The ratchet needs to be engaged first to load. Disengaged second to shoot
	DigitalInput leftLimitSwitch; // Not actual limit switches. This is the left button to stop the catapult
	DigitalInput rightLimitSwitch; // Not actual limit switches. This is the right button to stop the catapult
	/****** Autonomous Equipment Start ******/
	//DigitalInput leftPositionSwitch;
	//DigitalInput rightPositionSwitch;
	//Relay ringLight;
	//AxisCamera &camera;
	/****** Autonomous Equipment End ******/
public:
	RobotDemo():
		logitech(1),												// Joystick Port [Laptop USB]
		leftFront(1), leftBack(3), rightFront(2), rightBack(4), 	// Tank Drive Ports [PWM]
		retrievalMotor(6), winchMotor(5), 							// Retrieval and Catapult Winch Motor Ports [PWM]
		compressor(5,2), 											// Compressor connection ports [Digital Sidecar, Digital Sidecar]
		leftArmSolenoid(5,6), 										// Left Arm Solenoid ports [cRIO NI 9472, cRIO NI 9472]
		rightArmSolenoid(7,8), 										// Right Arm Solenoid ports [cRIO NI 9472, cRIO NI 9472]
		dogSolenoid(1,2), 											// Dog Solenoid ports [cRIO NI 9472, cRIO NI 9472]
		ratchetSolenoid(3,4), 										// Ratchet Solenoid ports [cRIO NI 9472, cRIO NI 9472]
		leftLimitSwitch(8), rightLimitSwitch(9) 					// Catapult Limit Switch ports [Digital Sidecar]
	/****** Autonomous Equipment Start ******/
	//leftPositionSwitch(6), // Left Autonomous Switch [Digital Sidecar]
	//rightPositionSwitch(7), // Right Autonomous Switch [Digital Sidecar]
	//ringLight(1, Relay::kBothDirections), // Ringlight in Digital Sidecar
	//camera(AxisCamera::GetInstance("10.28.53.11"))
	{
	//Expirations can be put here. WindRiver gets mad if you remove these curlybraces.
	//camera.WriteResolution(AxisCamera::kResolution_320x240); // loads the camera's resolution
	}
	/****** Autonomous Equipment End ******/
	void Stop()
	{
		leftFront.Set(0);
		leftBack.Set(0);
		rightFront.Set(0);
		rightBack.Set(0);
	}
	void Autonomous()
	{		
		while (IsAutonomous())
		{
		if (fabs(logitech.GetRawAxis(4)) > driveStickBuffer) 	// If the right Y Axis on the Joystick is moved beyond the driveStickBuffer
			{
				rightBack.Set((driveSpeedMultiplier) * logitech.GetRawAxis(4));		// Sets motor speed to Joystick position times the driveSpeedMultiplier
			}
		else
		{
			rightBack.Set(0);
		}
		}
	}
	void OperatorControl()
	{
		//compressor.Start(); 										// The class can run independently. This compressor will start and stop on its own to keep pressure under 120 PSI
		DriverStationLCD *screen = DriverStationLCD::GetInstance(); // Enables Driver Station Printing
		int autoLoad = 0;
		int counter = 0;
		while (IsOperatorControl())
		{
			/****** TEST ******/
			if (logitech.GetRawButton(1))
				{
					ratchetSolenoid.Set(DoubleSolenoid::kForward);	// Pushes the pneumatic piston forward to lower the retrieval arm
					dogSolenoid.Set(DoubleSolenoid::kForward);		// Brings the pneumatic piston backward to raise the retrieval arm
				}
			if (logitech.GetRawButton(3))
				{
					ratchetSolenoid.Set(DoubleSolenoid::kReverse);
					dogSolenoid.Set(DoubleSolenoid::kReverse);
				}
			/****** TEST ******/
			
			/****** AUTOLOAD FUNCTION START *****/
			if(logitech.GetRawButton(10)) 							// Start Button to do autoload
				{
					autoLoad = 1;
					ratchetSolenoid.Set(DoubleSolenoid::kReverse);
					dogSolenoid.Set(DoubleSolenoid::kReverse);
				}
			if(logitech.GetRawButton(9)) 							// Back Button to do manual load
				{
					autoLoad = 0;	
					ratchetSolenoid.Set(DoubleSolenoid::kReverse);
					dogSolenoid.Set(DoubleSolenoid::kReverse);
				}
			/****** AUTO LOAD FUNCTION END *****/
			
			/****** AUTO LOAD FUNCTION START *****/
			if(logitech.GetRawButton(5))
				compressor.Start(); 								// The class can run independently. This compressor will start and stop on its own to keep pressure under 120 PSI
			if(logitech.GetRawButton(7))
				compressor.Stop(); 									// Allows the compressor to stop running 
			/****** AUTO LOAD FUNCTION END *****/
			
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
			/****** TANK DRIVE END ******/
			
			/****** BALL RETRIEVAL START ******/
			if (logitech.GetRawButton(8)) 										//Press Upper Right Trigger to go down and start ball motor
				{		
					retrievalMotor.Set(1);										// Turns on the motor that spins the wheel to bring in the ball
					leftArmSolenoid.Set(DoubleSolenoid::kForward);				// Pushes the pneumatic piston forward to lower the retrieval arm
					rightArmSolenoid.Set(DoubleSolenoid::kForward);				// Pushes the pneumatic piston forward to lower the retrieval arm
				}
			else if (logitech.GetRawButton(6))									//Press Lower Right Trigger to go up and stop ball motor
				{
					retrievalMotor.Set(1);										// Turns off the motor that spins the wheel to bring in the ball
					leftArmSolenoid.Set(DoubleSolenoid::kReverse);				// Brings the pneumatic piston backward to raise the retrieval arm
					rightArmSolenoid.Set(DoubleSolenoid::kReverse);				// Brings the pneumatic piston backward to raise the retrieval arm
				}
			else
				{
					retrievalMotor.Set(0);										// Turns off the motor that spins the wheel to bring in the ball
					leftArmSolenoid.Set(DoubleSolenoid::kOff);					// Does nothing to the retreival arm piston
					rightArmSolenoid.Set(DoubleSolenoid::kOff);					// Does nothing to the retreival arm piston
				}
			/****** BALL RETRIEVAL END ******/	
			
			/******* AUTOLOAD CATAPULT START ******/							// Limit Buttons not pressed, both dogSolenoid and ratchetSolenoids are disengaged
			if (autoLoad == 1)
				{
					if (leftLimitSwitch.Get()== 1 && rightLimitSwitch.Get()== 1 && dogSolenoid.Get()==DoubleSolenoid::kReverse && dogSolenoid.Get()==DoubleSolenoid::kReverse)
						{
							winchMotor.Set(0.1);
							dogSolenoid.Set(DoubleSolenoid::kForward);
							Wait(1);
							ratchetSolenoid.Set(DoubleSolenoid::kForward);
							Wait(1);
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
							Wait(1);
							dogSolenoid.Set(DoubleSolenoid::kReverse);
							Wait(1);
							screen -> PrintfLine(DriverStationLCD::kUser_Line6,"Fire Complete!");
							Wait(2);
						}
					/****** CATAPULT FAILSAFE AUTO-REPAIR FUNCTION START *****/
					/*
					if (leftLimitSwitch.Get()== 1 && rightLimitSwitch.Get()== 1 && dogSolenoid.Get()==DoubleSolenoid::kForward && ratchetSolenoid.Get()==DoubleSolenoid::kReverse)
						{
							ratchetSolenoid.Set(DoubleSolenoid::kForward);
						}
					else if (leftLimitSwitch.Get()== 1 && rightLimitSwitch.Get()== 1 && dogSolenoid.Get()==DoubleSolenoid::kReverse && ratchetSolenoid.Get()==DoubleSolenoid::kForward)
						{
							ratchetSolenoid.Set(DoubleSolenoid::kReverse);
							dogSolenoid.Set(DoubleSolenoid::kForward);
							ratchetSolenoid.Set(DoubleSolenoid::kForward);
						}
					else if (leftLimitSwitch.Get()== 0 || rightLimitSwitch.Get()== 0 && dogSolenoid.Get()==DoubleSolenoid::kReverse && ratchetSolenoid.Get()==DoubleSolenoid::kReverse)
						{
							ratchetSolenoid.Set(DoubleSolenoid::kReverse);
						}
					else
					{
						screen -> PrintfLine(DriverStationLCD::kUser_Line6,"Working As Intended");
					}
					*/
				}
				/****** CATAPULT FAILSAFE AUTO-REPAIR FUNCTION END *****/
			/****** AUTO LOAD CATAPULT END *****/
			
			/****** MANUAL LOAD START ******/
			if (autoLoad == 0)
				{ 
				if (logitech.GetRawButton(4) && leftLimitSwitch.Get()== 1 && rightLimitSwitch.Get()== 1)
					{
						winchMotor.Set(0.1);
						dogSolenoid.Set(DoubleSolenoid::kForward);
						Wait(1);
						ratchetSolenoid.Set(DoubleSolenoid::kForward);
						Wait(1);
						winchMotor.Set(1);
					}
				if (leftLimitSwitch.Get()== 0 && rightLimitSwitch.Get()== 0)
					{
						winchMotor.Set(0);
					}
				if ((leftLimitSwitch.Get()== 0 && rightLimitSwitch.Get()== 0) && logitech.GetRawButton(2))
					{
						ratchetSolenoid.Set(DoubleSolenoid::kReverse); //THIS NEEDS TO BE TESTED 
						Wait(1);
						dogSolenoid.Set(DoubleSolenoid::kReverse);
						Wait(2);
					}
			}
			//****** MANUAL LOAD END ******/
			screen -> PrintfLine(DriverStationLCD::kUser_Line1,"Auto?:%d t=%d", autoLoad, counter);
			screen -> PrintfLine(DriverStationLCD::kUser_Line2,"B2%d B4%d B9%d B10%d", logitech.GetRawButton(2), logitech.GetRawButton(4), logitech.GetRawButton(9), logitech.GetRawButton(10));
			screen -> PrintfLine(DriverStationLCD::kUser_Line3,"WinchMotor:%f", winchMotor.Get()); // Print WinchMotor State
			screen -> PrintfLine(DriverStationLCD::kUser_Line4,"Dog%d Ratch%d L%d R%d", dogSolenoid.Get(), ratchetSolenoid.Get(), leftArmSolenoid.Get(), rightArmSolenoid.Get());
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
			/****** COMPRESSOR ON/OFF START *****/
			if(logitech.GetRawButton(5))
				compressor.Start();						// The class can run independently. This compressor will start and stop on its own to keep pressure under 120 PSI
			if(logitech.GetRawButton(7))
				compressor.Stop();						// Allows the compressor to stop running 
			/****** COMPRESSOR ON/OFF END *****/
		
			/****** CONFIGURE AUTO LOAD FUNCTION START *****/
			if(logitech.GetRawButton(11))
				{
					dogSolenoid.Set(DoubleSolenoid::kForward);
					ratchetSolenoid.Set(DoubleSolenoid::kForward);
				}
			if(logitech.GetRawButton(12))
				{
					dogSolenoid.Set(DoubleSolenoid::kReverse);
					ratchetSolenoid.Set(DoubleSolenoid::kReverse);
				}
			
			
			if(logitech.GetRawButton(3))
				{
					autoLoad = 1;
					dogSolenoid.Set(DoubleSolenoid::kReverse);
					ratchetSolenoid.Set(DoubleSolenoid::kReverse);
				}
			if(logitech.GetRawButton(1))
				{
					autoLoad = 0;	
					dogSolenoid.Set(DoubleSolenoid::kReverse);
					ratchetSolenoid.Set(DoubleSolenoid::kReverse);
				}
			/****** CONFIGURE AUTO LOAD FUNCTION END *****/
			
			/****** AUTO LOAD FUNCTION START *****/
			/* Loadstates 0 = Unloaded, 1 = Loaded, 2=Loading 3= FIRE 4 = ready for automation*/
			if (autoLoad == 1)
				{
					if (leftLimitSwitch.Get()== 1 && rightLimitSwitch.Get()== 1 && dogSolenoid.Get()==DoubleSolenoid::kReverse && dogSolenoid.Get()==DoubleSolenoid::kReverse)
						{
							screen -> PrintfLine(DriverStationLCD::kUser_Line6,"Loading Start!");
							winchMotor.Set(0.1);
							dogSolenoid.Set(DoubleSolenoid::kForward);
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
							dogSolenoid.Set(DoubleSolenoid::kReverse);
							Wait(1);
							screen -> PrintfLine(DriverStationLCD::kUser_Line6,"Fire Complete!");
							Wait(2);
							
					/****** CATAPULT FAILSAFE FUNCTION START *****/
					if (leftLimitSwitch.Get()== 1 && rightLimitSwitch.Get()== 1 && dogSolenoid.Get()==DoubleSolenoid::kForward && ratchetSolenoid.Get()==DoubleSolenoid::kReverse)
						{
							ratchetSolenoid.Set(DoubleSolenoid::kForward);
						}
					else if (leftLimitSwitch.Get()== 1 && rightLimitSwitch.Get()== 1 && dogSolenoid.Get()==DoubleSolenoid::kReverse && ratchetSolenoid.Get()==DoubleSolenoid::kForward)
						{
							ratchetSolenoid.Set(DoubleSolenoid::kReverse);
							dogSolenoid.Set(DoubleSolenoid::kForward);
							ratchetSolenoid.Set(DoubleSolenoid::kForward);
						}
					else if (leftLimitSwitch.Get()== 0 || rightLimitSwitch.Get()== 0 && dogSolenoid.Get()==DoubleSolenoid::kReverse && ratchetSolenoid.Get()==DoubleSolenoid::kReverse)
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
							dogSolenoid.Set(DoubleSolenoid::kForward);
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
						dogSolenoid.Set(DoubleSolenoid::kReverse);
						Wait(2);
						ratchetSolenoid.Set(DoubleSolenoid::kReverse);
						Wait(2);
					}
			}
			//****** MANUAL LOAD END ******/
			
			/****** TEST ******/
			/****** TEST ******/
				
			/****** MANUAL LOAD FUNCTION END *****/
			screen -> PrintfLine(DriverStationLCD::kUser_Line1,"Auto?:%d t=%d", autoLoad, counter);
			screen -> PrintfLine(DriverStationLCD::kUser_Line2,"B2%d B4%d B9%d B10%d", logitech.GetRawButton(2), logitech.GetRawButton(4), logitech.GetRawButton(9), logitech.GetRawButton(10));
			screen -> PrintfLine(DriverStationLCD::kUser_Line3,"WinchMotor:%f", winchMotor.Get());
			screen -> PrintfLine(DriverStationLCD::kUser_Line4,"Dog%d Ratch%d L%d R%d", dogSolenoid.Get(), ratchetSolenoid.Get(), leftArmSolenoid.Get(), rightArmSolenoid.Get());
			screen -> PrintfLine(DriverStationLCD::kUser_Line5,"Lim%d,%d-Flippy%d,%d", leftLimitSwitch.Get(), rightLimitSwitch.Get());
			counter ++;
			Wait(0.005);
			screen->UpdateLCD();
			}
		}
	};
START_ROBOT_CLASS(RobotDemo);
