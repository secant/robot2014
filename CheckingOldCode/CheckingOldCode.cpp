#include <WPILib.h>
//#include <stdlib.h> 
//#include <string.h>
#include <math.h>
#include <nivision.h>

class RobotDemo : public SimpleRobot
{
	RobotDrive myRobot; // robot drive system
	Joystick stick; // only joystick
	Relay backpack; // controls ringlight
	AxisCamera &camera;
public:
	RobotDemo():
		myRobot(1, 2),	// these must be initialized in the same order
		stick(1),		// as they are declared above.
		backpack(1, Relay::kBothDirections), // allows the ringlight to be turned on or off throughout the code
		camera(AxisCamera::GetInstance("10.28.53.11"))
	{
		myRobot.SetExpiration(0.1);
		//AxisCamera &camera = AxisCamera::GetInstance("10.28.53.11"); // establishes connection to camera
		camera.WriteResolution(AxisCamera::kResolution_320x240); // establishes the resolution of the camera we're using
	}

	void Autonomous()
	{		
	}
	void OperatorControl()
	{
		
		DriverStationLCD *screen = DriverStationLCD::GetInstance(); // enables printing in driverstationLCD
		int counter = 0; //troubleshoot placeholder
		
		while (IsOperatorControl())
		{
			counter++; //tracks the number of times the loop runs
			backpack.Set(Relay::kForward); //turns ringlight on
			HSLImage* imgpointer; // declares an image container as an HSL (hue-saturation-luminence) image
			imgpointer = camera.GetImage();	//tells camera to capture image
			screen->PrintfLine(DriverStationLCD::kUser_Line1,"width: %d",  imgpointer->GetWidth()); // prints out the width of the image so that we know the camera is working
			screen->PrintfLine(DriverStationLCD::kUser_Line2,"height: %d",  imgpointer->GetHeight());
			if (imgpointer->GetWidth() > 0 && imgpointer->GetHeight() > 0)
			{
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
				{
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
				}
				int ParticleCounter;	// stores number of particles
				int* countparticles; // stores the number of particles for the measure particle function
				int* countparticlesTwo; //stores the number of particles to print in driverstation
				//int PicInfo;
				//unsigned int visionInfo;
	
				
				//int paralysis;
				ParticleCounter = imaqCountParticles(Kirby, TRUE, countparticles); // Counts the number of particles to be sent off later to the MeasureParticle function. Then it gets eaten by the measureparticle function
				ParticleCounter = imaqCountParticles(KirbyTwo, TRUE, countparticlesTwo); //Counts the number of particles to be sent to the driverstation LCD
				screen->PrintfLine(DriverStationLCD::kUser_Line5,"Particles: %d", *countparticlesTwo); //Prints the number of particles found
				//screen->PrintfLine(DriverStationLCD::kUser_Line4,"%d", imaqGetLastError());
				//screen->PrintfLine(DriverStationLCD::kUser_Line5,"%d", *countparticles);
	
	
				
				//screen->PrintfLine(DriverStationLCD::kUser_Line1,"width: %d",  imgpointer->GetWidth());
							//screen->PrintfLine(DriverStationLCD::kUser_Line2,"height: %d",  imgpointer->GetHeight());
				
				//pointer = static_cast<int *>(paralysis);	//int to pointer
				int TinyRuler; // TRULY ARBITRARY name of the first measuring particle function (specifically for particle #1)
				int BabyYardstick; // TRULY ARBITRARY Name of the second measuring particle function (specifically for particle #2)
				//int TinyRulerHeight;
				//int BabyYardstickHeight;
				double* unowidth; // TRULY ARBITRARY name of the first particle it find
				double* doswidth; // TRULY ARBITRARY name of the second particle it finds

				
				//PicInfo = imaqGetVisionInfoTypes(Kirby, &visionInfo);
				//if (*countparticles > 0 && *countparticles < 3)
				TinyRuler = imaqMeasureParticle(Kirby, 0, FALSE, IMAQ_MT_BOUNDING_RECT_WIDTH, unowidth); // Function of measuring rectangle width is applied to particle 1 (unowidth)
				BabyYardstick = imaqMeasureParticle(Kirby, 1, FALSE, IMAQ_MT_BOUNDING_RECT_WIDTH, doswidth); // Function of measuring width is applied to particle 2 (doswidth)
				//TinyRulerHeight = imaqMeasureParticle(Kirby, 0, FALSE, IMAQ_MT_BOUNDING_RECT_HEIGHT, unoheight);
				//BabyYardstickHeight = imaqMeasureParticle(Kirby, 1, FALSE, IMAQ_MT_BOUNDING_RECT_HEIGHT, dosheight);
				
				
				
				
				
				//if(*unowidth > 0 && *doswidth > 0)
				{
				screen->PrintfLine(DriverStationLCD::kUser_Line3,"W1: %f",*unowidth); // Prints the applied information to particle 1. (Rectangle width)
				screen->PrintfLine(DriverStationLCD::kUser_Line4,"W2: %f",*doswidth); // Prints the applied information to particle 2. (Rectangle width)
				//screen->PrintfLine(DriverStationLCD::kUser_Line1,"H1: %f",*unoheight);
				//screen->PrintfLine(DriverStationLCD::kUser_Line2,"H2: %f",*dosheight);
				}
				/**else
				{
					screen->PrintfLine(DriverStationLCD::kUser_Line3,"Broken");
					screen->PrintfLine(DriverStationLCD::kUser_Line4,"GFrickinG");
				}
				*/
			/**	else
				{
					screen->PrintfLine(DriverStationLCD::kUser_Line3,"Too Much");
					screen->PrintfLine(DriverStationLCD::kUser_Line4,"Too Much");
					screen->UpdateLCD();
				}

				imaqDispose(KirbyTwo); 
				/**We are now done with torturing KirbyTwo and have extracted the information about the *number* of particles. It will now continue to eat up 
				  *our memory so we no longer have a use for KirbyTwo. Its services are no longer needed (until the next loop).**/
				imaqDispose(Kirby);	 
				/**We are now done with torturing Kirby and have extracted the information about the *size* of the particles. It will now continue to eat up 
				  *our memory so we no longer have a use for Kirby. Its services are no longer needed (until the next loop).**/
			}
			screen->UpdateLCD(); //Updates the Driverstation printing information.
		}
	}
	
	/**
	 * Runs during test mode
	 */
	void Test() {

	}
};

START_ROBOT_CLASS(RobotDemo);




