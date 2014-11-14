 
//**************************************************************************
//* WARNING: This file was automatically generated.  Any changes you make  *
//*          to this file will be lost if you generate the file again.     *
//**************************************************************************
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <nivision.h>
//#include <nimachinevision.h>
//#include <windows.h>
#include <stdio.h>
//typedef bool BOOL;

// If you call Machine Vision functions in your script, add NIMachineVision.c to the project.

#define IVA_MAX_BUFFERS 10
#define IVA_STORE_RESULT_NAMES

#define VisionErrChk(Function) {if (!(Function)) {success = 0; goto Error;}}

typedef enum IVA_ResultType_Enum {IVA_NUMERIC, IVA_BOOLEAN, IVA_STRING} IVA_ResultType;

typedef union IVA_ResultValue_Struct    // A result in Vision Assistant can be of type double, BOOL or string.
{
    double numVal;
    BOOL   boolVal;
    char*  strVal;
} IVA_ResultValue;

typedef struct IVA_Result_Struct
{
#if defined (IVA_STORE_RESULT_NAMES)
    char resultName[256];           // Result name
#endif
    IVA_ResultType  type;           // Result type
    IVA_ResultValue resultVal;      // Result value
} IVA_Result;

typedef struct IVA_StepResultsStruct
{
#if defined (IVA_STORE_RESULT_NAMES)
    char stepName[256];             // Step name
#endif
    int         numResults;         // number of results created by the step
    IVA_Result* results;            // array of results
} IVA_StepResults;

typedef struct IVA_Data_Struct
{
    Image* buffers[IVA_MAX_BUFFERS];            // Vision Assistant Image Buffers
    IVA_StepResults* stepResults;              // Array of step results
    int numSteps;                               // Number of steps allocated in the stepResults array
    CoordinateSystem *baseCoordinateSystems;    // Base Coordinate Systems
    CoordinateSystem *MeasurementSystems;       // Measurement Coordinate Systems
    int numCoordSys;                            // Number of coordinate systems
} IVA_Data;



static IVA_Data* IVA_InitData(int numSteps, int numCoordSys);
static int IVA_DisposeData(IVA_Data* ivaData);
static int IVA_DisposeStepResults(IVA_Data* ivaData, int stepIndex);
static int IVA_CLRThreshold(Image* image, int min1, int max1, int min2, int max2, int min3, int max3, int colorMode);
static int IVA_ParticleFilter(Image* image,
                                       int pParameter[],
                                       float plower[],
                                       float pUpper[],
                                       int pCalibrated[],
                                       int pExclude[],
                                       int criteriaCount,
                                       int rejectMatches,
                                       int connectivity);
static int IVA_AdjustDynamic(Image* image, ImageType imageType);
static int IVA_DetectRectangles(Image* image,
                                         IVA_Data* ivaData,
                                         double minWidth,
                                         double maxWidth,
                                         double minHeight,
                                         double maxHeight, 
                                         int extraction,
                                         int curveThreshold,
                                         int edgeFilterSize,
                                         int curveMinLength,
                                         int curveRowStepSize,
                                         int curveColumnStepSize,
                                         int curveMaxEndPointGap,
                                         int matchMode, 
                                         float rangeMin[],
                                         float rangeMax[],
                                         float score,
                                         ROI* roi,
                                         int stepIndex);

