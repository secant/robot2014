// Last Edited by Vivian Fang on 3/23/14  @ 6:26 PM
#include "WPILib.h" // Use the WPI Library
#include "Math.h" // Absolute Values Enabled
class RobotDemo : public SimpleRobot //all electrical components declared here
{
	const static float driveSpeedMultiplier = 0.5; 		//Sets the max power of the drive
	const static float driveStickBuffer = 0.25; 		//Sets the threshold for actually moving the drive
	Joystick logitech; 									// Logitech Gamepad (Joystick)
	Talon leftFront, leftBack, Front, Back; 	// Drive Motors
	Talon retrievalMotor; 								// Retrieval Motor attached to the wheels located at the top of the retreival arm
	Talon winchMotor; 									// Winch Motor attached to the Gearbox
	Compressor compressor; 								// Pneumatics Compressor for the Pneumatics system
	DoubleSolenoid leftArmSolenoid; 					// Left Retrevial Pneumatic Arm
	Solenoid rightArmSolenoidS; 					//  Retrevial Pneumatic Arm
	DoubleSolenoid dogSolenoid; 						// Disengaged first to shoot
	DoubleSolenoid ratchetSolenoid; 					// Disengaged second to shoot
	DigitalInput leftLimitSwitch; 						// Left button to stop the catapult
	DigitalInput LimitSwitch; 						//  button to stop the catapult
	
