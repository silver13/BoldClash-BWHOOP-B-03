#include "config.h"
#include "util.h"

#include <math.h>
#include <string.h>



extern float rxcopy[];
extern float GEstG[3];
extern float Q_rsqrt( float number );


float errorvect[3];

void stick_vector( float maxangle)
{
	
float stickvector[3];	
	

float pitch, roll;

	// rotate down vector to match stick position
pitch = rxcopy[1] * MAX_ANGLE_HI * DEGTORAD + (float) TRIM_PITCH  * DEGTORAD;
roll = rxcopy[0] * MAX_ANGLE_HI * DEGTORAD + (float) TRIM_ROLL  * DEGTORAD;

stickvector[0] = fastsin( roll );
stickvector[1] = fastsin( pitch );
stickvector[2] = fastcos( roll ) * fastcos( pitch );

		
float	mag2 = (stickvector[0] * stickvector[0] + stickvector[1] * stickvector[1]);

if ( mag2 > 0.001f ) 
{
mag2 = Q_rsqrt( mag2 / (1 - stickvector[2] * stickvector[2]) );
}
else mag2 = 0.707f;

stickvector[0] *=mag2;
stickvector[1] *=mag2;	


// find error between stick vector and quad orientation
// vector cross product 
  errorvect[0]= (GEstG[1]*stickvector[2]) - (GEstG[2]*stickvector[1]);
  errorvect[1]= (GEstG[2]*stickvector[0]) - (GEstG[0]*stickvector[2]);

// some limits just in case
limitf( & errorvect[0] , 1.0);
limitf( & errorvect[1] , 1.0);

float temp = errorvect[0];
errorvect[0] = (errorvect[1]);
errorvect[1] = (- temp);

}



