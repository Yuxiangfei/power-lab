#include "timer.h"

void timer_pwm_init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	TIM_BDTRInitTypeDef TIM_BDTRInitStructure;

	TIM_PWM_Data.period = (FPGA_CLOCK / TIMER_PWM_INIT_FREQ) - 2;

//TIMER8 setup for FPGA clock

	/* Time Base configuration */
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = 7;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);

	/* Channel 1, 2 and 3 Configuration in PWM mode */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	TIM_OCInitStructure.TIM_Pulse = 4;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

	TIM_OC3Init(TIM8, &TIM_OCInitStructure);

	/* Automatic Output enable, Break, dead time and lock configuration*/
	TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;
	TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;
	TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_1;
	TIM_BDTRInitStructure.TIM_DeadTime = 0;
	TIM_BDTRInitStructure.TIM_Break = TIM_Break_Disable;
	TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High;
	TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;

	TIM_BDTRConfig(TIM8, &TIM_BDTRInitStructure);

	/* TIM1 counter enable */
	TIM_Cmd(TIM8, ENABLE);

	/* Main Output Enable */
	TIM_CtrlPWMOutputs(TIM8, ENABLE);

	TIM_PWM_Data.phase = 0;
	logic_data.out_enable = 0;
	logic_calc_out(TIM_PWM_Data.period, TIM_PWM_Data.phase);
	logic_set_out();
}

void timer_pwm_freq_inc(u16 value)
{
	if(TIM_PWM_Data.state != TIMER_STATE_RUN2)
	{
		return;
	}

	if(TIM_PWM_Data.period > value)
	{
		TIM_PWM_Data.period -= value;
	}

	if(TIM_PWM_Data.period < TIMER_PWM_PMIN)
	{
		TIM_PWM_Data.period = TIMER_PWM_PMIN;
	}

	logic_calc_out(TIM_PWM_Data.period, TIM_PWM_Data.phase);
	logic_set_out();
}

void timer_pwm_freq_dec(u16 value)
{
	if(TIM_PWM_Data.state != TIMER_STATE_RUN2)
	{
		return;
	}

	if(TIM_PWM_Data.period < TIMER_PWM_PMAX)
	{
		TIM_PWM_Data.period += value;

		if(TIM_PWM_Data.period > TIMER_PWM_PMAX)
		{
			TIM_PWM_Data.period = TIMER_PWM_PMAX;
		}
	}

	logic_calc_out(TIM_PWM_Data.period, TIM_PWM_Data.phase);
	logic_set_out();
}

void timer_pwm_phase_inc(u16 value)
{
	if(TIM_PWM_Data.state != TIMER_STATE_RUN2)
	{
		return;
	}

	TIM_PWM_Data.phase += value;

	if(TIM_PWM_Data.phase > TIM_PWM_Data.period / 2)
	{
		TIM_PWM_Data.phase = TIM_PWM_Data.period / 2;
	}

	logic_calc_out(TIM_PWM_Data.period, TIM_PWM_Data.phase);
	logic_set_out();
}

void timer_pwm_phase_dec(u16 value)
{
	if(TIM_PWM_Data.state != TIMER_STATE_RUN2)
	{
		return;
	}

	if(TIM_PWM_Data.phase > value)
	{
		TIM_PWM_Data.phase -= value;
	}
	else
	{
		TIM_PWM_Data.phase = 0;
	}

	logic_calc_out(TIM_PWM_Data.period, TIM_PWM_Data.phase);
	logic_set_out();
}


u8 timer_check_faults(void)
{
	if(logic_check_fault())
	{
		logic_read_flags();
		TIM_PWM_Data.state = TIMER_STATE_EXT_FLT;
		return 1;
	}
/*	else if(ADC_Measurements.Ph1_Is1 > TIMER_IPH_LIMIT)
	{
		TIM_PWM_Data.state = TIMER_STATE_IPH1_FLT;
		return 1;
	}
	else if(ADC_Measurements.Ph1_Is2 > TIMER_IPH_LIMIT)
	{
		TIM_PWM_Data.state = TIMER_STATE_IPH1_FLT;
		return 1;
	}
	else if(ADC_Measurements.Ph2_Is1 > TIMER_IPH_LIMIT)
	{
		TIM_PWM_Data.state = TIMER_STATE_IPH2_FLT;
		return 1;
	}
	else if(ADC_Measurements.Ph2_Is2 > TIMER_IPH_LIMIT)
	{
		TIM_PWM_Data.state = TIMER_STATE_IPH2_FLT;
		return 1;
	}
	else if(ADC_Measurements.Ph3_Is1 > TIMER_IPH_LIMIT)
	{
		TIM_PWM_Data.state = TIMER_STATE_IPH3_FLT;
		return 1;
	}
	else if(ADC_Measurements.Ph3_Is2 > TIMER_IPH_LIMIT)
	{
		TIM_PWM_Data.state = TIMER_STATE_IPH3_FLT;
		return 1;
	}*/
	else if(ADC_Measurements.Temp1 > TIMER_TEMP_LIMIT)
	{
		TIM_PWM_Data.state = TIMER_STATE_TEMP_FLT;
		return 1;
	}
	else if(ADC_Measurements.Temp2 > TIMER_TEMP_LIMIT)
	{
		TIM_PWM_Data.state = TIMER_STATE_TEMP_FLT;
		return 1;
	}

	return 0;
}

