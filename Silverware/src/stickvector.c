#include "config.h"
#include "util.h"

#include <math.h>
#include <string.h>



extern float GEstG[3];
extern float Q_rsqrt( float number );

// error vector between stick position and quad orientation
// this is the output of this function
float errorvect[3];
// cache the last result so it does not get calculated everytime
float last_rx[2] = {13.13f , 12.12f};
float stickvector[3] = { 0 , 0 , 1};




void stick_vector( float rx_input[] , float maxangle)
{
// only compute stick rotation if values changed
if ( last_rx[0] == rx_input[0] && last_rx[1] == rx_input[1] )
{
     
}
else
{
    last_rx[0] = rx_input[0];
    last_rx[1] = rx_input[1]; 
	
	
float pitch, roll;

	// rotate down vector to match stick position
pitch = rx_input[1] * MAX_ANGLE_HI * DEGTORAD + (float) TRIM_PITCH  * DEGTORAD;
roll = rx_input[0] * MAX_ANGLE_HI * DEGTORAD + (float) TRIM_ROLL  * DEGTORAD;

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
}

// find error between stick vector and quad orientation
// vector cross product 
  errorvect[1]= -((GEstG[1]*stickvector[2]) - (GEstG[2]*stickvector[1]));
  errorvect[0]= (GEstG[2]*stickvector[0]) - (GEstG[0]*stickvector[2]);

// some limits just in case

limitf( &errorvect[0] , 1.0);
limitf( &errorvect[1] , 1.0);



}