int IVA_ProcessImage(Image *image)
{
	int success = 1;
    IVA_Data *ivaData;
    int pParameter[1] = {35};
    float plower[1] = {0};
    float pUpper[1] = {300};
    int pCalibrated[1] = {0};
    int pExclude[1] = {0};
    int pKernel[9] = {1,1,1,1,1,1,1,1,1};
    StructuringElement structElem;
    int i;
    ROI *roi;
    float rangeMin[3] = {0,0,1};
    float rangeMax[3] = {360,0,1000};

    // Initializes internal data (buffers and array of points for caliper measurements)
    VisionErrChk(ivaData = IVA_InitData(6, 0));

	VisionErrChk(IVA_CLRThreshold(image, 0, 255, 0, 255, 194, 255, IMAQ_HSI));

	VisionErrChk(IVA_ParticleFilter(image, pParameter, plower, pUpper, 
		pCalibrated, pExclude, 1, TRUE, TRUE));

    //-------------------------------------------------------------------//
    //                          Basic Morphology                         //
    //-------------------------------------------------------------------//

    // Sets the structuring element.
    structElem.matrixCols = 3;
    structElem.matrixRows = 3;
    structElem.hexa = FALSE;
    structElem.kernel = pKernel;

    // Applies multiple morphological transformation to the binary image.
    for (i = 0 ; i < 35 ; i++)
    {
        VisionErrChk(imaqMorphology(image, image, IMAQ_DILATE, &structElem));
    }

    //-------------------------------------------------------------------//
    //                          Conversion: Cast                         //
    //-------------------------------------------------------------------//

    // Changes the type of the image.
    VisionErrChk(imaqCast(image, image, (ImageType)7, NULL, 0));

	VisionErrChk(IVA_AdjustDynamic(image, IMAQ_IMAGE_U8));

    // Creates a new, empty region of interest.
    VisionErrChk(roi = imaqCreateROI());

    // Creates a new rectangle ROI contour and adds the rectangle to the provided ROI.
    VisionErrChk(imaqAddRectContour(roi, imaqMakeRect(0, 0, 480, 640)));

    //-------------------------------------------------------------------//
    //                            Detect Rectangles                      //
    //-------------------------------------------------------------------//

	VisionErrChk(IVA_DetectRectangles(image, ivaData, 10, 200, 10, 100, 
		IMAQ_NORMAL_IMAGE, 75, IMAQ_NORMAL, 25, 15, 15, 10, 3, rangeMin, 
		rangeMax, 300, roi, 5));

    // Cleans up resources associated with the object
    imaqDispose(roi);

    // Releases the memory allocated in the IVA_Data structure.
    IVA_DisposeData(ivaData);

Error:
	return success;
}

////////////////////////////////////////////////////////////////////////////////
//
// Function Name: IVA_CLRThreshold
//
// Description  : Thresholds a color image.
//
// Parameters   : image      -  Input image
//                min1       -  Minimum range for the first plane
//                max1       -  Maximum range for the first plane
//                min2       -  Minimum range for the second plane
//                max2       -  Maximum range for the second plane
//                min3       -  Minimum range for the third plane
//                max3       -  Maximum range for the third plane
//                colorMode  -  Color space in which to perform the threshold
//
// Return Value : success
//
////////////////////////////////////////////////////////////////////////////////
static int IVA_CLRThreshold(Image* image, int min1, int max1, int min2, int max2, int min3, int max3, int colorMode)
{
    int success = 1;
    Image* thresholdImage;
    Range plane1Range;
    Range plane2Range;
    Range plane3Range;


    //-------------------------------------------------------------------//
    //                          Color Threshold                          //
    //-------------------------------------------------------------------//

    // Creates an 8 bit image for the thresholded image.
    VisionErrChk(thresholdImage = imaqCreateImage(IMAQ_IMAGE_U8, 7));

    // Set the threshold range for the 3 planes.
    plane1Range.minValue = min1;
    plane1Range.maxValue = max1;
    plane2Range.minValue = min2;
    plane2Range.maxValue = max2;
    plane3Range.minValue = min3;
    plane3Range.maxValue = max3;

    // Thresholds the color image.
    VisionErrChk(imaqColorThreshold(thresholdImage, image, 1, (ColorMode)colorMode, &plane1Range, &plane2Range, &plane3Range));

    // Copies the threshold image in the souce image.
    VisionErrChk(imaqDuplicate(image, thresholdImage));

Error:
    imaqDispose(thresholdImage);

    return success;
}