	/****** AUTONOMOUS START ******/
	DigitalInput leftPositionSwitch;
	DigitalInput PositionSwitch;
	Relay ringLight;
	AxisCamera &camera;
	/****** AUTONOMOUS END ******/
public: //all ports of electrical components declared here
	RobotDemo():
		logitech(1), // Joystick Port [Laptop USB]
		leftFront(1), leftBack(3), Front(2), Back(4), // Tank Drive Ports [PWMOUT]
		retrievalMotor(6), winchMotor(5), // Retrieval and Catapult Winch Motor Ports [PWMOUT]
		compressor(5,2), // Compressor connection ports [Relay, Digital IO]
		leftArmSolenoid(5,6), // Left Arm Solenoid ports [Solenoid Breakout, Solenoid Breakout]
		rightArmSolenoidS(7), //  Arm Solenoid ports [Solenoid Breakout, Solenoid Breakout]
		dogSolenoid(1,2), // Dog Solenoid ports [Solenoid Breakout, Solenoid Breakout]
		ratchetSolenoid(3,4), // Ratchet Solenoid ports [Solenoid Breakout, Solenoid Breakout]
		leftLimitSwitch(8), LimitSwitch(9), // Catapult Limit Switch ports [Digital IO]
		/****** AUTONOMOUS ******/
		leftPositionSwitch(6), // Left Autonomous Switch [Digital IO]
		PositionSwitch(7), //  Autonomous Switch [Digital IO]
		ringLight(1, Relay::kBothDirections), // Ringlight [Relay]
		camera(AxisCamera::GetInstance("10.28.53.11")) //IP address of the camera
		{
			//Expirations can be put here. WindRiver gets mad if you remove these braces.
			camera.WriteResolution(AxisCamera::kResolution_320x240); // sets the camera's resolution
			Front.SetExpiration(0.1);
			Back.SetExpiration(0.1);
			leftFront.SetExpiration(0.1);
			leftBack.SetExpiration(0.1);
			winchMotor.SetExpiration(0.1);
			retrievalMotor.SetExpiration(0.1);
		}
		/****** AUTONOMOUS END ******/
	/****** AUTO FUNCTIONS START *******/
	void stopDriving()
	{
		leftFront.Set(0);
		leftBack.Set(0);
		Front.Set(0);
		Back.Set(0);
	}
	void driveForward() //Creates a function to drive forward (sets it)
	{
		leftFront.Set(0.75);
		leftBack.Set(0.75);
		Front.Set(0.75);
		Back.Set(0.75);
	}
	void turn() //Creates a function to turn the robot a set amount  and then stops movement
	{
		leftFront.Set(0.5);
		leftBack.Set(0.5);
		Front.Set(-0.5);
		Back.Set(-0.5);
		Wait(1.5);
		leftFront.Set(0);
		leftBack.Set(0);
		Front.Set(0);
		Back.Set(0);
	}
	void turnLeft() //Creates a function to turn the robot a set amount left and then stops movement
	{
		leftFront.Set(-0.5);
		leftBack.Set(-0.5);
		Front.Set(0.5);
		Back.Set(0.5);
		Wait(1.5);
		leftFront.Set(0);
		leftBack.Set(0);
		Front.Set(0);
		Front.Set(0);
	}
	void turnLeftMore() //Creates a function to turn the robot a larger set amount left and then stops movement.
	{
		leftFront.Set(-1);
		leftBack.Set(-1);
		Front.Set(1);
		Front.Set(1);
		Wait(1.5);
		leftFront.Set(0);
		leftBack.Set(0);
		Front.Set(0);
		Back.Set(0);
	}
	void turnMore() //Creates a function to turn the robot a larger set amount  and then stops movement.
	{
		leftFront.Set(1);
		leftBack.Set(1);
		Front.Set(-1);
		Front.Set(-1);
		Wait(1.5);
		leftFront.Set(0);
		leftBack.Set(0);
		Front.Set(0);
		Back.Set(0);
	}
	void driveBackward() //Creates a function to drive backward
	{
		leftFront.Set(-0.75);
		leftBack.Set(-0.75);
		Front.Set(-0.75);
		Back.Set(-0.75);
	}
	void autonomousCatapultRelease()
	{
		if ((leftLimitSwitch.Get()== 0 || LimitSwitch.Get()== 0) && winchMotor.Get() == 0 &&  logitech.GetRawButton(2))
		{
			stopDriving(); // Stops the drive so the robot doesn't flip on itself or something
			winchMotor.Set(0); // Redundant line for extra safety that can be removed after testing (The winch should already be off)
			dogSolenoid.Set(DoubleSolenoid::kReverse); // Brings the pneumatic piston backward to disengage the dog gear
			Wait(0.2); // Giving the pistons time to disengage properly
			ratchetSolenoid.Set(DoubleSolenoid::kReverse); // Brings the pneumatic piston backward to disengage the ratchet
			Wait(5); // Waits 5 seconds after shooting before starting to load the catapult
		}
	}
	/****** AUTO FUNCTIONS END *******/
	void Autonomous()
	{
		compressor.Start();
		rightArmSolenoidS.Set(1);
		leftArmSolenoid.Set(DoubleSolenoid::kReverse);
		/*** ENSURES THE CATAPULT IS LOADED AND LOADS IF UNLOADED ***/
		while (leftLimitSwitch.Get() == 1 && LimitSwitch.Get() == 1) // If Limit Switch Buttons are not pressed
			{
				if ((dogSolenoid.Get() && ratchetSolenoid.Get()) == DoubleSolenoid::kReverse)	//If both solenoids are disengaged, it will
																								//start the loading process from scratch
				{
					//Stop(); //Stops the motors so bad things don't happen --We should stop force stop the drive for autonomous to shoot, not to load ?--
					Wait(2); // Just incase the -- REMOVE AFTER TESTING --
					winchMotor.Set(0.1); // Gears need to be moving slowly to allow the dog gear to engage properly
					dogSolenoid.Set(DoubleSolenoid::kForward); // Pushes the pneumatic piston forward to engage the dog gear
					Wait(0.2); // Giving the pistons time to engage properly
					winchMotor.Set(0); // Now that the dog gear is engaged, the gears do not have to move
					ratchetSolenoid.Set(DoubleSolenoid::kForward); // Pushes the pneumatic piston forward to engage the ratchet
					Wait(0.2); // Giving the pistons time to engage properly
					winchMotor.Set(1); //Now starts the winch motor to load the catapult
				}
				else // If Limit Switch Buttons are not pressed
				{
					winchMotor.Set(1); // Continues to run the Winch Motor
				}
			}
		// If the Catapult Left &  Limit Switches are (0,0), (0,1), (1,0)
		{
			winchMotor.Set(0); // Stops the Winch Motor since one or more buttons are pressed
			if ((dogSolenoid.Get() == DoubleSolenoid::kReverse) && (ratchetSolenoid.Get() == DoubleSolenoid::kForward)) // If the Dog Gear is disengaged but the ratchet is engaged
				{
					winchMotor.Set(0.05); // Gears need to be moving slowly to allow the dog gear to engage properly. Might want to test this since the catapult's already loaded.
					dogSolenoid.Set(DoubleSolenoid::kForward); // Engages the dog gear so both dog gear and ratchet are engaged before shooting for safety
					Wait(0.5); // Giving the pistons time to engage properly
					winchMotor.Set(0); // Now that the dog gear is engaged, the gears do not have to move
				}
			else if ((dogSolenoid.Get() == DoubleSolenoid::kForward) && (ratchetSolenoid.Get() == DoubleSolenoid::kReverse)) // If the dog gear is engaged but the ratchet is disengaged
				{
					ratchetSolenoid.Set(DoubleSolenoid::kForward); // Engages the ratchet so that both dog gear and ratchet are engaged before shooting for safety
					Wait(0.5); // Giving the pistons time to engage properly
				}
		}
		/*** DONE LOADING THE CATAPULT ***/
		
		DriverStationLCD *screen = DriverStationLCD::GetInstance();
		float pLower = 175; // min height of rectangle for comparison
		float pUpper = 500;	// max height of rectangle for comparison
		int criteriaCount = 1; // number of elements to include/exclude at a time
		int rejectMatches = 1;	// when set to true, particles that do not meet the criteria are discarded
		int connectivity = 1;	// declares connectivity value as 1; so corners are not ignored
		int filterFunction;	// removes small blobs
		int borderSetting;	// variable to store border settings, limit for rectangle
		int borderSize = 1;  // border for the camera frame (if you don't put this, DriverStation gets mad at you)
		ParticleFilterCriteria2 particleCriteria;	
		ParticleFilterOptions2 particleFilterOptions;
		int numParticles;
		particleCriteria.parameter = IMAQ_MT_AREA; //The Morphological measurement we use
		particleCriteria.lower = pLower; // The lower bound of the criteria range
		particleCriteria.upper = pUpper; // The upper bound of the criteria range
		particleCriteria.calibrated = FALSE; // We aren't calibrating to real world measurements. We don't need this.
		particleCriteria.exclude = TRUE; // Remove all particles that aren't in specific pLower and pUpper range
		particleFilterOptions.rejectMatches = rejectMatches; // Set to 1 above, so images that do not meet the criteria are discarded
		particleFilterOptions.rejectBorder = 0; // Set to 0 over here so border images are not discarded
		particleFilterOptions.connectivity8 = connectivity; // Sets the image image to 8 bit
		while ((IsAutonomous()))
		{
			HSLImage* imgpointer; // declares an image container as an HSL (hue-saturation-luminence) image
			imgpointer = camera.GetImage();	//tells camera to capture image
			ringLight.Set(Relay::kForward); //turns ringlight on
			BinaryImage* binIMG = NULL;	// declares a container to hold a binary image
			binIMG = imgpointer -> ThresholdHSL(0, 255, 0, 255, 235, 255);	// thresholds HSL image and places in the binary image container
			delete imgpointer;	// deletes the HSL image to free up memory on the cRIO
			Image* modifiedImage = imaqCreateImage(IMAQ_IMAGE_U8, 0); //create a binary 8-bit format shell for the image
			filterFunction = imaqParticleFilter4(modifiedImage, binIMG -> GetImaqImage(), &particleCriteria, criteriaCount, &particleFilterOptions, NULL, &numParticles); //The Particle Filter Function we use. (The ones before it are outdated)
			borderSetting = imaqSetBorderSize(modifiedImage, borderSize); // Sets a border size so DriverStation is happy
			delete binIMG; //Deletes the Binary image
			int functionCountParticles; // stores number of particles
			int particleAmount; // stores the number of particles for the measure particle function
			functionCountParticles = imaqCountParticles(modifiedImage, TRUE, &particleAmount); // Counts the number of particles
			int functionOne; // The first measuring particle function (specifically for particle #1)
			int functionTwo; // The second measuring particle function (specifically for particle #2)
			double particleOneOrientation; // TRULY ARBITRARY name of the first particle it find
			double particleTwoOrientation; // TRULY ARBITRARY name of the second particle it finds
			functionOne = imaqMeasureParticle(modifiedImage, 0, FALSE, IMAQ_MT_ORIENTATION, &particleOneOrientation); // Measures orientation of particle 1
			functionTwo = imaqMeasureParticle(modifiedImage, 1, FALSE, IMAQ_MT_ORIENTATION, &particleTwoOrientation); // Measures orientation of particle 2
			screen->PrintfLine(DriverStationLCD::kUser_Line3,"W1: %f", particleOneOrientation); // Prints particle 1's orientation
			screen->PrintfLine(DriverStationLCD::kUser_Line4,"W2: %f", particleTwoOrientation); // Prints particle 2's orientation
			imaqDispose(modifiedImage); // Deletes the filtered image
			
			/**LEFT POSITION**/
			if (((leftPositionSwitch.Get()) == 0) && ((PositionSwitch.Get()) == 1)) // Left switch set on,  switch set off
			{
				screen -> PrintfLine(DriverStationLCD::kUser_Line1,"Left Position:F"); // Left position and facing forward
				if (75 < (particleOneOrientation || particleTwoOrientation) < 105) // The target should be hot. Now it goes to the other goal.
					/* Theoretically particle 1 or 2 should register as exactly 90 (the particle is vertical). We can edit these later. */
				{
					screen -> PrintfLine(DriverStationLCD::kUser_Line6,"Left Position Hot!");
					// These DEFINITELY need to be tested. All of them. Forreal.
					turn();
					driveForward();
					Wait(3); 
					stopDriving();
					autonomousCatapultRelease();
				}
				else // The target isn't hot. So it starts going toward this not hot goal.
				{
					screen -> PrintfLine(DriverStationLCD::kUser_Line6,"Left Position Not Hot");
					// These DEFINITELY need to be tested. All of them. Forreal.
					driveForward();
					Wait(3);
					stopDriving();
					autonomousCatapultRelease();
				}
			}
			/**CENTER POSITION**/
			else if (((leftPositionSwitch.Get() == 1) && (PositionSwitch.Get() == 1))||((leftPositionSwitch.Get() == 0) && (PositionSwitch.Get() == 0))) // Left switch off and  switch off
			{
				screen -> PrintfLine(DriverStationLCD::kUser_Line1,"Middle Position:R"); // Middle position and facing 
				if (75 < (particleOneOrientation || particleTwoOrientation) < 105) // The target should be hot. Now it goes to the other goal.
					/* Theoretically particle 1 or 2 should register as exactly 90 (the particle is vertical). We can edit these later. */
				{
					screen -> PrintfLine(DriverStationLCD::kUser_Line6,"Middle Position Hot");
					// These DEFINITELY need to be tested. All of them. Forreal.
					turnLeftMore();
					driveForward();
					Wait(3); 
					stopDriving();
					autonomousCatapultRelease();
				}
				else // The target isn't hot. So it starts going toward this not hot goal.
				{
					screen -> PrintfLine(DriverStationLCD::kUser_Line6,"Middle Position Not Hot");
					// These DEFINITELY need to be tested. All of them. Forreal.
					driveForward();
					Wait(3);
					stopDriving();
					autonomousCatapultRelease();
				}
			}
			/** POSITION**/
			else if (((leftPositionSwitch.Get()) == 1) && ((PositionSwitch.Get()) == 0)) // Left switch off and  switch on
			{
				screen -> PrintfLine(DriverStationLCD::kUser_Line6," Position"); //  position and facing forward
				if (75 < (particleOneOrientation || particleTwoOrientation) < 105) // The target should be hot. Now it goes to the other goal.
					/* Theoretically particle 1 or 2 should register as exactly 90 (the particle is vertical). We can edit these later. */
				{
					screen -> PrintfLine(DriverStationLCD::kUser_Line6," Position Hot");
					// These DEFINITELY need to be tested. All of them. Forreal.
					turnLeft();
					driveForward();
					Wait(3);
					stopDriving();
					autonomousCatapultRelease();
				}
				else // The target isn't hot. So it starts going toward this not hot goal.
				{
					screen -> PrintfLine(DriverStationLCD::kUser_Line6," Position Not Hot");
					// These DEFINITELY need to be tested. All of them. Forreal.
					driveForward();
					Wait(3);
					stopDriving();
					autonomousCatapultRelease();
				}
			}
		}//while loop end
		compressor.Stop();
	}//ends the autonomous
	void OperatorControl()
	{
		compressor.Start(); // The class can run independently. This compressor will start and stop on its own to keep pressure under 120 PSI
		DriverStationLCD *screen = DriverStationLCD::GetInstance(); // Enables Driver Station Printing
		bool autoLoad = false;
		while (IsOperatorControl())
		{
			/****** CONSTANT SETTINGS ******/
			retrievalMotor.Set(1);
			/****** KILL SWITCHES *******/
			
			/****** AUTO LOAD FUNCTION START *****/
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
				}
			}
			/****** AUTO LOAD FUNCTION END *****/
			
