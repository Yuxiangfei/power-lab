#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_iwdg.h"
#include "misc.h"

void RCC_Configuration(void);
void NVIC_Configuration(void);
void IWDT_Init(void);

#endif