////////////////////////////////////////////////////////////////////////////////
//
// Function Name: IVA_ParticleFilter
//
// Description  : Filters particles based on their morphological measurements.
//
// Parameters   : image          -  Input image
//                pParameter     -  Morphological measurement that the function
//                                  uses for filtering.
//                plower         -  Lower bound of the criteria range.
//                pUpper         -  Upper bound of the criteria range.
//                pCalibrated    -  Whether to take a calibrated measurement or not.
//                pExclude       -  TRUE indicates that a match occurs when the
//                                  value is outside the criteria range.
//                criteriaCount  -  number of particle filter criteria.
//                rejectMatches  -  Set this parameter to TRUE to transfer only
//                                  those particles that do not meet all the criteria.
//                                  Set this parameter to FALSE to transfer only those
//                                  particles that meet all the criteria to the destination.
//                connectivity   -  Set this parameter to 1 to use connectivity-8
//                                  to determine whether particles are touching.
//                                  Set this parameter to 0 to use connectivity-4
//                                  to determine whether particles are touching.
//
// Return Value : success
//
////////////////////////////////////////////////////////////////////////////////
static int IVA_ParticleFilter(Image* image,
                                       int pParameter[],
                                       float plower[],
                                       float pUpper[],
                                       int pCalibrated[],
                                       int pExclude[],
                                       int criteriaCount,
                                       int rejectMatches,
                                       int connectivity)
{
    int success = 1;
    ParticleFilterCriteria2* particleCriteria = NULL;
    int i;
    ParticleFilterOptions particleFilterOptions;
    int numParticles;


    //-------------------------------------------------------------------//
    //                          Particle Filter                          //
    //-------------------------------------------------------------------//

    if (criteriaCount > 0)
    {
        // Fill in the ParticleFilterCriteria2 structure.
        particleCriteria = (ParticleFilterCriteria2*)malloc(criteriaCount * sizeof(ParticleFilterCriteria2));

        for (i = 0 ; i < criteriaCount ; i++)
        {
            particleCriteria[i].parameter = (MeasurementType)pParameter[i];
            particleCriteria[i].lower = plower[i];
            particleCriteria[i].upper = pUpper[i];
            particleCriteria[i].calibrated = pCalibrated[i];
            particleCriteria[i].exclude = pExclude[i];
        }
        
        particleFilterOptions.rejectMatches = rejectMatches;
        particleFilterOptions.rejectBorder = 0;
        particleFilterOptions.connectivity8 = connectivity;
        
        // Filters particles based on their morphological measurements.
        VisionErrChk(imaqParticleFilter3(image, image, particleCriteria, criteriaCount, &particleFilterOptions, NULL, &numParticles));
    }

Error:
    free(particleCriteria);

    return success;
}


////////////////////////////////////////////////////////////////////////////////
//
// Function Name: IVA_AdjustDynamic
//
// Description  : Changes the type of an image and adjusts its dynamics.
//
// Parameters   : image      -  input image
//                imageType  -  New type for the image.
//
// Return Value : On success, this function returns a non-zero value.
//                On failure, this function returns 0.
//                To get extended error information, call imaqGetLastError().
//
////////////////////////////////////////////////////////////////////////////////
static int IVA_AdjustDynamic(Image* image, ImageType imageType)
{
    int success = 1;
    HistogramReport* histogram = NULL;
    PixelValue pixelValue;
	float maxValue;

    // Changes the type of the image to single.
    VisionErrChk(imaqCast(image, image, IMAQ_IMAGE_SGL, NULL, 0));

    // Calculates the histogram, or pixel distribution, of an image.
    VisionErrChk(histogram = imaqHistogram(image, 256, 0, 0, NULL));

    // Gets the minimum value of the pixels in the image.
    pixelValue.grayscale = histogram->min;

    // Subtract the minimum value to all the pixels of the image.
    VisionErrChk(imaqSubtractConstant(image, image, pixelValue));

    // Computes the coefficient to expand the dynamic of the image.
	switch (imageType)
	{
		case IMAQ_IMAGE_I16:
			maxValue = 32767.0;
			break;
		case IMAQ_IMAGE_U16:
			maxValue = 65535.0;
			break;
		default:
			maxValue = 255;
			break;
	}

    pixelValue.grayscale = maxValue / (histogram->max - histogram->min);

    // Expands the dynamic of the image.
    VisionErrChk(imaqMultiplyConstant(image, image, pixelValue));

    // Converts the image to the expected type.
    VisionErrChk(imaqCast(image, image, imageType, NULL, 0));

Error:
    imaqDispose(histogram);

    return success;
}


