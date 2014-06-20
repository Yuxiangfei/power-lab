#include "config.h"
#include "gpio.h"
#include "timer.h"
#include "usart.h"
#include "adc.h"
#include "simple_comm.h"
#include "logic.h"
#include "diagnostic.h"
#include "int_comm.h"

volatile USART2_Datas_Type USART2_Datas;
volatile USART1_Datas_Type USART1_Datas;

volatile TIM_Sys_Flags_Type TIM_Sys_Flags;

volatile TIM_PWM_Data_Type TIM_PWM_Data;

volatile ADC_Datas_Type ADC_Datas;
volatile ADC_Measurements_Type ADC_Measurements;

volatile logic_data_type logic_data;

volatile Diagnostic_Data_Type Diagnostic_Data;

volatile icomm_data_type icomm_data;

volatile TIM_Control_Var_Type TIM_Control_Var;

int main(void)
{
	RCC_Configuration();
	NVIC_Configuration();

	//IWDT_Init();
	gpio_init();
	TimerSys_init();
	adc1_init();
	timer_pwm_init();
	logic_init();
	usart1_init();
	usart2_init();
	icomm_init();

	while(1)
    {
		//IWDG_ReloadCounter();

		simple_comm_routine();
		icomm_rx_routine();

		if(TIM_Sys_Flags.Time1000ms)
		{
			TIM_Sys_Flags.Time1000ms = 0;
		}

		if(TIM_Sys_Flags.Time100ms)
		{
			TIM_Sys_Flags.Time100ms = 0;
		}

		if(TIM_Sys_Flags.Time10ms)
		{
			TIM_Sys_Flags.Time10ms = 0;

			icomm_tx_routine();
			timer_routine();
		}
    }
}
