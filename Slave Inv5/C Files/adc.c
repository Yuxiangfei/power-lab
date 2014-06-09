#include "adc.h"

//Initiate ADC
void adc1_init(void)
{
	u32 i;

	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;

	for(i = 0; i < ADC1_BUF_LEN * ADC1_MEAN_CNT; i++)
	{
		ADC_Datas.ADC1BUF[i] = 0;
	}


	/* DMA2 Stream0 configuration ----------------------------------------------*/
	DMA_InitStructure.DMA_Channel = DMA_Channel_0;
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)ADC_Datas.ADC1BUF;
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = ADC1_BUF_LEN * ADC1_MEAN_CNT;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA2_Stream0, &DMA_InitStructure);

	/* DMA2_Stream0 enable */
	DMA_Cmd(DMA2_Stream0, ENABLE);

	/* ADC1 configuration ------------------------------------------------------*/
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure);

	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T8_TRGO;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = ADC1_BUF_LEN;
	ADC_Init(ADC1, &ADC_InitStructure);

	/* ADC1 regular channels configuration */
	ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 	1, 	ADC_SampleTime_3Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 	2, 	ADC_SampleTime_3Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 	3, 	ADC_SampleTime_3Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 	4, 	ADC_SampleTime_3Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 	5, 	ADC_SampleTime_3Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 	6, 	ADC_SampleTime_3Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 	7, 	ADC_SampleTime_3Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 	8, 	ADC_SampleTime_3Cycles);

	ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);

	/* Enable ADC1 DMA */
	ADC_DMACmd(ADC1, ENABLE);

	DMA_ITConfig(DMA2_Stream0, DMA_IT_TC, ENABLE);

	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);

	/* Start ADC1 Software Conversion */
	ADC_SoftwareStartConv(ADC1);
}

//get mean adc data from selected channel
u32 adc1_get_mean(u8 channel)
{
	u8 i;
	u16 offset;
	u32 tmp = 0;

	offset = channel;

	for(i = 0; i < ADC1_MEAN_CNT; i++)
	{
		tmp += ADC_Datas.ADC1BUF[offset];
		offset += ADC1_BUF_LEN;
	}

	tmp /= ADC1_MEAN_CNT;

	return tmp;
}

//calculate voltage in mV
u32 adc_get_voltage(u32 value, u32 k)
{
	value *= k;
	value /= 1000;
	value *= ADC_REF;
	value /= ADC_MAX;

	return value;
}

//get current measurements
void adc_calc_currents(void)
{
	ADC_Measurements.Ph1_Is = adc_get_voltage(adc1_get_mean(ADC1_PH1_IS_CH), 2038);
	ADC_Measurements.Ph2_Is = adc_get_voltage(adc1_get_mean(ADC1_PH2_IS_CH), 2038);
	ADC_Measurements.Ph3_Is = adc_get_voltage(adc1_get_mean(ADC1_PH3_IS_CH), 2038);
	ADC_Measurements.Ph4_Is = adc_get_voltage(adc1_get_mean(ADC1_PH4_IS_CH), 2038);
}

//get voltage measurements
void adc_calc_voltages(void)
{
	ADC_Measurements.Vdc1 = adc_get_voltage(adc1_get_mean(ADC1_VDC1_CH), 1586);
	ADC_Measurements.Vdc2 = adc_get_voltage(adc1_get_mean(ADC1_VDC2_CH), 1702);
}

//get temp measurements
void adc_calc_temp(void)
{
	ADC_Measurements.Temp1 = adc_get_voltage(adc1_get_mean(ADC1_TEMP1_CH), 1000);
	ADC_Measurements.Temp2 = adc_get_voltage(adc1_get_mean(ADC1_TEMP2_CH), 1000);
}
