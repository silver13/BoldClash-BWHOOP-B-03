/*
The MIT License (MIT)

Copyright (c) 2016 silverx

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include <stdbool.h>
#include <stdlib.h>
#include "pid.h"
#include "util.h"
#include "config.h"
#include "led.h"
#include "defines.h"


//************************************PIDS****************************************

// Using Torque Boost 1 and Strong Filtering
//(EXPERIMENTAL) chime13
//                         ROLL       PITCH     YAW
//float pidkp[PIDNUMBER] = { 21.7e-2 , 21.7e-2  , 9.5e-1 }; 
//float pidki[PIDNUMBER] = { 12e-1  , 12e-1 , 8e-1 };	
//float pidkd[PIDNUMBER] = { 7.4e-1 , 7.4e-1  , 0.0e-1 };	

// Using Torque Boost 2 and Strong Filtering
//(EXPERIMENTAL) chime13
//                         ROLL       PITCH     YAW
float pidkp[PIDNUMBER] = { 22.7e-2 , 22.7e-2  , 9.5e-1 }; 
float pidki[PIDNUMBER] = { 12e-1  , 12e-1 , 8e-1 };	
float pidkd[PIDNUMBER] = { 7.4e-1 , 7.4e-1  , 0.0e-1 };	

//************************************Setpoint Weight****************************************
// "setpoint weighting" 0.0 - 1.0 where 1.0 = normal pid
#define ENABLE_SETPOINT_WEIGHTING
//            Roll   Pitch   Yaw
float b[3] = { 0.99 , 0.99 , 0.99};   //RACE
//float b[3] = { 0.93 , 0.93 , 0.9};      //FREESTYLE

/// output limit			
const float outlimit[PIDNUMBER] = { 1.7 , 1.7 , 0.5 };

// limit of integral term (abs)
const float integrallimit[PIDNUMBER] = { 1.7 , 1.7 , 0.5 };

#define SIMPSON_RULE_INTEGRAL

// non changable things below
float * pids_array[3] = {pidkp, pidki, pidkd};
int number_of_increments[3][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
int current_pid_axis = 0;
int current_pid_term = 0;
float * current_pid_term_pointer = pidkp;

float ierror[PIDNUMBER] = { 0 , 0 , 0};	
float pidoutput[PIDNUMBER];
float setpoint[PIDNUMBER];
static float lasterror[PIDNUMBER];
float v_compensation = 1.00;

extern float error[PIDNUMBER];
extern float looptime;
extern float gyro[3];
extern int onground;
extern float looptime;
extern int in_air;
extern char aux[AUXNUMBER];
extern float vbattfilt;
extern int ledcommand;


// multiplier for pids at 3V - for PID_VOLTAGE_COMPENSATION - default 1.33f from H101 code
#define PID_VC_FACTOR 1.33f

#ifdef NORMAL_DTERM
static float lastrate[PIDNUMBER];
#endif

#ifdef NEW_DTERM
static float lastratexx[PIDNUMBER][2];
#endif

#ifdef MAX_FLAT_LPF_DIFF_DTERM
static float lastratexx[PIDNUMBER][4];
#endif

#ifdef SIMPSON_RULE_INTEGRAL
static float lasterror2[PIDNUMBER];
#endif

float timefactor;

// pid calculation for acro ( rate ) mode
// input: error[x] = setpoint - gyro
// output: pidoutput[x] = change required from motors
float pid(int x )
{ 
    if ((aux[LEVELMODE]) && (!aux[RACEMODE])){
				if ((onground) || (in_air == 0)){
						ierror[x] *= 0.98f;}
		}else{
			  if (onground) ierror[x] *= 0.98f;
		}
		
#ifdef TRANSIENT_WINDUP_PROTECTION
    static float avgSetpoint[3];
    static int count[3];
    extern float splpf( float in,int num );
    
    if ( x < 2 && (count[x]++ % 2) == 0 ) {
        avgSetpoint[x] = splpf( setpoint[x], x );
    }
#endif
		
    int iwindup = 0;
    if (( pidoutput[x] == outlimit[x] )&& ( error[x] > 0) )
    {
        iwindup = 1;		
    }
    
    if (( pidoutput[x] == -outlimit[x])&& ( error[x] < 0) )
    {
        iwindup = 1;				
    } 
    
    #ifdef ANTI_WINDUP_DISABLE
    iwindup = 0;
    #endif
 
    #ifdef TRANSIENT_WINDUP_PROTECTION
		if ( x < 2 && fabsf( setpoint[x] - avgSetpoint[x] ) > 0.1f ) {
			iwindup = 1;
		}
    #endif
		
    if ( !iwindup)
    {
        #ifdef MIDPOINT_RULE_INTEGRAL
         // trapezoidal rule instead of rectangular
        ierror[x] = ierror[x] + (error[x] + lasterror[x]) * 0.5f *  pidki[x] * looptime;
        lasterror[x] = error[x];
        #endif
            
        #ifdef RECTANGULAR_RULE_INTEGRAL
        ierror[x] = ierror[x] + error[x] *  pidki[x] * looptime;
        lasterror[x] = error[x];					
        #endif
            
        #ifdef SIMPSON_RULE_INTEGRAL
        // assuming similar time intervals
        ierror[x] = ierror[x] + 0.166666f* (lasterror2[x] + 4*lasterror[x] + error[x]) *  pidki[x] * looptime;	
        lasterror2[x] = lasterror[x];
        lasterror[x] = error[x];
        #endif					
    }
            
    limitf( &ierror[x] , integrallimit[x] );
    
    
    #ifdef ENABLE_SETPOINT_WEIGHTING
    // P term
    pidoutput[x] = error[x] * ( b[x])* pidkp[x];				
    // b
    pidoutput[x] +=  - ( 1.0f - b[x])* pidkp[x] * gyro[x];
    #else
    // P term with b disabled
    pidoutput[x] = error[x] * pidkp[x];
    #endif
    
#ifdef FEED_FORWARD_STRENGTH
//if (aux[CH_AUX1]){	//I want it full time	
	if ( x < 2 ) {
		static float lastSetpoint[2];
		static float bucket[2];
		static float buckettake[2];
		if ( setpoint[x] != lastSetpoint[x] ) {
			bucket[x] += setpoint[x] - lastSetpoint[x];
			buckettake[x] = bucket[x] * 0.2f; // Spread it evenly over 5 ms (PACKET_PERIOD)
		}
		if ( fabsf( bucket[x] ) > 0.0f ) {
			float take = buckettake[x];
			if ( bucket[x] < 0.0f != take < 0.0f || fabsf( take ) > fabsf( bucket[x] ) ) {
				take = bucket[x];
			}
			bucket[x] -= take;
			float ff = take * timefactor * FEED_FORWARD_STRENGTH * pidkd[x];
			
			if ( ff < 0.0f == pidoutput[x] < 0.0f && fabsf( ff ) > fabsf( pidoutput[x] ) ) {
				pidoutput[x] = ff;
				ledcommand = 1;
			}
		}
		lastSetpoint[x] = setpoint[x];
	}
}	
#endif		
		
    // I term	
    pidoutput[x] += ierror[x];

    // D term
    // skip yaw D term if not set               
    if ( pidkd[x] > 0 )
    {
        #ifdef NORMAL_DTERM
        pidoutput[x] = pidoutput[x] - (gyro[x] - lastrate[x]) * pidkd[x] * timefactor  ;
        lastrate[x] = gyro[x];
        #endif

        #ifdef NEW_DTERM
        pidoutput[x] = pidoutput[x] - ( ( 0.5f) *gyro[x] 
                    - (0.5f) * lastratexx[x][1] ) * pidkd[x] * timefactor  ;
                        
        lastratexx[x][1] = lastratexx[x][0];
        lastratexx[x][0] = gyro[x];
        #endif
    
        #ifdef MAX_FLAT_LPF_DIFF_DTERM 
        pidoutput[x] = pidoutput[x] - ( + 0.125f *gyro[x] + 0.250f * lastratexx[x][0]
                    - 0.250f * lastratexx[x][2] - ( 0.125f) * lastratexx[x][3]) * pidkd[x] * timefactor 						;

        lastratexx[x][3] = lastratexx[x][2];
        lastratexx[x][2] = lastratexx[x][1];
        lastratexx[x][1] = lastratexx[x][0];
        lastratexx[x][0] = gyro[x];
        #endif 


        #ifdef DTERM_LPF_1ST_HZ
        float dterm;
        static float lastrate[3];
        static float dlpf[3] = {0};

        dterm = - (gyro[x] - lastrate[x]) * pidkd[x] * timefactor;
        lastrate[x] = gyro[x];

        lpf( &dlpf[x], dterm, FILTERCALC( 0.001 , 1.0f/DTERM_LPF_1ST_HZ ) );

        pidoutput[x] += dlpf[x];                   
        #endif
        
        #if (defined DTERM_LPF_2ND_HZ && defined ERROR_D_TERM)
        float dterm;
        //static float lastrate[3]; 
				static float lasterrorx[PIDNUMBER];
        float lpf2( float in, int num);
        if ( pidkd[x] > 0)
        {
					//if (aux[CH_AUX1]){
						     dterm =  (error[x] - lasterrorx[x]) * pidkd[x] * timefactor;
                 lasterrorx[x] = error[x];
					//}else{	
								// dterm = - (gyro[x] - lastrate[x]) * pidkd[x] * timefactor;
								// lastrate[x] = gyro[x];	}
	
            dterm = lpf2(  dterm, x );
            pidoutput[x] += dterm;}
				#endif
				
        #if (defined DTERM_LPF_2ND_HZ && !defined ERROR_D_TERM)
        float dterm;
        static float lastrate[3]; 
        float lpf2( float in, int num);
        if ( pidkd[x] > 0){
						dterm = - (gyro[x] - lastrate[x]) * pidkd[x] * timefactor;
						lastrate[x] = gyro[x];	
            dterm = lpf2(  dterm, x );
            pidoutput[x] += dterm;}
				#endif   

    }
    		#ifdef PID_VOLTAGE_COMPENSATION
					pidoutput[x] *= v_compensation;
				#endif
    limitf(  &pidoutput[x] , outlimit[x]);

return pidoutput[x];		 		
}

// calculate change from ideal loop time
// 0.0032f is there for legacy purposes, should be 0.001f = looptime
// this is called in advance as an optimization because it has division
void pid_precalc()
{
	timefactor = 0.0032f / looptime;
	
	#ifdef PID_VOLTAGE_COMPENSATION
	v_compensation = mapf ( vbattfilt , 3.00 , 4.00 , PID_VC_FACTOR , 1.00);
	if( v_compensation > PID_VC_FACTOR) v_compensation = PID_VC_FACTOR;
	if( v_compensation < 1.00f) v_compensation = 1.00;
#endif
}


#ifndef DTERM_LPF_2ND_HZ 
#define DTERM_LPF_2ND_HZ 99
#endif

//the compiler calculates these
static float two_one_minus_alpha = 2*FILTERCALC( 0.001 , (1.0f/DTERM_LPF_2ND_HZ) );
static float one_minus_alpha_sqr = (FILTERCALC( 0.001 , (1.0f/DTERM_LPF_2ND_HZ) ) )*(FILTERCALC( 0.001 , (1.0f/DTERM_LPF_2ND_HZ) ));
static float alpha_sqr = (1 - FILTERCALC( 0.001 , (1.0f/DTERM_LPF_2ND_HZ) ))*(1 - FILTERCALC( 0.001 , (1.0f/DTERM_LPF_2ND_HZ) ));

static float last_out[3], last_out2[3];

float lpf2( float in, int num)
 {

  float ans = in * alpha_sqr + two_one_minus_alpha * last_out[num]
      - one_minus_alpha_sqr * last_out2[num];   

  last_out2[num] = last_out[num];
  last_out[num] = ans;
  
  return ans;
 }

// below are functions used with gestures for changing pids by a percentage

// Cycle through P / I / D - The initial value is P
// The return value is the currently selected TERM (after setting the next one)
// 1: P
// 2: I
// 3: D
// The return value is used to blink the leds in main.c
int next_pid_term()
{
//	current_pid_axis = 0;
	
	switch (current_pid_term)
	{
		case 0:
			current_pid_term_pointer = pidki;
			current_pid_term = 1;
			break;
		case 1:
			current_pid_term_pointer = pidkd;
			current_pid_term = 2;
			break;
		case 2:
			current_pid_term_pointer = pidkp;
			current_pid_term = 0;
			break;
	}
	
	return current_pid_term + 1;
}

// Cycle through the axis - Initial is Roll
// Return value is the selected axis, after setting the next one.
// 1: Roll
// 2: Pitch
// 3: Yaw
// The return value is used to blink the leds in main.c
int next_pid_axis()
{
	const int size = 3;
	if (current_pid_axis == size - 1) {
		current_pid_axis = 0;
	}
	else {
		#ifdef COMBINE_PITCH_ROLL_PID_TUNING
		if (current_pid_axis <2 ) {
			// Skip axis == 1 which is roll, and go directly to 2 (Yaw)
			current_pid_axis = 2;
		}
		#else
		current_pid_axis++;
		#endif
	}
	
	return current_pid_axis + 1;
}

#define PID_GESTURES_MULTI 1.1f

int change_pid_value(int increase)
{
	float multiplier = 1.0f/(float)PID_GESTURES_MULTI;
	if (increase) {
		multiplier = (float)PID_GESTURES_MULTI;
		number_of_increments[current_pid_term][current_pid_axis]++;
	}
	else {
		number_of_increments[current_pid_term][current_pid_axis]--;
	}
    
	current_pid_term_pointer[current_pid_axis] = current_pid_term_pointer[current_pid_axis] * multiplier;
	
    #ifdef COMBINE_PITCH_ROLL_PID_TUNING
	if (current_pid_axis == 0) {
		current_pid_term_pointer[current_pid_axis+1] = current_pid_term_pointer[current_pid_axis+1] * multiplier;
	}
	#endif
	
	return abs(number_of_increments[current_pid_term][current_pid_axis]);
}

// Increase currently selected term, for the currently selected axis, (by functions above) by 10%
// The return value, is absolute number of times the specific term/axis was increased or decreased.  For example, if P for Roll was increased by 10% twice,
// And then reduced by 10% 3 times, the return value would be 1  -  The user has to rememeber he has eventually reduced the by 10% and not increased by 10%
// I guess this can be improved by using the red leds for increments and blue leds for decrements or something, or just rely on SilverVISE
int increase_pid()
{
	return change_pid_value(1);
}

// Same as increase_pid but... you guessed it... decrease!
int decrease_pid()
{
	return change_pid_value(0);
}

void rotateErrors()
{
	#ifdef YAW_FIX
	// rotation around x axis:
	ierror[1] -= ierror[2] * gyro[0] * looptime;
	ierror[2] += ierror[1] * gyro[0] * looptime;

	// rotation around y axis:
	ierror[2] -= ierror[0] * gyro[1] * looptime;
	ierror[0] += ierror[2] * gyro[1] * looptime;

	// rotation around z axis:
	ierror[0] -= ierror[1] * gyro[2] * looptime;
	ierror[1] += ierror[0] * gyro[2] * looptime;
#endif
}
