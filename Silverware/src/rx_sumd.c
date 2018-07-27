// serial for stm32 not used yet
#include "project.h"
#include "stm32f0xx_usart.h"
#include <stdio.h>
#include "drv_serial.h"
#include "config.h"
#include "drv_time.h"
#include "util.h"

// sumd input ( pin SWCLK after calibration) 
// WILL DISABLE PROGRAMMING AFTER GYRO CALIBRATION - 2 - 3 seconds after powerup)


#ifdef RX_SUMD

#define SERIAL_BAUDRATE 115000

#define SBUS_INVERT 0

// global use rx variables
extern float rx[4];
extern char aux[AUXNUMBER];
extern char lastaux[AUXNUMBER];
extern char auxchange[AUXNUMBER];
int failsafe = 0;
int rxmode = 0;


// internal sumd variables
#define RX_BUFF_SIZE 64
uint8_t rx_buffer[RX_BUFF_SIZE];
uint8_t rx_start = 0;
uint8_t rx_end = 0;
uint16_t rx_time[RX_BUFF_SIZE];

int framestarted = -1;
uint8_t framestart = 0;


unsigned long time_lastrx;
unsigned long time_siglost;

unsigned long time_lastframe;
int frame_received = 0;
int rx_state = 0;
uint8_t data[25];
//int channels[9];

int failsafe_sumd_failsafe = 0;
int failsafe_noframes = 0;

// enable statistics
int sumd_stats = 1;

// check timing of received data
int serial_timing = 0;
// bypass start frame count / throttle check
int bypass_safety = 1;

// statistics
int stat_framestartcount;
int stat_timing_fail;
int stat_garbage;
//int stat_timing[25];
int stat_frames_accepted = 0;
int stat_frames_second;
int stat_overflow;
int crcfail = 0;


#define CRC_POLYNOME 0x1021

uint16_t CRC16(uint16_t crc, uint8_t value)
{
crc = crc ^ ( int16_t )value<<8;
for(int i=0; i<8; i++) {
 if (crc & 0x8000)
 crc = (crc << 1) ^ CRC_POLYNOME;
 else
 crc = (crc << 1);
}
return crc;
}


int mapint(int x, int in_min, int in_max, int out_min, int out_max)
{
return ((x - in_min) * (out_max - out_min)) / (in_max - in_min) + out_min;
}

void USART1_IRQHandler(void)
{
    rx_buffer[rx_end] = USART_ReceiveData(USART1);
    // calculate timing since last rx
    if (serial_timing)
    {
    unsigned long  maxticks = SysTick->LOAD;	
    unsigned long ticks = SysTick->VAL;	
    unsigned long elapsedticks;	
    static unsigned long lastticks;

    if (ticks < lastticks) 
        elapsedticks = lastticks - ticks;	
    else
        {// overflow ( underflow really)
        elapsedticks = lastticks + ( maxticks - ticks);	
        }

    if ( elapsedticks < 65536 ) rx_time[rx_end] = elapsedticks; 
    else rx_time[rx_end] = 65535;

    lastticks = ticks;
    } 
    if ( USART_GetFlagStatus(USART1 , USART_FLAG_ORE ) )
    {
      // overflow means something was lost 
      if (serial_timing) rx_time[rx_end]= 0xFFFe;
      USART_ClearFlag( USART1 , USART_FLAG_ORE );
      if ( sumd_stats ) stat_overflow++;
    }    
        
    rx_end++;
    rx_end%=(RX_BUFF_SIZE);
}

void inject( int in)
{
    rx_buffer[rx_end] = in;
    rx_end++;
    rx_end%=(RX_BUFF_SIZE);
}

void sumd_init(void)
{
    // make sure there is some time to program the board
    if ( gettime() < 2000000 ) return;
    
    GPIO_InitTypeDef  GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;	
    GPIO_Init(GPIOA, &GPIO_InitStructure); 

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource14 , GPIO_AF_1);
     
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);


    USART_InitTypeDef USART_InitStructure;

    USART_InitStructure.USART_BaudRate = SERIAL_BAUDRATE;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx ;//USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART1, &USART_InitStructure);
// swap rx/tx pins
    USART_SWAPPinCmd( USART1, ENABLE);
// invert signal ( default sbus )
   if (SBUS_INVERT) USART_InvPinCmd(USART1, USART_InvPin_Rx|USART_InvPin_Tx , ENABLE );


    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

    USART_Cmd(USART1, ENABLE);

    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);


    rxmode = !RXMODE_BIND;

// set setup complete flag
 framestarted = 0;
}


void rx_init(void)
{
    
}

unsigned int auto_size = 4;

