#ifndef __TIMER_H__
#define __TIMER_H__

#include "stm32f4xx_tim.h"
#include "stm32f4xx_gpio.h"
#include "adc.h"
#include "logic.h"
#include "diagnostic.h"

#define TIMER_PWM_PMAX			2275 // resonant frequency 1375
#define TIMER_PWM_PMIN			800

#define TIMER_PWM_INIT_FREQ		220000 //240000
#define FPGA_CLOCK				199500000 //301000000
//#define TIMER_PWM_INIT_PERIOD	((SystemCoreClock / TIMER_PWM_INIT_FREQ) - 1)
#define TIMER_PWM_INIT_PERIOD	((FPGA_CLOCK / TIMER_PWM_INIT_FREQ) - 2)

#define TIMER_STATE_OFF			0
#define TIMER_STATE_SS			1
#define TIMER_STATE_RUN1		2
#define TIMER_STATE_RUN2		3
#define TIMER_STATE_RUN3		4
#define TIMER_STATE_RUN4		5

#define TIMER_STATE_EXT_FLT		10
#define TIMER_STATE_IPH1_FLT	11
#define TIMER_STATE_IPH2_FLT	12
#define TIMER_STATE_IPH3_FLT	13
#define TIMER_STATE_TEMP_FLT	14

#define TIMER_IPH_LIMIT			2500
#define TIMER_TEMP_LIMIT		700

#define TIMER_CMD_ENABLE		'e'
#define TIMER_CMD_RUN			'E'
#define TIMER_CMD_DISABLE		'd'
#define TIMER_CMD_RESET			'r'

typedef struct
{
	u8 Time1ms;
	u8 Time10ms;
	u8 Time100ms;
	u8 TimeDisp200ms;
	u8 Time1000ms;
} TIM_Sys_Flags_Type;

typedef struct
{
	u8  state;
	u8  cmd;
	u32 cnt;
	u32 period;
	u32 phase;
	u16	vfb;
	u16	ifb;
} TIM_PWM_Data_Type;

extern volatile	TIM_Sys_Flags_Type TIM_Sys_Flags;
extern volatile TIM_PWM_Data_Type TIM_PWM_Data;

void timer_pwm_init(void);
void timer_pwm_freq_inc(u16 value);
void timer_pwm_freq_dec(u16 value);
void timer_pwm_phase1_inc(u16 value);
void timer_pwm_phase1_dec(u16 value);
void timer_pwm_phase2_inc(u16 value);
void timer_pwm_phase2_dec(u16 value);

void timer_routine(void);
void TIM1_UP_TIM10_IRQHandler(void);
void TIM8_UP_TIM13_IRQHandler(void);
void DMA2_Stream0_IRQHandler(void);

void TimerSys_init(void);
void SysTick_Handler(void);

#endif
