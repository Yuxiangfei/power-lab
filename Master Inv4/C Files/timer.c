#include "timer.h"
#include "math.h"
#define PI 3.1415926535897932384626433832795

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
	if(TIM_PWM_Data.state != TIMER_STATE_RUN2 && TIM_PWM_Data.state != TIMER_STATE_RUN3)
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
	if(TIM_PWM_Data.state != TIMER_STATE_RUN2 && TIM_PWM_Data.state != TIMER_STATE_RUN3)
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
	if(TIM_PWM_Data.state != TIMER_STATE_RUN2 && TIM_PWM_Data.state != TIMER_STATE_RUN3)
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
	if(TIM_PWM_Data.state != TIMER_STATE_RUN2 && TIM_PWM_Data.state != TIMER_STATE_RUN3)
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
//	else if(ADC_Measurements.Temp1 > TIMER_TEMP_LIMIT)
//	{
//		TIM_PWM_Data.state = TIMER_STATE_TEMP_FLT;
//		return 1;
//	}
//	else if(ADC_Measurements.Temp2 > TIMER_TEMP_LIMIT)
//	{
//		TIM_PWM_Data.state = TIMER_STATE_TEMP_FLT;
//		return 1;
//	}

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
			TIM_Control_Var.precision = TIMER_PWM_INIT_PREC; // Precision of controller constants
			TIM_Control_Var.kp_f = 5;       // 5
			TIM_Control_Var.ki_f = 10;      // 10
			TIM_Control_Var.kp_ph = 0.0003; // 0.0003
			TIM_Control_Var.ki_ph = 0.025;  // 0.025
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
				else if(TIM_PWM_Data.cmd == TIMER_CMD_RUN)
				{
					TIM_PWM_Data.state = TIMER_STATE_RUN3;
				}
			}
			break;
		case TIMER_STATE_RUN3:
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

// Controller code starts here...

void timer_pwm_freq_control(float new_value)
{
	static u16 new_value_t;

	new_value_t = FPGA_CLOCK / new_value - 2; // Convert input into period

	if(TIM_PWM_Data.state != TIMER_STATE_RUN3)
	{
		return;
	}

	if(TIMER_PWM_PMIN < new_value_t && new_value_t < TIMER_PWM_PMAX)
	{
		TIM_PWM_Data.period = new_value_t;
	}
	else if(new_value_t <= TIMER_PWM_PMIN)
	{
		TIM_PWM_Data.period = TIMER_PWM_PMIN;
	}
	else if(new_value_t >= TIMER_PWM_PMAX)
	{
		TIM_PWM_Data.period = TIMER_PWM_PMAX;
	}

	logic_calc_out(TIM_PWM_Data.period, TIM_PWM_Data.phase);
	logic_set_out();
}

void timer_pwm_phase_control(u16 new_value)
{
	if(TIM_PWM_Data.state != TIMER_STATE_RUN3)
	{
		return;
	}

	if(0 < new_value && new_value < TIM_PWM_Data.period / 2)
	{
		TIM_PWM_Data.phase = new_value;
	}
	else if (new_value <= 0)
	{
		TIM_PWM_Data.phase = 0;
	}
	else if(new_value >= TIM_PWM_Data.period / 2)
	{
		TIM_PWM_Data.phase = TIM_PWM_Data.period / 2;
	}

	logic_calc_out(TIM_PWM_Data.period, TIM_PWM_Data.phase);
	logic_set_out();
}