void timer_routine(void)
{
	switch(TIM_PWM_Data.state)
	{
		case TIMER_STATE_OFF:
			logic_data.out_enable = 0;
			TIM_PWM_Data.period = TIMER_PWM_INIT_PERIOD;
			TIM_PWM_Data.phase = 0;
			logic_calc_out(TIM_PWM_Data.period, TIM_PWM_Data.phase);
			logic_set_out();

			if(timer_check_faults() == 0)
			{
				if(TIM_PWM_Data.cmd == TIMER_CMD_ENABLE)
				{
					logic_data.out_enable = 1;
					logic_set_out();

					TIM_PWM_Data.state = TIMER_STATE_RUN1;
				}
				else if(TIM_PWM_Data.cmd == TIMER_CMD_RESET)
				{
					logic_reset();
					logic_read_flags();
				}
			}
			break;
		case TIMER_STATE_RUN1:
			if(timer_check_faults() == 0)
			{
				if(TIM_PWM_Data.cmd == TIMER_CMD_DISABLE)
				{
					TIM_PWM_Data.state = TIMER_STATE_OFF;
				}
				else if(TIM_PWM_Data.cmd == TIMER_CMD_RUN)
				{
					TIM_PWM_Data.state = TIMER_STATE_RUN2;
				}
			}
			break;
		case TIMER_STATE_RUN2:
			if(timer_check_faults() == 0)
			{
				if(TIM_PWM_Data.cmd == TIMER_CMD_DISABLE)
				{
					TIM_PWM_Data.state = TIMER_STATE_OFF;
				}
			}
			break;
		case TIMER_STATE_EXT_FLT:
			logic_data.out_enable = 0;
			logic_set_out();

			if(TIM_PWM_Data.cmd == TIMER_CMD_RESET)
			{
				logic_reset();
				logic_read_flags();

				if(timer_check_faults() == 0)
				{
					TIM_PWM_Data.state = TIMER_STATE_OFF;
				}
			}
			break;
		case  TIMER_STATE_IPH1_FLT:
			logic_data.out_enable = 0;
			logic_set_out();

			if(TIM_PWM_Data.cmd == TIMER_CMD_RESET)
			{
				logic_reset();
				logic_read_flags();

				if(timer_check_faults() == 0)
				{
					TIM_PWM_Data.state = TIMER_STATE_OFF;
				}
			}
			break;
		case  TIMER_STATE_IPH2_FLT:
			logic_data.out_enable = 0;
			logic_set_out();

			if(TIM_PWM_Data.cmd == TIMER_CMD_RESET)
			{
				logic_reset();
				logic_read_flags();

				if(timer_check_faults() == 0)
				{
					TIM_PWM_Data.state = TIMER_STATE_OFF;
				}
			}
			break;
		case  TIMER_STATE_IPH3_FLT:
			logic_data.out_enable = 0;
			logic_set_out();

			if(TIM_PWM_Data.cmd == TIMER_CMD_RESET)
			{
				logic_reset();
				logic_read_flags();

				if(timer_check_faults() == 0)
				{
					TIM_PWM_Data.state = TIMER_STATE_OFF;
				}
			}
			break;
		case  TIMER_STATE_TEMP_FLT:
			logic_data.out_enable = 0;
			logic_set_out();

			if(TIM_PWM_Data.cmd == TIMER_CMD_RESET)
			{
				logic_reset();
				logic_read_flags();

				if(timer_check_faults() == 0)
				{
					TIM_PWM_Data.state = TIMER_STATE_OFF;
				}
			}
			break;
		default:
			TIM_PWM_Data.state = TIMER_STATE_OFF;
			break;
	}

	TIM_PWM_Data.cmd = 0;
}

void TIM1_UP_TIM10_IRQHandler(void)
{
	TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
}

void TIM8_UP_TIM13_IRQHandler(void)
{
	TIM_ClearITPendingBit(TIM8, TIM_IT_Update);
}

void DMA2_Stream0_IRQHandler(void)
{
	DMA_ClearITPendingBit(DMA2_Stream0, DMA_FLAG_TCIF0);

	adc_calc_currents();
	adc_calc_voltages();
	adc_calc_temp();

	Diagnostic_Data.cnt_check++;
}

//initiate system timer	for 1ms
void TimerSys_init(void)
{
	SysTick_Config(SystemCoreClock / 1000);
}

void SysTick_Handler(void)
{
	static u8 cnt10 = 0;
	static u8 cnt100 = 5;
	static u8 cnt200 = 50;
	static u16 cnt1000 = 500;

  	TIM_Sys_Flags.Time1ms = 1;
	cnt10++;
	cnt100++;
	cnt200++;
	cnt1000++;

	diag_store_data();
	logic_routine();

	//executed every 10ms
	if(cnt10 >= 10)
	{
		cnt10 = 0;
		TIM_Sys_Flags.Time10ms = 1;
	}

	//executed every 100ms
	if(cnt100 >= 100)
	{
		cnt100 = 0;
		TIM_Sys_Flags.Time100ms = 1;
	}

	//executed every 100ms
	if(cnt200 >= 200)
	{
		cnt200 = 0;
		TIM_Sys_Flags.TimeDisp200ms = 1;
	}

	//executed every 1000ms
	if(cnt1000 >= 1000)
	{
		Diagnostic_Data.CntVal = Diagnostic_Data.cnt_check;
		Diagnostic_Data.cnt_check = 0;

		cnt1000 = 0;
		TIM_Sys_Flags.Time1000ms = 1;
	}
}
