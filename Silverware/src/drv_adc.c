#include "project.h"
#include "drv_adc.h"
#include "util.h"
#include "config.h"
#include "debug.h"

uint16_t adcarray[2];
extern debug_type debug;


#ifndef DISABLE_LVC

#ifndef ADC_BATT_VOLTAGE 
#define ADC_BATT_VOLTAGE 3.77
#undef ADC_READOUT
#define ADC_READOUT 2727
#endif

void adc_init(void)
{	 
  ADC_InitTypeDef     ADC_InitStructure;
  
	{
  GPIO_InitTypeDef    GPIO_InitStructure;
  
  GPIO_InitStructure.GPIO_Pin = BATTERYPIN ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(BATTERYPORT, &GPIO_InitStructure);
	}

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1 , ENABLE);
	{
  DMA_InitTypeDef     DMA_InitStructure;
	
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)0x40012440;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)adcarray;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = 2;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	}
  
  ADC_DMARequestModeConfig(ADC1, ADC_DMAMode_Circular);
 
  ADC_DMACmd(ADC1, ENABLE);  

  ADC_StructInit(&ADC_InitStructure);
  
	
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; 
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Backward;
  ADC_Init(ADC1, &ADC_InitStructure); 
	
  ADC_ChannelConfig(ADC1, ADC_Channel_Vrefint , ADC_SampleTime_239_5Cycles); 
 
	ADC_ChannelConfig(ADC1, BATTERY_ADC_CHANNEL , ADC_SampleTime_239_5Cycles); 

	
  ADC_GetCalibrationFactor(ADC1);
  
  ADC_Cmd(ADC1, ENABLE);     

  ADC_StartOfConversion(ADC1);
	
  DMA_Cmd(DMA1_Channel1, ENABLE);
}

float adc_read(int channel)
{
	switch(channel)
	{
		case 0:
		#ifdef DEBUG
		lpf(&debug.adcfilt , (float) adcarray[0] , 0.998);
		#endif
		//return mapf( (float) adcarray[0] , 2727.0 , 3050.0 , 3.77 , 4.22);	
		return (float) adcarray[0] * ((float)(ADC_BATT_VOLTAGE)/(float) (ADC_READOUT)) ;
		
		case 1:
		return adcarray[1];
		
		default:			
	  return 0;
	}
	
	
}
#else
// // lvc disabled
void adc_init(void)
{
	
}
// dummy function with lvc disabled
float adc_read(int channel)
{
	switch(channel)
	{
		case 0:
		return 4.20f;	
		
		case 1:
		return adcarray[1];
		
		default:			
	  return 0;
	}
	
	
}


#endif