////////////////////////////////////////////////////////////////////////////////
//
// Function Name: IVA_DetectRectangles
//
// Description  : Searches for rectangles in an image that are within a given range
//
// Parameters   : image                -  Input image
//                ivaData              -  Internal Data structure
//                minWidth             -  Minimum Width
//                maxWidth             -  Maximum Width
//                minHeight            -  Minimum Height
//                maxHeight            -  Maximum Height
//                extraction           -  Extraction mode
//                curveThreshold       -  Specifies the minimum contrast at a
//                                        pixel for it to be considered as part
//                                        of a curve.
//                edgeFilterSize       -  Specifies the width of the edge filter
//                                        the function uses to identify curves in
//                                        the image.
//                curveMinLength       -  Specifies the smallest curve the
//                                        function will identify as a curve.
//                curveRowStepSize     -  Specifies the distance, in the x direction
//                                        between two pixels the function inspects
//                                        for curve seed points.
//                curveColumnStepSize  -  Specifies the distance, in the y direction,
//                                        between two pixels the function inspects
//                                        for curve seed points.
//                curveMaxEndPointGap  -  Specifies the maximum gap, in pixels,
//                                        between the endpoints of a curve that the
//                                        function identifies as a closed curve.
//                matchMode            -  Specifies the method to use when looking
//                                        for the pattern in the image.
//                rangeMin             -  Match constraints range min array
//                                        (angle 1, angle 2, scale, occlusion)
//                rangeMax             -  Match constraints range max array
//                                        (angle 1, angle 2, scale, occlusion)
//                score                -  Minimum score a match can have for the
//                                        function to consider the match valid.
//                roi                  -  Search area
//                stepIndex            -  Step index (index at which to store the results in the resuts array)
//
// Return Value : success
//
////////////////////////////////////////////////////////////////////////////////
static int IVA_DetectRectangles(Image* image,
                                         IVA_Data* ivaData,
                                         double minWidth,
                                         double maxWidth,
                                         double minHeight,
                                         double maxHeight, 
                                         int extraction,
                                         int curveThreshold,
                                         int edgeFilterSize,
                                         int curveMinLength,
                                         int curveRowStepSize,
                                         int curveColumnStepSize,
                                         int curveMaxEndPointGap,
                                         int matchMode, 
                                         float rangeMin[],
                                         float rangeMax[],
                                         float score,
                                         ROI* roi,
                                         int stepIndex)
{
    int success = 1;
    RectangleDescriptor rectangleDescriptor;     
    CurveOptions curveOptions;
    ShapeDetectionOptions shapeOptions; 
    RangeFloat orientationRange[2]; 
    int i;
    RectangleMatch* matchedRectangles = NULL; 
    int numMatchesFound;
    int numObjectResults;
    IVA_Result* shapeMacthingResults;
    unsigned int visionInfo;
    TransformReport* realWorldPosition = NULL;
    float calibratedWidth;
    float calibratedHeight;


    //-------------------------------------------------------------------//
    //                        Detect Rectangles                          //
    //-------------------------------------------------------------------//

    // Fill in the Curve options.
    curveOptions.extractionMode = (ExtractionMode)extraction;
    curveOptions.threshold = curveThreshold;
    curveOptions.filterSize = (EdgeFilterSize)edgeFilterSize;
    curveOptions.minLength = curveMinLength;
    curveOptions.rowStepSize = curveRowStepSize;
    curveOptions.columnStepSize = curveColumnStepSize;
    curveOptions.maxEndPointGap = curveMaxEndPointGap;
    curveOptions.onlyClosed = 0;
    curveOptions.subpixelAccuracy = 0;

    rectangleDescriptor.minWidth = minWidth;
    rectangleDescriptor.maxWidth = maxWidth;
    rectangleDescriptor.minHeight = minHeight;
    rectangleDescriptor.maxHeight = maxHeight;

    for (i = 0 ; i < 2 ; i++)
    {
        orientationRange[i].minValue = rangeMin[i];
        orientationRange[i].maxValue = rangeMax[i];
    }

    shapeOptions.mode = matchMode;
    shapeOptions.angleRanges = orientationRange;
    shapeOptions.numAngleRanges = 2;
    shapeOptions.scaleRange.minValue = rangeMin[2];
    shapeOptions.scaleRange.maxValue = rangeMax[2];
    shapeOptions.minMatchScore = score;

    matchedRectangles = NULL;
    numMatchesFound = 0;

    // Searches for rectangles in the image that are within the range.
    VisionErrChk(matchedRectangles = imaqDetectRectangles(image, &rectangleDescriptor, &curveOptions, &shapeOptions, roi, &numMatchesFound));

    // ////////////////////////////////////////
    // Store the results in the data structure.
    // ////////////////////////////////////////
    
    // First, delete all the results of this step (from a previous iteration)
    IVA_DisposeStepResults(ivaData, stepIndex);

    // Check if the image is calibrated.
    VisionErrChk(imaqGetVisionInfoTypes(image, &visionInfo));

    // If the image is calibrated, we also need to log the calibrated position (x and y) -> 22 results instead of 12
    numObjectResults = (visionInfo & IMAQ_VISIONINFO_CALIBRATION ? 22 : 12);

    ivaData->stepResults[stepIndex].numResults = numMatchesFound * numObjectResults + 1;
    ivaData->stepResults[stepIndex].results = (IVA_Result*)malloc (sizeof(IVA_Result) * ivaData->stepResults[stepIndex].numResults);
    shapeMacthingResults = ivaData->stepResults[stepIndex].results;
    
    if (shapeMacthingResults)
    {
        #if defined (IVA_STORE_RESULT_NAMES)
            sprintf(shapeMacthingResults->resultName, "# Matches");
        #endif
        shapeMacthingResults->type = IVA_NUMERIC;
        shapeMacthingResults->resultVal.numVal = numMatchesFound;
        shapeMacthingResults++;
        
        for (i = 0 ; i < numMatchesFound ; i++)
        {
            #if defined (IVA_STORE_RESULT_NAMES)
                sprintf(shapeMacthingResults->resultName, "Rectangle %d.Score", i + 1);
            #endif
            shapeMacthingResults->type = IVA_NUMERIC;
            shapeMacthingResults->resultVal.numVal = matchedRectangles[i].score;
            shapeMacthingResults++;
            
            #if defined (IVA_STORE_RESULT_NAMES)
                sprintf(shapeMacthingResults->resultName, "Rectangle %d.Width (Pix.)", i + 1);
            #endif
            shapeMacthingResults->type = IVA_NUMERIC;
            shapeMacthingResults->resultVal.numVal = matchedRectangles[i].width;
            shapeMacthingResults++;
            
            if (visionInfo & IMAQ_VISIONINFO_CALIBRATION)
            {
                realWorldPosition = imaqTransformPixelToRealWorld(image, matchedRectangles[i].corner, 4);
                imaqGetDistance (realWorldPosition->points[0], realWorldPosition->points[1], &calibratedWidth);
                imaqGetDistance (realWorldPosition->points[1], realWorldPosition->points[2], &calibratedHeight);
                
                #if defined (IVA_STORE_RESULT_NAMES)
                    sprintf(shapeMacthingResults->resultName, "Rectangle %d.Width (World)", i + 1);
                #endif
                shapeMacthingResults->type = IVA_NUMERIC;
                shapeMacthingResults->resultVal.numVal = calibratedWidth;
                shapeMacthingResults++;
            }

            #if defined (IVA_STORE_RESULT_NAMES)
                sprintf(shapeMacthingResults->resultName, "Rectangle %d.Height (Pix.)", i + 1);
            #endif
            shapeMacthingResults->type = IVA_NUMERIC;
            shapeMacthingResults->resultVal.numVal = matchedRectangles[i].height;
            shapeMacthingResults++;
            
            if (visionInfo & IMAQ_VISIONINFO_CALIBRATION)
            {
                #if defined (IVA_STORE_RESULT_NAMES)
                    sprintf(shapeMacthingResults->resultName, "Rectangle %d.Height (World)", i + 1);
                #endif
                shapeMacthingResults->type = IVA_NUMERIC;
                shapeMacthingResults->resultVal.numVal = calibratedHeight;
                shapeMacthingResults++;
            }
            
            #if defined (IVA_STORE_RESULT_NAMES)
                sprintf(shapeMacthingResults->resultName, "Rectangle %d.Angle (degrees)", i + 1);
            #endif
            shapeMacthingResults->type = IVA_NUMERIC;
            shapeMacthingResults->resultVal.numVal = matchedRectangles[i].rotation;
            shapeMacthingResults++;

            #if defined (IVA_STORE_RESULT_NAMES)
                sprintf(shapeMacthingResults->resultName, "Rectangle %d.Corner1 X (Pix.)", i + 1);
            #endif
            shapeMacthingResults->type = IVA_NUMERIC;
            shapeMacthingResults->resultVal.numVal = matchedRectangles[i].corner[0].x;
            shapeMacthingResults++;

            #if defined (IVA_STORE_RESULT_NAMES)
                sprintf(shapeMacthingResults->resultName, "Rectangle %d.Corner1 Y (Pix.)", i + 1);
            #endif
            shapeMacthingResults->type = IVA_NUMERIC;
            shapeMacthingResults->resultVal.numVal = matchedRectangles[i].corner[0].y;
            shapeMacthingResults++;
            
            if (visionInfo & IMAQ_VISIONINFO_CALIBRATION)
            {
                #if defined (IVA_STORE_RESULT_NAMES)
                    sprintf(shapeMacthingResults->resultName, "Rectangle %d.Corner1 X (World)", i + 1);
                #endif
                shapeMacthingResults->type = IVA_NUMERIC;
                shapeMacthingResults->resultVal.numVal = realWorldPosition->points[0].x;
                shapeMacthingResults++;
                
                #if defined (IVA_STORE_RESULT_NAMES)
                    sprintf(shapeMacthingResults->resultName, "Rectangle %d.Corner1 Y (World)", i + 1);
                #endif
                shapeMacthingResults->type = IVA_NUMERIC;
                shapeMacthingResults->resultVal.numVal = realWorldPosition->points[0].y;
                shapeMacthingResults++;
            }
            
            #if defined (IVA_STORE_RESULT_NAMES)
                sprintf(shapeMacthingResults->resultName, "Rectangle %d.Corner2 X (Pix.)", i + 1);
            #endif
            shapeMacthingResults->type = IVA_NUMERIC;
            shapeMacthingResults->resultVal.numVal = matchedRectangles[i].corner[1].x;
            shapeMacthingResults++;

            #if defined (IVA_STORE_RESULT_NAMES)
                sprintf(shapeMacthingResults->resultName, "Rectangle %d.Corner2 Y (Pix.)", i + 1);
            #endif
            shapeMacthingResults->type = IVA_NUMERIC;
            shapeMacthingResults->resultVal.numVal = matchedRectangles[i].corner[1].y;
            shapeMacthingResults++;
            
            if (visionInfo & IMAQ_VISIONINFO_CALIBRATION)
            {
                #if defined (IVA_STORE_RESULT_NAMES)
                    sprintf(shapeMacthingResults->resultName, "Rectangle %d.Corner2 X (World)", i + 1);
                #endif
                shapeMacthingResults->type = IVA_NUMERIC;
                shapeMacthingResults->resultVal.numVal = realWorldPosition->points[1].x;
                shapeMacthingResults++;
                
                #if defined (IVA_STORE_RESULT_NAMES)
                    sprintf(shapeMacthingResults->resultName, "Rectangle %d.Corner2 Y (World)", i + 1);
                #endif
                shapeMacthingResults->type = IVA_NUMERIC;
                shapeMacthingResults->resultVal.numVal = realWorldPosition->points[1].y;
                shapeMacthingResults++;
            }
            
            #if defined (IVA_STORE_RESULT_NAMES)
                sprintf(shapeMacthingResults->resultName, "Rectangle %d.Corner3 X (Pix.)", i + 1);
            #endif
            shapeMacthingResults->type = IVA_NUMERIC;
            shapeMacthingResults->resultVal.numVal = matchedRectangles[i].corner[2].x;
            shapeMacthingResults++;

            #if defined (IVA_STORE_RESULT_NAMES)
                sprintf(shapeMacthingResults->resultName, "Rectangle %d.Corner3 Y (Pix.)", i + 1);
            #endif
            shapeMacthingResults->type = IVA_NUMERIC;
            shapeMacthingResults->resultVal.numVal = matchedRectangles[i].corner[2].y;
            shapeMacthingResults++;
            
            if (visionInfo & IMAQ_VISIONINFO_CALIBRATION)
            {
                #if defined (IVA_STORE_RESULT_NAMES)
                    sprintf(shapeMacthingResults->resultName, "Rectangle %d.Corner3 X (World)", i + 1);
                #endif
                shapeMacthingResults->type = IVA_NUMERIC;
                shapeMacthingResults->resultVal.numVal = realWorldPosition->points[2].x;
                shapeMacthingResults++;
                
                #if defined (IVA_STORE_RESULT_NAMES)
                    sprintf(shapeMacthingResults->resultName, "Rectangle %d.Corner3 Y (World)", i + 1);
                #endif
                shapeMacthingResults->type = IVA_NUMERIC;
                shapeMacthingResults->resultVal.numVal = realWorldPosition->points[2].y;
                shapeMacthingResults++;
            }
            
            #if defined (IVA_STORE_RESULT_NAMES)
                sprintf(shapeMacthingResults->resultName, "Rectangle %d.Corner4 X (Pix.)", i + 1);
            #endif
            shapeMacthingResults->type = IVA_NUMERIC;
            shapeMacthingResults->resultVal.numVal = matchedRectangles[i].corner[3].x;
            shapeMacthingResults++;

            #if defined (IVA_STORE_RESULT_NAMES)
                sprintf(shapeMacthingResults->resultName, "Rectangle %d.Corner4 Y (Pix.)", i + 1);
            #endif
            shapeMacthingResults->type = IVA_NUMERIC;
            shapeMacthingResults->resultVal.numVal = matchedRectangles[i].corner[3].y;
            shapeMacthingResults++;
            
            if (visionInfo & IMAQ_VISIONINFO_CALIBRATION)
            {
                #if defined (IVA_STORE_RESULT_NAMES)
                    sprintf(shapeMacthingResults->resultName, "Rectangle %d.Corner4 X (World)", i + 1);
                #endif
                shapeMacthingResults->type = IVA_NUMERIC;
                shapeMacthingResults->resultVal.numVal = realWorldPosition->points[3].x;
                shapeMacthingResults++;
                
                #if defined (IVA_STORE_RESULT_NAMES)
                    sprintf(shapeMacthingResults->resultName, "Rectangle %d.Corner4 Y (World)", i + 1);
                #endif
                shapeMacthingResults->type = IVA_NUMERIC;
                shapeMacthingResults->resultVal.numVal = realWorldPosition->points[3].y;
                shapeMacthingResults++;
            }
        }
    }

Error:
    // Disposes temporary structures.
    imaqDispose(matchedRectangles);
    imaqDispose(realWorldPosition);

    return success;
}