void checkrx()
{
 
if ( framestarted == 0)
{
    while (  rx_end != rx_start )
    { 
    if ( rx_buffer[rx_start] == 0xA8 )
            {
                // start detected
                framestart = rx_start;
                framestarted = 1;  
                stat_framestartcount++; 
            break;                
            }         
    rx_start++;
    rx_start%=(RX_BUFF_SIZE);
            
    stat_garbage++;
    }
            
}
else if ( framestarted == 1)
{
 // frame already has begun
 int size = 0;
    if (rx_end > framestart ) size = rx_end - framestart;
    else size = RX_BUFF_SIZE - framestart + rx_end;
 if ( size >= auto_size )
    {    
    int timing_fail = 0; 
        
    for ( int i = framestart ; i <framestart + auto_size; i++  )  
    {
      data[ i - framestart] = rx_buffer[i%(RX_BUFF_SIZE)];
      int symboltime = rx_time[i%(RX_BUFF_SIZE)];
      //stat_timing[ i - framestart] = symboltime;
      if ( serial_timing && symboltime > 1024 &&  i - framestart > 0 ) timing_fail = 1;
        else 
        {
            if ( i == framestart+ 2 )
            {
                auto_size = data[ i - framestart]*2 + 3 + 2; 
                if ( auto_size > 0x20 + 5 ) 
                {   timing_fail = 1;   
                   auto_size = 4; 
                }  
                if ( auto_size < 4 ) auto_size = 4;                 
                if ( size < auto_size ) return;
            }
        }
    }    

   if (!timing_fail) 
   {      
       
       uint16_t receiverd_crc = 0;
       uint16_t crc = 0;
       int i;
       //crc = 0;
       for (  i = 0 ; i < auto_size - 2; i++  ) 
       {
         crc = CRC16( crc , data[i]);
       }
       
       receiverd_crc = (data[i]<<8) + data[i+1]; 
       if ( receiverd_crc != crc)
       {
           // rx_start = rx_end;
            //framestarted = 0;
            crcfail++;
           goto end;
       }
       else frame_received = 1;
       
      if (data[1] != 0x01) 
      {
        // failsafe bit
        failsafe_sumd_failsafe = 1;
      }
      else
      {
        failsafe_sumd_failsafe = 0;
      }
      
           
   }else if (sumd_stats) stat_timing_fail++; 
    

end:  
    rx_start = rx_end;
    framestarted = 0;
    
    } // end frame complete  
    
}// end frame pending
else
    if ( framestarted < 0)
    {
        // initialize sumd
      sumd_init();
       // set in routine above "framestarted = 0;"
        if (0)
        {
        uint8_t test[] = 
        {
         0xA8, /* Graupner ID, start */
         0x01, /* SUMH/SUMD */
         0x08, /* number of channels */
         0x2E, 0xE8, /* data channel 1 */
         0x2E, 0xD0, /* data channel 2 */
         0x2E, 0xF0, /* data channel 3 */
         0x2E, 0xe0, /* data channel 4 */
         0x2E, 0xE0, /* data channel 5 */
         0x2E, 0xE0, /* data channel 6 */
         0x2E, 0xE0, /* data channel 6 */
         0x2E, 0xE0, /* data channel 8 */
         0x57, 0xB4, /* CRC */
        };
 inject(255);
        inject(255);
for ( int i = 0 ; i < sizeof(test) ; i++)
        {
            inject(test[i]);
        }
        inject(255);
        inject(255);
        for ( int i = 0 ; i < sizeof(test) ; i++)
        {
            inject(test[i]);
        }
                inject(255);
        framestarted = 0;  
        }
              
    }
      
if ( frame_received )
{
   int channels[9] = {6400};
   int chan_num =  data[2]; 
   if ( chan_num > 9) chan_num = 9;
   
   //decode frame   
    if ( chan_num > 3  )
    {

        for ( int i = 0 ; i < chan_num ; i++)
        {
          channels[i]  = (data[i*2 + 3]<< 8) + data[i*2 + 4];            
        }
    }
   
    
    
if ( bypass_safety)
{
 rx_state = 1;
 rxmode = !RXMODE_BIND; 
}    
    if ( rx_state == 0)
    {
     // wait for valid sumd signal
     static int frame_count = 0; 
     failsafe = 1;
     rxmode = RXMODE_BIND; 
     // if throttle < 50%   
     if (  channels[2] < 6400 ) frame_count++;
     frame_count++;
     if (frame_count  > 130 )
     {
         if( stat_frames_second > 30 )
         {
             rx_state++; 
             rxmode = !RXMODE_BIND; 
         }
         else
         {
             frame_count = 0;
         }
     }
      
    }
    
    if ( rx_state == 1)
    {
      // normal rx mode

       for ( int i = 0 ; i < chan_num ; i++)
        {
          channels[i] = mapint( channels[i] , 0x2260 , 0x3b60, - 16384, 16384);
        }            
      // AETR channel order        
      
        rx[0] = (float) -channels[1] / 16384.0f;
        rx[1] = (float) channels[2] / 16384.0f;
        rx[2] = (float) -channels[3] / 16384.0f;
        rx[3] = (float) channels[0] / 32768.0f + 0.5f;
                           
        if ( rx[3] > 1 ) rx[3] = 1;
        
        aux[CH_FLIP] = (channels[4] > 0) ? 1 : 0;
		aux[CH_EXPERT] = (channels[5] > 0) ? 1 : 0;
		aux[CH_HEADFREE] = (channels[6] > 0) ? 1 : 0;
		aux[CH_RTH] = (channels[7] > 0) ? 1 : 0;
        
        time_lastframe = gettime(); 
        if (sumd_stats) stat_frames_accepted++;       
    }
 

// stats
    static int fps = 0;
    static unsigned long secondtime = 0;
    
    if ( gettime() - secondtime > 1000000 )
    {
       stat_frames_second = fps;
       fps = 0;
       secondtime = gettime();
    }
    fps++;
    
frame_received = 0;    
} // end frame received


if ( gettime() - time_lastframe > 1000000 )
{
    failsafe_noframes = 1;
}else failsafe_noframes = 0;

// add the 3 failsafes together
    failsafe = failsafe_noframes || failsafe_sumd_failsafe;

}

#endif