// controller tuning
void timer_pwm_control_prec_inc(void)
{
	if (TIM_Control_Var.precision < 1000)
		TIM_Control_Var.precision *= 10;
	else if (TIM_Control_Var.precision >= 1000)
		TIM_Control_Var.precision = 1000;
}
void timer_pwm_control_prec_dec(void)
{
	if (TIM_Control_Var.precision > 0.0001)
		TIM_Control_Var.precision /= 10;
	else if (TIM_Control_Var.precision <= 0.0001)
		TIM_Control_Var.precision = 0.0001;
}
void timer_pwm_control_kp_f_inc(void)
{
	if (TIM_Control_Var.kp_f < 10000)
		TIM_Control_Var.kp_f += TIM_Control_Var.precision;
	else if (TIM_Control_Var.kp_f >= 10000)
		TIM_Control_Var.kp_f = 10000;
}
void timer_pwm_control_kp_f_dec(void)
{
	if (TIM_Control_Var.kp_f >= 0.0001)
		TIM_Control_Var.kp_f -= TIM_Control_Var.precision;
	else if (TIM_Control_Var.kp_f < 0.0001)
		TIM_Control_Var.kp_f = 0.0001;
}
void timer_pwm_control_ki_f_inc(void)
{
	if (TIM_Control_Var.ki_f < 10000)
		TIM_Control_Var.ki_f += TIM_Control_Var.precision;
	else if (TIM_Control_Var.ki_f >= 10000)
		TIM_Control_Var.ki_f = 10000;
}
void timer_pwm_control_ki_f_dec(void)
{
	if (TIM_Control_Var.ki_f > 0.0001)
		TIM_Control_Var.ki_f -= TIM_Control_Var.precision;
	else if (TIM_Control_Var.ki_f <= 0.0001)
		TIM_Control_Var.ki_f = 0.0001;
}
void timer_pwm_control_kp_ph_inc(void)
{
	if (TIM_Control_Var.kp_ph < 10000)
		TIM_Control_Var.kp_ph += TIM_Control_Var.precision;
	else if (TIM_Control_Var.kp_ph >= 10000)
		TIM_Control_Var.kp_ph = 10000;
}
void timer_pwm_control_kp_ph_dec(void)
{
	if (TIM_Control_Var.kp_ph > 0.0001)
		TIM_Control_Var.kp_ph -= TIM_Control_Var.precision;
	else if (TIM_Control_Var.kp_ph <= 0.0001)
		TIM_Control_Var.kp_ph = 0.0001;
}
void timer_pwm_control_ki_ph_inc(void)
{
	if (TIM_Control_Var.ki_ph < 10000)
		TIM_Control_Var.ki_ph += TIM_Control_Var.precision;
	else if (TIM_Control_Var.ki_ph >= 10000)
		TIM_Control_Var.ki_ph = 10000;
}
void timer_pwm_control_ki_ph_dec(void)
{
	if (TIM_Control_Var.ki_ph > 0.0001)
		TIM_Control_Var.ki_ph -= TIM_Control_Var.precision;
	else if (TIM_Control_Var.ki_ph <= 0.0001)
		TIM_Control_Var.ki_ph = 0.0001;
}

float phasor_sum_magnitude_control(float i1, float i2, float i_ph_1, float i_ph_2, float v_ph, float T)
{
	float i1h;
	float i1v;
	float i2h;
	float i2v;
	float I12;

	i1h = i1 * cos((i_ph_1) * 2 * PI / T);
	i1v = i1 * sin((i_ph_1) * 2 * PI / T);
	i2h = i2 * cos((i_ph_2 + v_ph) * 2 * PI / T);
	i2v = i2 * sin((i_ph_2 + v_ph) * 2 * PI / T);

	I12 = sqrt((i1h + i2h) * (i1h + i2h) + (i1v + i2v) * (i1v + i2v));

	return I12;
}

float phasor_sum_phase_control(float i1, float i2, float i_ph_1, float i_ph_2, float v_ph, float T)
{
	float i1h;
	float i1v;
	float i2h;
	float i2v;
	float I_ph_12;

	i1h = i1 * cos((i_ph_1) * 2 * PI / T);
	i1v = i1 * sin((i_ph_1) * 2 * PI / T);
	i2h = i2 * cos((i_ph_2 + v_ph) * 2 * PI / T);
	i2v = i2 * sin((i_ph_2 + v_ph) * 2 * PI / T);

	if ((i1h + i2h) > 0 && (i1v + i2v) > 0)
		I_ph_12 = (atan((i1v + i2v) / (i1h + i2h)) * T / (2 * PI));
	else if ((i1h + i2h) < 0 && (i1v + i2v) > 0)
		I_ph_12 = ((PI - atan((i1v + i2v) / (-i1h - i2h))) * T / (2 * PI));
	else if ((i1h + i2h) < 0 && (i1v + i2v) < 0)
		I_ph_12 = ((PI + atan((i1v + i2v) / (i1h + i2h))) * T / (2 * PI));
	else
		I_ph_12 = ((2 * PI - atan((-i1v - i2v) / (i1h + i2h))) * T / (2 * PI));

	return I_ph_12;
}

