

#include <stdbool.h>
#include "pid.h"
#include "util.h"
#include "config.h"
#include "defines.h"

#define APIDNUMBER 2

//                             ANGLE PIDS      
// yaw is done by the rate yaw pid
// Kp                       ROLL     PITCH  
float apidkp[APIDNUMBER] = { 11e-2, 11e-2 };

// Ki                        ROLL     PITCH  
float apidki[APIDNUMBER] = { 0e-2, 0e-2 };   

// code variables below

// rate limit
#define OUTLIMIT_FLOAT (float)LEVEL_MAX_RATE*DEGTORAD

#define ITERMLIMIT_FLOAT OUTLIMIT_FLOAT


extern int onground;
extern float looptime;
extern float gyro[3];

float aierror[APIDNUMBER] = { 0, 0};
float apidoutput[APIDNUMBER];
float angleerror[3];

float apid(int x)
{
 int index = x;
	
	if (onground)
	  {
		  aierror[x] *= 0.8f;
	  }
	// anti windup
	// prevent integral increase if output is at max
	int iwindup = 0;
	if ((apidoutput[x] == OUTLIMIT_FLOAT) && (gyro[index] > 0))
	  {
		  iwindup = 1;
	  }
	if ((apidoutput[x] == -OUTLIMIT_FLOAT) && (gyro[index] < 0))
	  {
		  iwindup = 1;
	  }
	if (!iwindup)
	  {
		  aierror[x] = aierror[x] + angleerror[index] * apidki[x] * looptime;
	  }

	limitf(&aierror[x], ITERMLIMIT_FLOAT);

	// P term
	apidoutput[x] = angleerror[index] * apidkp[x];


	// I term       
	apidoutput[x] += aierror[x];



	limitf(&apidoutput[x], OUTLIMIT_FLOAT);


	return apidoutput[x];
}
