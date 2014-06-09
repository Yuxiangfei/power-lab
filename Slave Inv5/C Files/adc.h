#ifndef __ADC_H__
#define __ADC_H__

#include "stm32f4xx_adc.h"
#include "stm32f4xx_dma.h"

#define ADC1_DR_Address    	((u32)0x4001204C)
#define ADC3_DR_ADDRESS     ((u32)0x4001224C)

#define ADC1_MEAN_CNT		8

#define ADC1_BUF_LEN		8

//ADC channels numbers-------------------------------------------------
#define ADC1_PH1_IS_CH		0
#define ADC1_PH2_IS_CH		1
#define ADC1_PH3_IS_CH		2
#define ADC1_PH4_IS_CH		3
#define ADC1_VDC1_CH		4
#define ADC1_VDC2_CH		5
#define ADC1_TEMP1_CH		6
#define ADC1_TEMP2_CH		7

//ADC constants for calculations---------------------------------------
//multipliers
#define ADC1_PH1_IS_N		2500
#define ADC1_PH2_IS_N		2500
#define ADC1_PH3_IS_N		2500
#define ADC1_PH4_IS_N		2500
#define ADC1_VDC1_N			2500
#define ADC1_VDC2_N			2500
#define ADC1_TEMP1_N		2500
#define ADC1_TEMP2_N		2500

#define ADC1_PH1_IS_D		4096
#define ADC1_PH2_IS_D		4096
#define ADC1_PH3_IS_D		4096
#define ADC1_PH4_IS_D		4096
#define ADC1_VDC1_D		    4096
#define ADC1_VDC2_D		    4096
#define ADC1_TEMP1_D		4096
#define ADC1_TEMP2_D		4096

//ADC Vref in mV
#define ADC_REF				2500
//ADC max value
#define ADC_MAX				4095

extern volatile u16 ADC3ConvertedValue;

typedef struct
{
  u32	Ph1_Is;
  u32	Ph2_Is;
  u32	Ph3_Is;
  u32	Ph4_Is;

  u32	Vdc1;
  u32	Vdc2;

  s32	Temp1;
  s32	Temp2;
} ADC_Measurements_Type;

typedef struct
{
  u16 	ADC1BUF[ADC1_BUF_LEN * ADC1_MEAN_CNT];
} ADC_Datas_Type;

volatile extern ADC_Datas_Type ADC_Datas;
volatile extern ADC_Measurements_Type ADC_Measurements;

//Initiate ADC
void adc1_init(void);

//get mean adc data from selected channel
u32 adc1_get_mean(u8 channel);

//calculate voltage in mV
u32 adc_get_voltage(u32 value, u32 k);

//get current measurements
void adc_calc_currents(void);

//get voltage measurements
void adc_calc_voltages(void);

//get temp measurements
void adc_calc_temp(void);

#endif