float phasor_sum_magnitude(float i1, float i2, float i_ph_1, float i_ph_2, float T)
{
	float i1h;
	float i1v;
	float i2h;
	float i2v;
	float I12;

	i1h = i1 * cos(i_ph_1 * 2 * PI / T);
	i1v = i1 * sin(i_ph_1 * 2 * PI / T);
	i2h = i2 * cos(i_ph_2 * 2 * PI / T);
	i2v = i2 * sin(i_ph_2 * 2 * PI / T);

	I12 = sqrt((i1h + i2h) * (i1h + i2h) + (i1v + i2v) * (i1v + i2v));

	return I12;
}

float phasor_sum_phase(float i1, float i2, float i_ph_1, float i_ph_2, float T)
{
	float i1h;
	float i1v;
	float i2h;
	float i2v;
	float I_ph_12;

	i1h = i1 * cos(i_ph_1 * 2 * PI / T);
	i1v = i1 * sin(i_ph_1 * 2 * PI / T);
	i2h = i2 * cos(i_ph_2 * 2 * PI / T);
	i2v = i2 * sin(i_ph_2 * 2 * PI / T);

	if ((i1h + i2h) > 0 && (i1v + i2v) > 0)
		I_ph_12 = (atan((i1v + i2v) / (i1h + i2h)) * T / (2 * PI));
	else if ((i1h + i2h) < 0 && (i1v + i2v) > 0)
		I_ph_12 = ((PI - atan((i1v + i2v) / (-i1h - i2h))) * T / (2 * PI));
	else if ((i1h + i2h) < 0 && (i1v + i2v) < 0)
		I_ph_12 = ((PI + atan((i1v + i2v) / (i1h + i2h))) * T / (2 * PI));
	else
		I_ph_12 = ((2 * PI - atan((-i1v - i2v) / (i1h + i2h))) * T / (2 * PI));

	return I_ph_12;
}

// Initiate system timer for 0.1ms
void TimerSys_init(void)
{
	SysTick_Config(SystemCoreClock / 10000);
}

