

#include "project.h"
#include "drv_spi.h"
#include "binary.h"
#include "config.h"
#include "drv_time.h"

void rgb_send( int data);

#if ( RGB_LED_NUMBER > 0)

void rgb_init(void)
{    
	// spi port inits

	if ( (RGB_PIN == GPIO_Pin_13 || RGB_PIN == GPIO_Pin_14) && RGB_PORT == GPIOA ) 
	{
		// programming port used
		
		// wait until 2 seconds from powerup passed
		while ( gettime() < 2e6 ) ;
	}
	
		GPIO_InitTypeDef  GPIO_InitStructure;
	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_InitStructure.GPIO_Pin = RGB_PIN;
	GPIO_Init(RGB_PORT, &GPIO_InitStructure);


	for (int i = 0 ; i < RGB_LED_NUMBER ; i++)
	{
		rgb_send( 0 );
	}
	
}


#define gpioset( port , pin) port->BSRR = pin
#define gpioreset( port , pin) port->BRR = pin

#define RGBHIGH gpioset( RGB_PORT, RGB_PIN)
#define RGBLOW gpioreset( RGB_PORT, RGB_PIN);



#pragma push

#pragma Otime
#pragma O2

void delay1a()
{
	uint8_t count = 2; 
	while (count--);
}

void delay1b()
{
	uint8_t count = 2; 
	while (count--);
}

void delay2a()
{
	uint8_t count = 1; 
	while (count--);
}

void delay2b()
{
	uint8_t count = 3; 
	while (count--);
}

void rgb_send( int data)
{
for ( int i =23 ; i >=0 ; i--)
	{
		if (  (data>>i)&1  ) 
		{
			RGBHIGH;
			delay1a();
			RGBLOW;
			delay1b();
		}
		else 
		{
			RGBHIGH;
			delay2a();
			RGBLOW;
			delay2b();
		}

	}

}



#pragma pop

#else
// rgb led not found
// some dummy headers just in case
void rgb_init(void)
{
	
}

void rgb_send( int data)
{
	
}
#endif











