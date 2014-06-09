#ifndef __LOGIC_H__
#define __LOGIC_H__

#include "stm32f4xx_gpio.h"
#include "stm32f4xx_spi.h"

#define LOGIC_CSN_PIN		GPIOD, GPIO_Pin_0
#define LOGIC_ADDR_PORT		GPIOD
#define LOGIC_A0_PIN		GPIO_Pin_1
#define LOGIC_A1_PIN		GPIO_Pin_2
#define LOGIC_A2_PIN		GPIO_Pin_3
#define LOGIC_A3_PIN		GPIO_Pin_4
#define LOGIC_A4_PIN		GPIO_Pin_5

#define LOGIC_FLT_PIN		GPIOD, GPIO_Pin_7
#define LOGIC_RST_PIN		GPIOD, GPIO_Pin_6

#define LOGIC_FALG_REG		0
#define LOGIC_PERIOD_REG	1
#define LOGIC_PH1_REG		2
#define LOGIC_PH2_REG		3
#define LOGIC_PH3_REG		4
#define LOGIC_PH4_REG		5

#define LOGIC_PERIOD_MEAS	0
#define LOGIC_PH1_MEAS		1
#define LOGIC_PH2_MEAS		2
#define LOGIC_PH3_MEAS		3
#define LOGIC_PH4_MEAS		4

typedef struct
{
	u8  fault;
	u32 all_faults;
	u16 measurement[16];
	u16 period;
	u16 ph1_on;
	u16 ph2_on;
	u16 ph1_off;
	u16 ph2_off;
	u8  out_enable;
} logic_data_type;

extern volatile logic_data_type logic_data;

void logic_init(void);
void logic_set_addr(u8 addr);
void logic_set_period(void);
void logic_set_ph1(void);
void logic_set_ph2(void);
void logic_set_out(void);
void logic_calc_out(u16 period, u16 phase);
void logic_read_flags(void);
void logic_read_phase(void);
void logic_reset(void);
u8 logic_check_fault(void);

#endif
