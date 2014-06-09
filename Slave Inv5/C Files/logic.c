#include "logic.h"

void logic_init(void)
{
	SPI_InitTypeDef  SPI_InitStructure;

	logic_reset();

	SPI_I2S_DeInit(SPI3);
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;

	SPI_Init(SPI3, &SPI_InitStructure);

	SPI_Cmd(SPI3, ENABLE);

	GPIO_SetBits(LOGIC_CSN_PIN);

	logic_read_flags();
}

void logic_set_addr(u8 addr)
{
	u8 BitVal;

	BitVal = addr & 0x01;

	if (BitVal != Bit_RESET)
	{
		LOGIC_ADDR_PORT->BSRRL = LOGIC_A0_PIN;
	}
	else
	{
		LOGIC_ADDR_PORT->BSRRH = LOGIC_A0_PIN;
	}

	addr >>= 1;

	BitVal = addr & 0x01;

	if (BitVal != Bit_RESET)
	{
		LOGIC_ADDR_PORT->BSRRL = LOGIC_A1_PIN;
	}
	else
	{
		LOGIC_ADDR_PORT->BSRRH = LOGIC_A1_PIN;
	}

	addr >>= 1;

	BitVal = addr & 0x01;

	if (BitVal != Bit_RESET)
	{
		LOGIC_ADDR_PORT->BSRRL = LOGIC_A2_PIN;
	}
	else
	{
		LOGIC_ADDR_PORT->BSRRH = LOGIC_A2_PIN;
	}

	addr >>= 1;

	BitVal = addr & 0x01;

	if (BitVal != Bit_RESET)
	{
		LOGIC_ADDR_PORT->BSRRL = LOGIC_A3_PIN;
	}
	else
	{
		LOGIC_ADDR_PORT->BSRRH = LOGIC_A3_PIN;
	}

	addr >>= 1;

	BitVal = addr & 0x01;

	if (BitVal != Bit_RESET)
	{
		LOGIC_ADDR_PORT->BSRRL = LOGIC_A4_PIN;
	}
	else
	{
		LOGIC_ADDR_PORT->BSRRH = LOGIC_A4_PIN;
	}
}

void logic_set_period(void)
{
	u16 tmpH, tmpL;

	tmpH = 0x8000;

	if(logic_data.out_enable)
	{
		tmpH += 0x4000;
	}

	tmpL = logic_data.period;

	logic_set_addr(LOGIC_PERIOD_REG);

	GPIO_ResetBits(LOGIC_CSN_PIN);

	SPI_I2S_SendData(SPI3, tmpH);

	while(SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_BSY)) ;

	tmpH = SPI_I2S_ReceiveData(SPI3);

	SPI_I2S_SendData(SPI3, tmpL);

	while(SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_BSY)) ;

	tmpL = SPI_I2S_ReceiveData(SPI3);

	GPIO_SetBits(LOGIC_CSN_PIN);
}

void logic_set_ph1(void)
{
	u16 tmpH, tmpL;

	tmpH = logic_data.ph1_off;
	tmpL = logic_data.ph1_on;

	logic_set_addr(LOGIC_PH1_REG);

	GPIO_ResetBits(LOGIC_CSN_PIN);

	SPI_I2S_SendData(SPI3, tmpH);

	while(SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_BSY)) ;

	tmpH = SPI_I2S_ReceiveData(SPI3);

	SPI_I2S_SendData(SPI3, tmpL);

	while(SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_BSY)) ;

	tmpL = SPI_I2S_ReceiveData(SPI3);

	GPIO_SetBits(LOGIC_CSN_PIN);
}

void logic_set_ph2(void)
{
	u16 tmpH, tmpL;

	tmpH = logic_data.ph2_off;
	tmpL = logic_data.ph2_on;

	logic_set_addr(LOGIC_PH2_REG);

	GPIO_ResetBits(LOGIC_CSN_PIN);

	SPI_I2S_SendData(SPI3, tmpH);

	while(SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_BSY)) ;

	tmpH = SPI_I2S_ReceiveData(SPI3);

	SPI_I2S_SendData(SPI3, tmpL);

	while(SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_BSY)) ;

	tmpL = SPI_I2S_ReceiveData(SPI3);

	GPIO_SetBits(LOGIC_CSN_PIN);
}


void logic_set_out(void)
{
	logic_set_ph1();
	logic_set_ph2();
	logic_set_period();
}

void logic_calc_out(u16 period, u16 phase)
{
	u16 tmp = period / 2;

	if(phase >= period)
		phase = period - 1;

	logic_data.period = period;
	logic_data.ph1_on = tmp;
	logic_data.ph2_on = tmp + phase;
	logic_data.ph1_off = logic_data.ph1_on + logic_data.period / 2;
	logic_data.ph2_off = logic_data.ph2_on + logic_data.period / 2;

	if(logic_data.ph1_off >= logic_data.period)
		logic_data.ph1_off -= logic_data.period;

	if(logic_data.ph2_off >= logic_data.period)
		logic_data.ph2_off -= logic_data.period;

}

void logic_read_flags(void)
{
	u16 tmpH, tmpL;

	logic_set_addr(LOGIC_FALG_REG);

	GPIO_ResetBits(LOGIC_CSN_PIN);

	SPI_I2S_SendData(SPI3, 0);

	while(SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_BSY)) ;

	tmpH = SPI_I2S_ReceiveData(SPI3);

	SPI_I2S_SendData(SPI3, 0);

	while(SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_BSY)) ;

	tmpL = SPI_I2S_ReceiveData(SPI3);

	logic_data.all_faults = tmpH;
	logic_data.all_faults <<= 16;
	logic_data.all_faults += tmpL;

	GPIO_SetBits(LOGIC_CSN_PIN);
}

void logic_read_measurement(u8 addr)
{
	u16 tmpH, tmpL;

	addr &= 0x0F;

	logic_set_addr(0x10 + addr);

	GPIO_ResetBits(LOGIC_CSN_PIN);

	SPI_I2S_SendData(SPI3, 0);

	while(SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_BSY)) ;

	tmpH = SPI_I2S_ReceiveData(SPI3);

	SPI_I2S_SendData(SPI3, 0);

	while(SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_BSY)) ;

	tmpL = SPI_I2S_ReceiveData(SPI3);

	logic_data.measurement[addr] = tmpL & 0x0FFF;

	GPIO_SetBits(LOGIC_CSN_PIN);
}

void logic_reset(void)
{
	GPIO_SetBits(LOGIC_RST_PIN);
	GPIO_ResetBits(LOGIC_RST_PIN);
}

u8 logic_check_fault(void)
{
	logic_data.fault = GPIO_ReadInputDataBit(LOGIC_FLT_PIN);
	return logic_data.fault;
}

void logic_routine(void)
{
	static u8 state = 0;

	switch(state)
	{
		case 0:
			logic_read_measurement(LOGIC_PERIOD_MEAS);
			state++;
			break;
		case 1:
			logic_read_measurement(LOGIC_PH1_MEAS);
			state++;
			break;
		case 2:
			logic_read_measurement(LOGIC_PH2_MEAS);
			state++;
			break;
		case 3:
			logic_read_measurement(LOGIC_PH3_MEAS);
			state++;
			break;
		case 4:
			logic_read_measurement(LOGIC_PH4_MEAS);
			state++;
			break;
		default:
			state = 0;
			break;
	}
}