////////////////////////////////////////////////////////////////////////////////
//
// Function Name: IVA_InitData
//
// Description  : Initializes data for buffer management and results.
//
// Parameters   : # of steps
//                # of coordinate systems
//
// Return Value : success
//
////////////////////////////////////////////////////////////////////////////////
static IVA_Data* IVA_InitData(int numSteps, int numCoordSys)
{
    int success = 1;
    IVA_Data* ivaData = NULL;
    int i;


    // Allocate the data structure.
    VisionErrChk(ivaData = (IVA_Data*)malloc(sizeof (IVA_Data)));

    // Initializes the image pointers to NULL.
    for (i = 0 ; i < IVA_MAX_BUFFERS ; i++)
        ivaData->buffers[i] = NULL;

    // Initializes the steo results array to numSteps elements.
    ivaData->numSteps = numSteps;

    ivaData->stepResults = (IVA_StepResults*)malloc(ivaData->numSteps * sizeof(IVA_StepResults));
    for (i = 0 ; i < numSteps ; i++)
    {
        #if defined (IVA_STORE_RESULT_NAMES)
            sprintf(ivaData->stepResults[i].stepName, "");
        #endif
        ivaData->stepResults[i].numResults = 0;
        ivaData->stepResults[i].results = NULL;
    }

    // Create the coordinate systems
	ivaData->baseCoordinateSystems = NULL;
	ivaData->MeasurementSystems = NULL;
	if (numCoordSys)
	{
		ivaData->baseCoordinateSystems = (CoordinateSystem*)malloc(sizeof(CoordinateSystem) * numCoordSys);
		ivaData->MeasurementSystems = (CoordinateSystem*)malloc(sizeof(CoordinateSystem) * numCoordSys);
	}

    ivaData->numCoordSys = numCoordSys;

Error:
    return ivaData;
}


