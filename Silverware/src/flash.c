
#include "project.h"
#include "drv_fmc.h"

extern int fmc_erase( void );
extern void fmc_unlock(void);
extern void fmc_lock(void);

extern float accelcal[];
extern float * pids_array[3];

extern float hardcoded_pid_identifier;


#define FMC_HEADER 0x12AA0001

float initial_pid_identifier = -10;
float saved_pid_identifier;


float flash_get_hard_coded_pid_identifier( void) {
	float result = 0;

	for (int i=0;  i<3 ; i++) {
		for (int j=0; j<3 ; j++) {
			result += pids_array[i][j] * (i+1) * (j+1) * 0.932f;
		}
	}
	return result;
}


void flash_hard_coded_pid_identifier( void)
{
 initial_pid_identifier = flash_get_hard_coded_pid_identifier();
}




void flash_save( void) {

    fmc_unlock();
	fmc_erase();
	
	unsigned long addresscount = 0;

    writeword(addresscount++, FMC_HEADER);
   
	fmc_write_float(addresscount++, initial_pid_identifier );
	
	for (int i=0;  i<3 ; i++) {
		for (int j=0; j<3 ; j++) {
            fmc_write_float(addresscount++, pids_array[i][j]);
		}
	}
 

    fmc_write_float(addresscount++, accelcal[0]);
    fmc_write_float(addresscount++, accelcal[1]);
    fmc_write_float(addresscount++, accelcal[2]);

   
   
extern char rfchannel[4];
extern char rxaddress[5];
extern int telemetry_enabled;
extern int rx_bind_enable;
    
 // save radio bind info  
    if ( rx_bind_enable )
    {
    writeword(50, rxaddress[4]|telemetry_enabled<<8);
    writeword(51, rxaddress[0]|(rxaddress[1]<<8)|(rxaddress[2]<<16)|(rxaddress[3]<<24));
    writeword(52, rfchannel[0]|(rfchannel[1]<<8)|(rfchannel[2]<<16)|(rfchannel[3]<<24));
    }
    else
    {
      // this will leave 255's so it will be picked up as disabled  
    }
    writeword(255, FMC_HEADER);
    
	fmc_lock();
}



void flash_load( void) {

	unsigned long addresscount = 0;
// check if saved data is present
    if (FMC_HEADER == fmc_read(addresscount++)&& FMC_HEADER == fmc_read(255))
    {

     saved_pid_identifier = fmc_read_float(addresscount++);
// load pids from flash if pid.c values are still the same       
     if (  saved_pid_identifier == initial_pid_identifier )
     {
         for (int i=0;  i<3 ; i++) {
            for (int j=0; j<3 ; j++) {
                pids_array[i][j] = fmc_read_float(addresscount++);
            }
        }
     }
     else{
         addresscount+=9; 
     }    

    accelcal[0] = fmc_read_float(addresscount++ );
    accelcal[1] = fmc_read_float(addresscount++ );
    accelcal[2] = fmc_read_float(addresscount++ );  

       
   
extern char rfchannel[4];
extern char rxaddress[5];
extern int telemetry_enabled;
extern int rx_bind_load;
extern int rx_bind_enable;
     
 // save radio bind info   

    int temp = fmc_read(52);
    int error = 0;
    for ( int i = 0 ; i < 4; i++)
    {
        if ((temp>>(i*8))&0xff > 127)
        {
            error = 1;
        }   
    }
    
    if( !error )   
    {
        rx_bind_load = rx_bind_enable = 1; 
        
        rxaddress[4] = fmc_read(50);

        telemetry_enabled = fmc_read(50)>>8;
        int temp = fmc_read(51);
        for ( int i = 0 ; i < 4; i++)
        {
            rxaddress[i] =  temp>>(i*8);        
        }
        
        temp = fmc_read(52);  
        for ( int i = 0 ; i < 4; i++)
        {
            rfchannel[i] =  temp>>(i*8);  
        }
    }

    }
    else
    {
        
    }
    
}













