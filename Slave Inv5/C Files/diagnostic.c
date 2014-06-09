#include "diagnostic.h"

void diag_store_data(void)
{
	if(Diagnostic_Data.enable && Diagnostic_Data.cnt < DIAG_BUF_CNT_LIM)
	{
		Diagnostic_Data.Ph1_Is[DIAG_MEAN] += ADC_Measurements.Ph1_Is;
		Diagnostic_Data.Ph2_Is[DIAG_MEAN] += ADC_Measurements.Ph2_Is;
		Diagnostic_Data.Ph3_Is[DIAG_MEAN] += ADC_Measurements.Ph3_Is;
		Diagnostic_Data.Ph4_Is[DIAG_MEAN] += ADC_Measurements.Ph4_Is;
		Diagnostic_Data.Vdc1[DIAG_MEAN] += ADC_Measurements.Vdc1;
		Diagnostic_Data.Vdc2[DIAG_MEAN] += ADC_Measurements.Vdc2;

		Diagnostic_Data.IPhase1[DIAG_MEAN] += logic_data.measurement[LOGIC_PH1_MEAS];
		Diagnostic_Data.IPhase2[DIAG_MEAN] += logic_data.measurement[LOGIC_PH2_MEAS];
		Diagnostic_Data.IPhase3[DIAG_MEAN] += logic_data.measurement[LOGIC_PH3_MEAS];
		Diagnostic_Data.IPhase4[DIAG_MEAN] += logic_data.measurement[LOGIC_PH4_MEAS];

		Diagnostic_Data.Period[DIAG_MEAN] += TIM_PWM_Data.period;
		Diagnostic_Data.VPhase[DIAG_MEAN] += TIM_PWM_Data.phase;
		Diagnostic_Data.Temp1 += ADC_Measurements.Temp1;
		Diagnostic_Data.Temp2 += ADC_Measurements.Temp2;

		if(ADC_Measurements.Ph1_Is > Diagnostic_Data.Ph1_Is[DIAG_MAX]) Diagnostic_Data.Ph1_Is[DIAG_MAX] = ADC_Measurements.Ph1_Is;
		if(ADC_Measurements.Ph2_Is > Diagnostic_Data.Ph2_Is[DIAG_MAX]) Diagnostic_Data.Ph2_Is[DIAG_MAX] = ADC_Measurements.Ph2_Is;
		if(ADC_Measurements.Ph3_Is > Diagnostic_Data.Ph3_Is[DIAG_MAX]) Diagnostic_Data.Ph3_Is[DIAG_MAX] = ADC_Measurements.Ph3_Is;
		if(ADC_Measurements.Ph4_Is > Diagnostic_Data.Ph4_Is[DIAG_MAX]) Diagnostic_Data.Ph4_Is[DIAG_MAX] = ADC_Measurements.Ph4_Is;
		if(ADC_Measurements.Vdc1 > Diagnostic_Data.Vdc1[DIAG_MAX]) Diagnostic_Data.Vdc1[DIAG_MAX] = ADC_Measurements.Vdc1;
		if(ADC_Measurements.Vdc2 > Diagnostic_Data.Vdc2[DIAG_MAX]) Diagnostic_Data.Vdc2[DIAG_MAX] = ADC_Measurements.Vdc2;

		if(logic_data.measurement[LOGIC_PH1_MEAS] > Diagnostic_Data.IPhase1[DIAG_MAX]) Diagnostic_Data.IPhase1[DIAG_MAX] = logic_data.measurement[LOGIC_PH1_MEAS];
		if(logic_data.measurement[LOGIC_PH2_MEAS] > Diagnostic_Data.IPhase2[DIAG_MAX]) Diagnostic_Data.IPhase2[DIAG_MAX] = logic_data.measurement[LOGIC_PH2_MEAS];
		if(logic_data.measurement[LOGIC_PH3_MEAS] > Diagnostic_Data.IPhase3[DIAG_MAX]) Diagnostic_Data.IPhase3[DIAG_MAX] = logic_data.measurement[LOGIC_PH3_MEAS];
		if(logic_data.measurement[LOGIC_PH4_MEAS] > Diagnostic_Data.IPhase4[DIAG_MAX]) Diagnostic_Data.IPhase4[DIAG_MAX] = logic_data.measurement[LOGIC_PH4_MEAS];

		if(TIM_PWM_Data.period > Diagnostic_Data.Period[DIAG_MAX]) Diagnostic_Data.Period[DIAG_MAX] = TIM_PWM_Data.period;
		if(TIM_PWM_Data.phase > Diagnostic_Data.VPhase[DIAG_MAX]) Diagnostic_Data.VPhase[DIAG_MAX] = TIM_PWM_Data.phase;

		if(ADC_Measurements.Ph1_Is < Diagnostic_Data.Ph1_Is[DIAG_MIN]) Diagnostic_Data.Ph1_Is[DIAG_MIN] = ADC_Measurements.Ph1_Is;
		if(ADC_Measurements.Ph2_Is < Diagnostic_Data.Ph2_Is[DIAG_MIN]) Diagnostic_Data.Ph2_Is[DIAG_MIN] = ADC_Measurements.Ph2_Is;
		if(ADC_Measurements.Ph3_Is < Diagnostic_Data.Ph3_Is[DIAG_MIN]) Diagnostic_Data.Ph3_Is[DIAG_MIN] = ADC_Measurements.Ph3_Is;
		if(ADC_Measurements.Ph4_Is < Diagnostic_Data.Ph4_Is[DIAG_MIN]) Diagnostic_Data.Ph4_Is[DIAG_MIN] = ADC_Measurements.Ph4_Is;
		if(ADC_Measurements.Vdc1 < Diagnostic_Data.Vdc1[DIAG_MIN]) Diagnostic_Data.Vdc1[DIAG_MIN] = ADC_Measurements.Vdc1;
		if(ADC_Measurements.Vdc2 < Diagnostic_Data.Vdc2[DIAG_MIN]) Diagnostic_Data.Vdc2[DIAG_MIN] = ADC_Measurements.Vdc2;

		if(logic_data.measurement[LOGIC_PH1_MEAS] < Diagnostic_Data.IPhase1[DIAG_MIN]) Diagnostic_Data.IPhase1[DIAG_MIN] = logic_data.measurement[LOGIC_PH1_MEAS];
		if(logic_data.measurement[LOGIC_PH2_MEAS] < Diagnostic_Data.IPhase2[DIAG_MIN]) Diagnostic_Data.IPhase2[DIAG_MIN] = logic_data.measurement[LOGIC_PH2_MEAS];
		if(logic_data.measurement[LOGIC_PH3_MEAS] < Diagnostic_Data.IPhase3[DIAG_MIN]) Diagnostic_Data.IPhase3[DIAG_MIN] = logic_data.measurement[LOGIC_PH3_MEAS];
		if(logic_data.measurement[LOGIC_PH4_MEAS] < Diagnostic_Data.IPhase4[DIAG_MIN]) Diagnostic_Data.IPhase4[DIAG_MIN] = logic_data.measurement[LOGIC_PH4_MEAS];

		if(TIM_PWM_Data.period < Diagnostic_Data.Period[DIAG_MIN]) Diagnostic_Data.Period[DIAG_MIN] = TIM_PWM_Data.period;
		if(TIM_PWM_Data.phase < Diagnostic_Data.VPhase[DIAG_MIN]) Diagnostic_Data.VPhase[DIAG_MIN] = TIM_PWM_Data.phase;

		Diagnostic_Data.cnt++;
	}
}