////////////////////////////////////////////////////////////////////////////////
//
// Function Name: IVA_DisposeData
//
// Description  : Releases the memory allocated in the IVA_Data structure
//
// Parameters   : ivaData  -  Internal data structure
//
// Return Value : success
//
////////////////////////////////////////////////////////////////////////////////
static int IVA_DisposeData(IVA_Data* ivaData)
{
    int i;


    // Releases the memory allocated for the image buffers.
    for (i = 0 ; i < IVA_MAX_BUFFERS ; i++)
        imaqDispose(ivaData->buffers[i]);

    // Releases the memory allocated for the array of measurements.
    for (i = 0 ; i < ivaData->numSteps ; i++)
        IVA_DisposeStepResults(ivaData, i);

    free(ivaData->stepResults);

    // Dispose of coordinate systems
    if (ivaData->numCoordSys)
    {
        free(ivaData->baseCoordinateSystems);
        free(ivaData->MeasurementSystems);
    }

    free(ivaData);

    return TRUE;
}


////////////////////////////////////////////////////////////////////////////////
//
// Function Name: IVA_DisposeStepResults
//
// Description  : Dispose of the results of a specific step.
//
// Parameters   : ivaData    -  Internal data structure
//                stepIndex  -  step index
//
// Return Value : success
//
////////////////////////////////////////////////////////////////////////////////
static int IVA_DisposeStepResults(IVA_Data* ivaData, int stepIndex)
{
    int i;

    
    for (i = 0 ; i < ivaData->stepResults[stepIndex].numResults ; i++)
    {
        if (ivaData->stepResults[stepIndex].results[i].type == IVA_STRING)
            free(ivaData->stepResults[stepIndex].results[i].resultVal.strVal);
    }

    free(ivaData->stepResults[stepIndex].results);

    return TRUE;
}


