// System time function for testing
#include "WPILib.h"	
#include "Math.h"
//Last modified on Feb. 13 by: Sean
class RobotDemo : public SimpleRobot
{
	/*HEY DRIVERS LOOK AT THIS*/
	/*HEY DRIVERS LOOK AT THIS*/
	/*HEY DRIVERS LOOK AT THIS*/
	/*IF YOU WANT IT TO DRIVE FASTER OR SLOWER, EDIT THE "driverSpeedMultiplier". IT CAN GO ANYWHERE FROM 0<X<1*/
	/*IF YOU WANT TO CHANGE WHEN THE ROBOT MOVES AFTER A SMALL INCREMENT IS MOVED ON THE JOYSTICK, EDIT THE "driveStickBuffer". I RECCOMEND IT BEING 0<X<0.5*/
	/*EXAMPLE MODIFICATION:*/
	// const static float driveSpeedMultiplier = 0.75;
	const static float driveSpeedMultiplier = 0.50;
	/*EXAMPLE MODIFICATION:*/
	//const static float driveStickBuffer = 0.10;
	const static float driveStickBuffer = 0.25;
	/*HEY DRIVERS LOOK AT THIS*/
	/*HEY DRIVERS LOOK AT THIS*/
	/*HEY DRIVERS LOOK AT THIS*/
	Talon leftFront, leftBack, rightFront, rightBack;
	Joystick logitech; // Catapult Limit Switch
	DigitalInput buttonOne; // Catapult Limit Switch
	DigitalInput buttonTwo; // VehiclePosition
	DigitalInput togglebuttonOne; //Vehicle Position
	DigitalInput togglebuttonTwo;
	Compressor compressor;
	DoubleSolenoid armSolenoidOne;
	DoubleSolenoid armSolenoidTwo;
	Relay emergencyCompressor;
	Talon catapultMotor;
	DoubleSolenoid dogSolenoid;
	DoubleSolenoid ratchetSolenoid;
	Relay backpack; // controls ringlight
	AxisCamera &camera;
public:
	RobotDemo():
		leftFront(1), leftBack(3), rightFront(2), rightBack(4), 
		logitech(1),
		buttonOne(8), buttonTwo(9),
		togglebuttonOne(6), //Left
		togglebuttonTwo(7), //Right
		compressor(5,2),
		armSolenoidOne(7,8),
		armSolenoidTwo(5,6),
		emergencyCompressor(2),
		catapultMotor(5),
		dogSolenoid(1,2),
		ratchetSolenoid(3,4),
		backpack(1, Relay::kBothDirections), // allows the ringlight to be turned on or off throughout the code
		camera(AxisCamera::GetInstance("10.28.53.11"))
	{	
		camera.WriteResolution(AxisCamera::kResolution_320x240); // establishes the resolution of the camera we're using
	}
	void driveForward() //Creates a function to drive forward (sets it)
	{
		leftFront.Set(0.75);
		leftBack.Set(0.75);
		rightFront.Set(0.75);
		rightBack.Set(0.75);
	}
	void turnRight() //Creates a function to turn the robot a set amount right and then stops movement
	{
		leftFront.Set(0.5);
		leftBack.Set(0.5);
		rightFront.Set(-0.5);
		rightBack.Set(-0.5);
		Wait(1.5);
		leftFront.Set(0);
		leftBack.Set(0);
		rightFront.Set(0);
		rightBack.Set(0);
	}
	void turnLeft() //Creates a function to turn the robot a set amount left and then stops movement
	{
		leftFront.Set(-0.5);
		leftBack.Set(-0.5);
		rightFront.Set(0.5);
		rightBack.Set(0.5);
		Wait(1.5);
		leftFront.Set(0);
		leftBack.Set(0);
		rightFront.Set(0);
		rightFront.Set(0);
	}
	void turnLeftMore() //Creates a function to turn the robot a larger set amount left and then stops movement.
	{
		leftFront.Set(-1);
		leftBack.Set(-1);
		rightFront.Set(1);
		rightFront.Set(1);
		Wait(1.5);
		leftFront.Set(0);
		leftBack.Set(0);
		rightFront.Set(0);
		rightBack.Set(0);
	}
	void turnRightMore() //Creates a function to turn the robot a larger set amount right and then stops movement.
	{
		leftFront.Set(1);
		leftBack.Set(1);
		rightFront.Set(-1);
		rightFront.Set(-1);
		Wait(1.5);
		leftFront.Set(0);
		leftBack.Set(0);
		rightFront.Set(0);
		rightBack.Set(0);
	}
	void driveBackward() //Creates a function to drive backward (sets it)
	{
		leftFront.Set(-0.75);
		leftBack.Set(-0.75);
		rightFront.Set(-0.75);
		rightBack.Set(-0.75);
	}
	void stopDriving()
	{
		leftFront.Set(0);
		leftBack.Set(0);
		rightFront.Set(0);
		rightBack.Set(0);
	}
	void loadCatapult()
	{
		if (buttonOne.Get()==1 && buttonTwo.Get()==1 && dogSolenoid.Get()==DoubleSolenoid::kReverse)
		{
			dogSolenoid.Set(DoubleSolenoid::kForward);
			Wait(0.5);
			ratchetSolenoid.Set(DoubleSolenoid::kForward);
			Wait(0.5);
			catapultMotor.Set(1);
		}
	}
	void shootCatapult()
	{
		if (logitech.GetRawButton(2) && buttonOne.Get()==0 && buttonTwo.Get()==0)
		{
			dogSolenoid.Set(DoubleSolenoid::kReverse);
			Wait(0.5);
			ratchetSolenoid.Set(DoubleSolenoid::kReverse);
			Wait(1);
		}
	}
	void Autonomous()
	{
		DriverStationLCD *screen = DriverStationLCD::GetInstance(); 
		while ((IsAutonomous()))
		{
			HSLImage* imgpointer; // declares an image container as an HSL (hue-saturation-luminence) image
			imgpointer = camera.GetImage();	//tells camera to capture image
			backpack.Set(Relay::kForward); //turns ringlight on
			BinaryImage* binIMG = NULL;	// declares a container to hold a binary image
			binIMG = imgpointer -> ThresholdHSL(0, 255, 0, 255, 235, 255);	// thresholds HSL image and places in the binary image container
			delete imgpointer;	// deletes the HSL image to free up memory on the cRIO
			Image* Kirby = imaqCreateImage(IMAQ_IMAGE_U8, 0); //create 8 bit image
			Image* KirbyTwo = imaqCreateImage(IMAQ_IMAGE_U8, 0); // creates the second 8-bit image that we can use separately for counting particles. 
																 // (The first image gets eaten by the measureparticle function)
			float pLower = 175; // min height of rectangle for comparison
			float pUpper = 500;	// max height of rectangle for comparison
			int criteriaCount = 1; // number of elements to include/exclude at a time
			int rejectMatches = 1;	// when set to true, particles that do not meet the criteria are discarded
			int connectivity = 1;	// declares connectivity value as 1; so corners are not ignored
			int Polturgust3000;	// removes small blobs
			int borderSetting;	// variable to store border settings, limit for rectangle
			int cloningDevice; // we create another image because the ParticleMeasuring steals the image from particlecounter
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
			
			Polturgust3000 = imaqParticleFilter4(Kirby, binIMG -> GetImaqImage(), &particleCriteria, criteriaCount, &particleFilterOptions, NULL, &numParticles); //The Particle Filter Function we use. (The ones before it are outdated)
			borderSetting = imaqSetBorderSize(Kirby, borderSize); // Sets a border size
			cloningDevice =  imaqDuplicate(KirbyTwo, Kirby); //Officially creating a duplicate of the first image to count the number of particles.
			delete binIMG; //Deletes the Binary image
			int ParticleCounter;	// stores number of particles
			int* countparticles; // stores the number of particles for the measure particle function

			ParticleCounter = imaqCountParticles(Kirby, TRUE, countparticles); // Counts the number of particles to be sent off later to the MeasureParticle function. Then it gets eaten by the measureparticle function
			int TinyRuler; // TRULY ARBITRARY name of the first measuring particle function (specifically for particle #1)
			int BabyYardstick; // TRULY ARBITRARY Name of the second measuring particle function (specifically for particle #2)
			double* unowidth; // TRULY ARBITRARY name of the first particle it find
			double* doswidth; // TRULY ARBITRARY name of the second particle it finds

			TinyRuler = imaqMeasureParticle(Kirby, 0, FALSE, IMAQ_MT_BOUNDING_RECT_WIDTH, unowidth); // Function of measuring rectangle width is applied to particle 1 (unowidth)
			BabyYardstick = imaqMeasureParticle(Kirby, 1, FALSE, IMAQ_MT_BOUNDING_RECT_WIDTH, doswidth); // Function of measuring width is applied to particle 2 (doswidth)

			
			screen->PrintfLine(DriverStationLCD::kUser_Line3,"W1: %f",*unowidth); // Prints the applied information to particle 1. (Rectangle width)
			screen->PrintfLine(DriverStationLCD::kUser_Line4,"W2: %f",*doswidth);
			imaqDispose(Kirby);
			imaqDispose(KirbyTwo);
			if (((togglebuttonOne.Get()) == 0) && ((togglebuttonTwo.Get()) == 1))
			{
				screen -> PrintfLine(DriverStationLCD::kUser_Line1,"Left Position");
				if (*unowidth > 20) // The target should be hot. Now it goes to the other goal.
					// Even this needs to be tested
				{	
					screen -> PrintfLine(DriverStationLCD::kUser_Line6,"Left Position Hot");
					// These DEFINITELY need to be tested. All of them. Forreal.
					turnRight();
					driveForward();
					Wait(3); 
					stopDriving();
					shootCatapult();
				}
				else // The target isn't hot. So it starts going toward this not hot goal.
				{
					screen -> PrintfLine(DriverStationLCD::kUser_Line6,"Left Position Not Hot");
					// These DEFINITELY need to be tested. All of them. Forreal.
					driveForward();
					Wait(3);
					stopDriving();
					shootCatapult();
				}
			}
			//both on
			else if (((togglebuttonOne.Get()) == 1) && ((togglebuttonTwo.Get()) == 1))
			{
				screen -> PrintfLine(DriverStationLCD::kUser_Line1,"Middle Position");
				if (*unowidth > 20) // The target should be hot. Now it goes to the other goal.
				{
					screen -> PrintfLine(DriverStationLCD::kUser_Line6,"Middle Position Hot");
					// These DEFINITELY need to be tested. All of them. Forreal.
					turnLeftMore();
					driveForward();
					Wait(3); 
					stopDriving();
					shootCatapult();
				}
				else if (((togglebuttonOne.Get()) == 0) && ((togglebuttonTwo.Get()) == 0))
				{
					screen -> PrintfLine(DriverStationLCD::kUser_Line1,"Middle Position");
					if (*unowidth > 20) // The target should be hot. Now it goes to the other goal.
					{
						screen -> PrintfLine(DriverStationLCD::kUser_Line6,"Middle Position Hot");
						// These DEFINITELY need to be tested. All of them. Forreal.
						turnRightMore();
						driveForward();
						Wait(3); 
						stopDriving();
						shootCatapult();
					}
					else // The target isn't hot. So it starts going toward this not hot goal.
					{
						screen -> PrintfLine(DriverStationLCD::kUser_Line6,"Middle Position Not Hot");
						// These DEFINITELY need to be tested. All of them. Forreal.
						driveForward();
						Wait(3);
						stopDriving();
						shootCatapult();
						driveForward();
						Wait(3);
						stopDriving();
					}
				}
				//Left button on && right off
				else if (((togglebuttonOne.Get()) == 1) && ((togglebuttonTwo.Get()) == 0))
				{
					screen -> PrintfLine(DriverStationLCD::kUser_Line6,"Right Position");
					if (*unowidth > 20) // The target should be hot. Now it goes to the other goal.
					{
						screen -> PrintfLine(DriverStationLCD::kUser_Line6,"Right Position Hot");
						// These DEFINITELY need to be tested. All of them. Forreal.
						turnLeft();
						driveForward();
						Wait(3); 
						stopDriving();
						shootCatapult();
					}
					else // The target isn't hot. So it starts going toward this not hot goal.
					{
						screen -> PrintfLine(DriverStationLCD::kUser_Line6,"Right Position Not Hot");
						// These DEFINITELY need to be tested. All of them. Forreal.
						driveForward();
						Wait(3);
						stopDriving();
						shootCatapult();
					}
				}
			Wait(0.005);
			screen -> UpdateLCD();
			}
		}
	}
	void OperatorControl()
	{
		compressor.Start();
		DriverStationLCD *screen = DriverStationLCD::GetInstance(); 
		while (IsOperatorControl())
		{
			screen -> PrintfLine(DriverStationLCD::kUser_Line1,"LeftTopM1_%f", logitech.GetRawAxis(2));
			screen -> PrintfLine(DriverStationLCD::kUser_Line2,"LeftBackM2_%f", logitech.GetRawAxis(2));
			screen -> PrintfLine(DriverStationLCD::kUser_Line3,"RightTopM1_%f", logitech.GetRawAxis(4));
			//screen -> PrintfLine(DriverStationLCD::kUser_Line4,"RightTopM2_%f", logitech.GetRawAxis(4));
			//screen -> PrintfLine(DriverStationLCD::kUser_Line5,"Button_%d", buttonOne.Get());
			//screen -> PrintfLine(DriverStationLCD::kUser_Line6,"toggle_%d", togglebuttonOne.Get());
			
			/** CATAPULT **/
			loadCatapult();
			shootCatapult(); //Press A to shoot
			
			/** RETRIEVAL **/
			if(logitech.GetRawButton(6)) //Press Upper Right Trigger to go down
			{		
				armSolenoidOne.Set(DoubleSolenoid::kForward);
				armSolenoidTwo.Set(DoubleSolenoid::kForward);
			}
			else if(logitech.GetRawButton(8)) //Press Lower Right Trigger to go up
			{
				armSolenoidOne.Set(DoubleSolenoid::kReverse);
				armSolenoidTwo.Set(DoubleSolenoid::kReverse);
			}
			else
			{
				armSolenoidOne.Set(DoubleSolenoid::kOff);
				armSolenoidTwo.Set(DoubleSolenoid::kOff);
			}
			
			/**DRIVING**/
			if (fabs(logitech.GetRawAxis(2)) > driveStickBuffer)
			{
				leftFront.Set((driveSpeedMultiplier) * logitech.GetRawAxis(2)*(-1));
				leftBack.Set((driveSpeedMultiplier) * logitech.GetRawAxis(2)*(-1));
			}		
			else
			{
				leftFront.Set(0);
				leftBack.Set(0);
			}
			if (fabs(logitech.GetRawAxis(4)) > driveStickBuffer)
			{
				rightFront.Set((driveSpeedMultiplier) * logitech.GetRawAxis(4));
				rightBack.Set((driveSpeedMultiplier) * logitech.GetRawAxis(4));
			}
			else
			{
				rightFront.Set(0);
				rightBack.Set(0);
			}
// END TANK DRIVE
			/***********************/
			if ((buttonOne.Get()) == 1)
			{
				screen -> PrintfLine(DriverStationLCD::kUser_Line1,"Button1_Not Pressed!");
			}
			else
			{
				screen -> PrintfLine(DriverStationLCD::kUser_Line1,"Button1_Pressed!");
			}
			if ((buttonTwo.Get()) == 1)
			{
				screen -> PrintfLine(DriverStationLCD::kUser_Line2,"Button2_Not Pressed!");
			}
			else
			{
				screen -> PrintfLine(DriverStationLCD::kUser_Line2,"Button2_Pressed!");
			}
			if (((togglebuttonOne.Get()) == 1) && ((togglebuttonTwo.Get()) == 0))
			{
				screen -> PrintfLine(DriverStationLCD::kUser_Line3,"Right Position");
			}
			//both on or off
			else if ((((togglebuttonOne.Get()) == 1) && ((togglebuttonTwo.Get()) == 1))||((togglebuttonOne.Get()) == 0) && ((togglebuttonTwo.Get()) == 0))
			{
				screen -> PrintfLine(DriverStationLCD::kUser_Line3,"Middle Position");
			}
			//Left button on && right off
			else if (((togglebuttonOne.Get()) == 0) && ((togglebuttonTwo.Get()) == 1))
			{
				screen -> PrintfLine(DriverStationLCD::kUser_Line3,"Left Position");
			}
			
			/***********************/
			screen -> PrintfLine(DriverStationLCD::kUser_Line4,"Pressure:%f", compressor.GetPressureSwitchValue());
			screen -> PrintfLine(DriverStationLCD::kUser_Line5,"Left_%f Right_&f", leftFront.Get(), rightFront.Get());
			screen -> PrintfLine(DriverStationLCD::kUser_Line6,"But6_%d But8_%d", logitech.GetRawButton(6), logitech.GetRawButton(8));
			Wait(0.005);
			screen -> UpdateLCD();
		}
		compressor.Stop();
	}
	void Test() 
	{
		DriverStationLCD *screen = DriverStationLCD::GetInstance();
		while (IsTest())
		{
			if ((buttonOne.Get()) == 1)
			{
				screen -> PrintfLine(DriverStationLCD::kUser_Line1,"Button1_Not Pressed!");
			}
			else
			{
				screen -> PrintfLine(DriverStationLCD::kUser_Line1,"Button1_Pressed!");
			}
			if ((buttonTwo.Get()) == 1)
			{
				screen -> PrintfLine(DriverStationLCD::kUser_Line2,"Button2_Not Pressed!");
			}
			else
			{
				screen -> PrintfLine(DriverStationLCD::kUser_Line2,"Button2_Pressed!");
			}
			if ((togglebuttonOne.Get()) == 0)
			{
				screen -> PrintfLine(DriverStationLCD::kUser_Line3,"Left Is_On!");
			}
			else
			{
				screen -> PrintfLine(DriverStationLCD::kUser_Line3,"Left Is_Off!");
			}
			if ((togglebuttonTwo.Get()) == 0)
			{
				screen -> PrintfLine(DriverStationLCD::kUser_Line4,"Right Is_On!");
			}
			else
			{
				screen -> PrintfLine(DriverStationLCD::kUser_Line4,"Right Is_Off!");
			}
			if (buttonOne.Get() == 0)
			{
				emergencyCompressor.Set(Relay::kOff);
			}
			
#if 0
		screen -> PrintfLine(DriverStationLCD::kUser_Line1,"ButtonOneIs_%d", buttonOne.Get());
		screen -> PrintfLine(DriverStationLCD::kUser_Line2,"toggle_%d", buttonTwo.Get());
		screen -> PrintfLine(DriverStationLCD::kUser_Line3,"Button_%d", togglebuttonOne.Get());
		screen -> PrintfLine(DriverStationLCD::kUser_Line4,"toggle_%d", togglebuttonTwo.Get());
#endif
		Wait(0.005);
		screen -> UpdateLCD();
		}
	}
};
START_ROBOT_CLASS(RobotDemo);