void SysTick_Handler(void)
{
	// Timer variables
	static u8 cnt1 = 0;
	static u8 cnt10 = 1;
	static u8 cnt100 = 5;
	static u8 cnt200 = 50;
	static u16 cnt1000 = 500;
	static u16 cnt10000 = 0;

	// Controller variables
	static float Io_ph_ref; // Degrees
	static float Io_mag_ref; // A
	static float f_sw_0;
	static float v_ph_0;
	static float error_freq_0;
	static float error_ph_0;
	static float f_sw_1;
	static float v_ph_1;
	static float error_freq_1;
	static float error_ph_1;
	static u16 bcnt10 = 0;
	static u16 bcnt100 = 0;

  	cnt1++;

	diag_store_data();
	logic_routine();

	//executed every 1ms
	if(cnt1 >= 10)
	{
		cnt1 = 0;
		TIM_Sys_Flags.Time1ms = 1;
		cnt10++;
		cnt100++;
		cnt200++;
		cnt1000++;
		cnt10000++;

//   // Bang–Bang Controller
//
//		if (TIM_PWM_Data.state == TIMER_STATE_RUN3) {
//
//			Io_ph_ref = 120 * ((float)TIM_PWM_Data.period / 360); // Degrees
//			Io_mag_ref = 7 * (100); // Amperes
//
//			bcnt10++;
//			bcnt100++;
//
//			//executed every 10ms
//			if (bcnt10 >= 10) {
//				bcnt10 = 0;
//				if (logic_data.Io_mag > Io_mag_ref) {
//					timer_pwm_phase_dec(1);
//				}
//				else if (logic_data.Io_mag < Io_mag_ref) {
//					timer_pwm_phase_inc(1);
//				}
//			}
//			//executed every 100ms
//			if (bcnt100 >= 100) {
//				bcnt100 = 0;
//				if (logic_data.Io_phase > Io_ph_ref) {
//					timer_pwm_freq_dec(1);
//				}
//				else if (logic_data.Io_phase < Io_ph_ref) {
//					timer_pwm_freq_inc(1);
//				}
//			}
//		}

		//executed every 10ms
		if(cnt10 >= 10)
		{
			cnt10 = 0;
			TIM_Sys_Flags.Time10ms = 1;

			logic_data.I12_mag = phasor_sum_magnitude_control(ADC_Measurements.Ph1_Is, ADC_Measurements.Ph2_Is, logic_data.measurement[LOGIC_PH1_MEAS], logic_data.measurement[LOGIC_PH2_MEAS], TIM_PWM_Data.phase, TIM_PWM_Data.period);
			logic_data.I12_phase = phasor_sum_phase_control(ADC_Measurements.Ph1_Is, ADC_Measurements.Ph2_Is, logic_data.measurement[LOGIC_PH1_MEAS], logic_data.measurement[LOGIC_PH2_MEAS], TIM_PWM_Data.phase, TIM_PWM_Data.period);
			logic_data.I34_mag = phasor_sum_magnitude_control(ADC_Measurements.Ph3_Is, ADC_Measurements.Ph4_Is, logic_data.measurement[LOGIC_PH3_MEAS], logic_data.measurement[LOGIC_PH4_MEAS], TIM_PWM_Data.phase, TIM_PWM_Data.period);
			logic_data.I34_phase = phasor_sum_phase_control(ADC_Measurements.Ph3_Is, ADC_Measurements.Ph4_Is, logic_data.measurement[LOGIC_PH3_MEAS], logic_data.measurement[LOGIC_PH4_MEAS], TIM_PWM_Data.phase, TIM_PWM_Data.period);

			logic_data.Io_mag = phasor_sum_magnitude(logic_data.I12_mag, logic_data.I34_mag, logic_data.I12_phase, logic_data.I34_phase, TIM_PWM_Data.period);
			logic_data.Io_phase = phasor_sum_phase(logic_data.I12_mag, logic_data.I34_mag, logic_data.I12_phase, logic_data.I34_phase, TIM_PWM_Data.period);
		}

		//executed every 100ms
		if(cnt100 >= 100)
		{
			cnt100 = 0;
			TIM_Sys_Flags.Time100ms = 1;

		// PI Controller

			if (TIM_PWM_Data.state == TIMER_STATE_RUN3) {

				// Frequency changes to control Io phase (Vdc input = 7V)
				Io_ph_ref = 120 * ((float)TIM_PWM_Data.period / 360); // Degrees (110)

				error_freq_0 = Io_ph_ref - logic_data.Io_phase;
				f_sw_0 = TIM_Control_Var.ki_f * error_freq_0;
				f_sw_0 += TIM_Control_Var.kp_f * (error_freq_0 - error_freq_1);
				f_sw_0 += f_sw_1;

				timer_pwm_freq_control(f_sw_0);

				f_sw_1 = f_sw_0;
				error_freq_1 = error_freq_0;

				// V phase changes to control Io magnitude (10x faster than frequency control)
				Io_mag_ref = 7 * (100); // Amperes

				error_ph_0 = Io_mag_ref - logic_data.Io_mag;
				v_ph_0 = TIM_Control_Var.ki_ph * error_ph_0;
				v_ph_0 += TIM_Control_Var.kp_ph * (error_ph_0 - error_ph_1);
				v_ph_0 += v_ph_1;

				timer_pwm_phase_control(v_ph_0);

//				v_ph_0 *= 1000;
//				error_ph_0 *= 1000;
//				diag_store_buffer(v_ph_0, error_ph_0);
//				v_ph_0 /= 1000;
//				error_ph_0 /= 1000;

				v_ph_1 = v_ph_0;
				error_ph_1 = error_ph_0;
			}
			else if (TIM_PWM_Data.state == TIMER_STATE_RUN2)
			{
				f_sw_1 = FPGA_CLOCK / (TIM_PWM_Data.period + 2);
				v_ph_1 = TIM_PWM_Data.phase;
				error_freq_1 = 0;
				error_ph_1 = 0;
				f_sw_0 = 0;
				v_ph_0 = 0;
				error_freq_0 = 0;
				error_ph_0 = 0;
			}
		}

		//executed every 200ms
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

		//executed every 10000ms
		if(cnt10000 >= 10000)
		{
			cnt10000 = 0;
			TIM_Sys_Flags.Time10000ms = 1;
		}
	}
}