void diag_reset_data(void)
{
	Diagnostic_Data.Ph1_Is[DIAG_MEAN] = 0;
	Diagnostic_Data.Ph2_Is[DIAG_MEAN] = 0;
	Diagnostic_Data.Ph3_Is[DIAG_MEAN] = 0;
	Diagnostic_Data.Ph4_Is[DIAG_MEAN] = 0;
	Diagnostic_Data.Vdc1[DIAG_MEAN] = 0;
	Diagnostic_Data.Vdc2[DIAG_MEAN] = 0;

	Diagnostic_Data.IPhase1[DIAG_MEAN] = 0;
	Diagnostic_Data.IPhase2[DIAG_MEAN] = 0;
	Diagnostic_Data.IPhase3[DIAG_MEAN] = 0;
	Diagnostic_Data.IPhase4[DIAG_MEAN] = 0;

	Diagnostic_Data.Period[DIAG_MEAN] = 0;
	Diagnostic_Data.VPhase[DIAG_MEAN] = 0;

	Diagnostic_Data.Temp1 = 0;
	Diagnostic_Data.Temp2 = 0;

	Diagnostic_Data.Ph1_Is[DIAG_MAX] = 0;
	Diagnostic_Data.Ph2_Is[DIAG_MAX] = 0;
	Diagnostic_Data.Ph3_Is[DIAG_MAX] = 0;
	Diagnostic_Data.Ph4_Is[DIAG_MAX] = 0;
	Diagnostic_Data.Vdc1[DIAG_MAX] = 0;
	Diagnostic_Data.Vdc2[DIAG_MAX] = 0;

	Diagnostic_Data.IPhase1[DIAG_MAX] = 0;
	Diagnostic_Data.IPhase2[DIAG_MAX] = 0;
	Diagnostic_Data.IPhase3[DIAG_MAX] = 0;
	Diagnostic_Data.IPhase4[DIAG_MAX] = 0;

	Diagnostic_Data.Period[DIAG_MAX] = 0;
	Diagnostic_Data.VPhase[DIAG_MAX] = 0;

	Diagnostic_Data.Ph1_Is[DIAG_MIN] = 0x7FFFFFFF;
	Diagnostic_Data.Ph2_Is[DIAG_MIN] = 0x7FFFFFFF;
	Diagnostic_Data.Ph3_Is[DIAG_MIN] = 0x7FFFFFFF;
	Diagnostic_Data.Ph4_Is[DIAG_MIN] = 0x7FFFFFFF;
	Diagnostic_Data.Vdc1[DIAG_MIN] = 0x7FFFFFFF;
	Diagnostic_Data.Vdc2[DIAG_MIN] = 0x7FFFFFFF;

	Diagnostic_Data.IPhase1[DIAG_MIN] = 0x7FFFFFFF;
	Diagnostic_Data.IPhase2[DIAG_MIN] = 0x7FFFFFFF;
	Diagnostic_Data.IPhase3[DIAG_MIN] = 0x7FFFFFFF;
	Diagnostic_Data.IPhase4[DIAG_MIN] = 0x7FFFFFFF;

	Diagnostic_Data.Period[DIAG_MIN] = 0x7FFFFFFF;
	Diagnostic_Data.VPhase[DIAG_MIN] = 0x7FFFFFFF;

	Diagnostic_Data.cnt = 0;

	Diagnostic_Data.enable = 1;
}
void diag_get_data(void)
{
	Diagnostic_Data.enable = 0;

	Diagnostic_Data.Ph1_Is[DIAG_MEAN] /= Diagnostic_Data.cnt;
	Diagnostic_Data.Ph2_Is[DIAG_MEAN] /= Diagnostic_Data.cnt;
	Diagnostic_Data.Ph3_Is[DIAG_MEAN] /= Diagnostic_Data.cnt;
	Diagnostic_Data.Ph4_Is[DIAG_MEAN] /= Diagnostic_Data.cnt;
	Diagnostic_Data.Vdc1[DIAG_MEAN] /= Diagnostic_Data.cnt;
	Diagnostic_Data.Vdc2[DIAG_MEAN] /= Diagnostic_Data.cnt;

	Diagnostic_Data.IPhase1[DIAG_MEAN] /= Diagnostic_Data.cnt;
	Diagnostic_Data.IPhase2[DIAG_MEAN] /= Diagnostic_Data.cnt;
	Diagnostic_Data.IPhase3[DIAG_MEAN] /= Diagnostic_Data.cnt;
	Diagnostic_Data.IPhase4[DIAG_MEAN] /= Diagnostic_Data.cnt;

	Diagnostic_Data.Period[DIAG_MEAN] /= Diagnostic_Data.cnt;
	Diagnostic_Data.VPhase[DIAG_MEAN] /= Diagnostic_Data.cnt;

	Diagnostic_Data.Temp1 /= Diagnostic_Data.cnt;
	Diagnostic_Data.Temp2 /= Diagnostic_Data.cnt;

	Diagnostic_Data.Faults = logic_data.all_faults;
	Diagnostic_Data.State = TIM_PWM_Data.state;
}

void diag_store_buffer(s32 A, s32 B)
{
	if(Diagnostic_Data.benable)
	{
		Diagnostic_Data.buffer1[Diagnostic_Data.bcnt] = A;
		Diagnostic_Data.buffer2[Diagnostic_Data.bcnt] = B;

		Diagnostic_Data.bcnt++;

		if(Diagnostic_Data.bcnt >= DIAG_BUF_SIZE)
		{
			diag_reset_buffer();
		}
	}
}

void diag_reset_buffer(void)
{
	Diagnostic_Data.benable = 0;
	Diagnostic_Data.bcnt = 0;
}