			/****** AUTO LOAD FUNCTION START *****/
			if(logitech.GetRawButton(5))
				compressor.Start(); // The class can run independently. This compressor will start and stop on its own to keep pressure under 120 PSI
			if(logitech.GetRawButton(7))
				compressor.Stop(); // Allows the compressor to stop running if the mode is switched outside of OperatorControl
			/****** AUTO LOAD FUNCTION END *****/
			
			
			/****** TANK DRIVE START ******/
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
					Front.Set((driveSpeedMultiplier) * logitech.GetRawAxis(4)); // Sets motor speed to Joystick position times the driveSpeedMultiplier
					Back.Set((driveSpeedMultiplier) * logitech.GetRawAxis(4)); // Sets motor speed to Joystick position times the driveSpeedMultiplier
				}
			else
				{
					Front.Set(0);  // Turns  Motor 2 off
					Back.Set(0);  // Turns  Motor 2 off
				}
			/****** TANK DRIVE END ******/
			
			/****** BALL RETRIEVAL START ******/ /*CHECK THE POLARITY OF YOUR SOLENOIDS*/
			if (logitech.GetRawButton(8)) 										//Press Lower  Trigger to go down and STOP ball motor
				{		
					retrievalMotor.Set(0);										// Turns on the motor that spins the wheel to bring in the ball
					leftArmSolenoid.Set(DoubleSolenoid::kReverse);				// Pushes the pneumatic piston forward to lower the retrieval arm
					rightArmSolenoidS.Set(1);				// Pushes the pneumatic piston forward to lower the retrieval arm
				}
			else if (logitech.GetRawButton(6))									//Press Upper  Trigger to go up and STOP ball motor
				{
					retrievalMotor.Set(0);										// Turns off the motor that spins the wheel to bring in the ball
					leftArmSolenoid.Set(DoubleSolenoid::kForward);				// Brings the pneumatic piston backward to raise the retrieval arm
					rightArmSolenoidS.Set(0);									// Brings the pneumatic piston backward to raise the retrieval arm
				}
			else
				{
					leftArmSolenoid.Set(DoubleSolenoid::kOff);					// Does nothing to the retreival arm piston
					//ArmSolenoid.Set(DoubleSolenoid::kOff);					// Does nothing to the retreival arm piston
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
				while(leftLimitSwitch.Get()==1 && LimitSwitch.Get()==1)
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
						Front.Set((driveSpeedMultiplier) * logitech.GetRawAxis(4)); // Sets motor speed to Joystick position times the driveSpeedMultiplier
						Back.Set((driveSpeedMultiplier) * logitech.GetRawAxis(4)); // Sets motor speed to Joystick position times the driveSpeedMultiplier
					}
					else
					{
						Front.Set(0);  // Turns  Motor 2 off
						Back.Set(0);  // Turns  Motor 2 off
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
				{
					winchMotor.Set(0); // Stops the Winch Motor since one or more buttons are pressed
					if ((dogSolenoid.Get() == DoubleSolenoid::kReverse) && (ratchetSolenoid.Get() == DoubleSolenoid::kForward)) // If the Dog Gear is disengaged but the ratchet is engaged
					{
						winchMotor.Set(0.05); // Gears need to be moving slowly to allow the dog gear to engage properly. Might want to test this since the catapult's already loaded.
						dogSolenoid.Set(DoubleSolenoid::kForward); // Engages the dog gear so both dog gear and ratchet are engaged before shooting for safety
						Wait(0.5); // Giving the pistons time to engage properly
						winchMotor.Set(0); // Now that the dog gear is engaged, the gears do not have to move
					}
					else if ((dogSolenoid.Get() == DoubleSolenoid::kForward) && (ratchetSolenoid.Get() == DoubleSolenoid::kReverse)) // If the dog gear is engaged but the ratchet is disengaged
					{
						ratchetSolenoid.Set(DoubleSolenoid::kForward); // Engages the ratchet so that both dog gear and ratchet are engaged before shooting for safety
						Wait(0.5); // Giving the pistons time to engage properly
					}
				}
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
					while(leftLimitSwitch.Get()==1 && LimitSwitch.Get()==1)
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
							Front.Set((driveSpeedMultiplier) * logitech.GetRawAxis(4)); // Sets motor speed to Joystick position times the driveSpeedMultiplier
							Back.Set((driveSpeedMultiplier) * logitech.GetRawAxis(4)); // Sets motor speed to Joystick position times the driveSpeedMultiplier
						}
						else
						{
							Front.Set(0);  // Turns  Motor 2 off
							Back.Set(0);  // Turns  Motor 2 off
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
			if ((leftLimitSwitch.Get()== 0 || LimitSwitch.Get()== 0) && winchMotor.Get() == 0 &&  logitech.GetRawButton(7))
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
			screen -> PrintfLine(DriverStationLCD::kUser_Line2,"LimSwi:%d and %d", leftLimitSwitch.Get(), LimitSwitch.Get()); // Print Left& Buttons
			screen -> PrintfLine(DriverStationLCD::kUser_Line3,"WinchMotor:%f", winchMotor.Get()); // Print WinchMotor State
			screen -> PrintfLine(DriverStationLCD::kUser_Line4,"Autoload:%s", autoLoad? "On" : "Off" ); // Print state of autoLoad
			//screen -> PrintfLine(DriverStationLCD::kUser_Line5,""); // Free Line for now
			screen -> PrintfLine(DriverStationLCD::kUser_Line6,"LDri:%f RDri:%f", leftFront.Get(), Front.Get()); // Print Tank Drive Motor State
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
			if(logitech.GetRawButton(9)) //press Back
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
					rightArmSolenoidS.Set(0);
					leftArmSolenoid.Set(DoubleSolenoid::kForward);
				}
				else if(logitech.GetRawButton(2)) //press A
				{
					rightArmSolenoidS.Set(1);
					leftArmSolenoid.Set(DoubleSolenoid::kReverse);
				}
				else
				{
					leftArmSolenoid.Set(DoubleSolenoid::kOff);
					//rightArmSolenoid.Set(DoubleSolenoid::kOff);
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
					Front.Set(logitech.GetRawAxis(2)); //Press left joystick
				}
				else
				{
					Front.Set(0);
				}
				if(logitech.GetRawButton(2)) //Press A
				{
					Back.Set(logitech.GetRawAxis(2));
				}
				else
				{
					Back.Set(0);
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
			screen -> PrintfLine(DriverStationLCD::kUser_Line2,"RF:%f RB:%f LF:%f LB:%f", Front.Get(), Back.Get(), leftFront.Get(), leftBack.Get()); // Print WinchMotor State
			screen -> PrintfLine(DriverStationLCD::kUser_Line3,"Solenoid Testing:%d", solenoidTest);
			screen -> PrintfLine(DriverStationLCD::kUser_Line4,"rightArmSolenoidS:%d", rightArmSolenoidS.Get());
			screen -> PrintfLine(DriverStationLCD::kUser_Line5,"time:%d", counter);
			counter ++;
			Wait(0.005);	// Waits to run the loop every 0.005 seconds so the cRIO doesn't explode
			screen->UpdateLCD();
	}
	}
};
START_ROBOT_CLASS(RobotDemo);
