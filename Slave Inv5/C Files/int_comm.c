#include "int_comm.h"

void icomm_init(void)
{
	icomm_data.mode = ICOMM_M_MTS;
	icomm_data.adi = 0;
	icomm_data.state = 0;
}

void icomm_tx_routine(void)
{
	static u8 buf[164];
	u8* ptr;
	u32 tmp32;
	u16 tmp16;
	u8 crc;
	u8 i;

	if(icomm_data.mode == ICOMM_M_MTS)
	{
		TIM_PWM_Data.state = 0;

		tmp32 = ADC_Measurements.Ph1_Is;
		tmp32 += ADC_Measurements.Ph2_Is;
		tmp32 += ADC_Measurements.Ph3_Is;
		tmp32 += ADC_Measurements.Ph4_Is;
		tmp32 >>= 2;
		tmp16 = (u16)tmp32;
		crc = 0;

		usart1_put_byte(ICOMM_PCKSTART);
		usart1_put_byte(ICOMM_PCKSTART);

		ptr = (u8*)&tmp16;
		crc += ptr[0];
		crc += ptr[1];
		usart1_put_byte(ptr[0]);
		usart1_put_byte(ptr[1]);

		tmp16 = (u16)ADC_Measurements.Vdc1;

		ptr = (u8*)&tmp16;
		crc += ptr[0];
		crc += ptr[1];
		usart1_put_byte(ptr[0]);
		usart1_put_byte(ptr[1]);

		usart1_put_byte(icomm_data.adi);

		ptr = (u8*)&buf;
		ptr += 16 * icomm_data.adi;

		if(icomm_data.adi == ICOMM_ADPCNT - 1)
			tmp16 = 4;
		else
			tmp16 = 16;

		for(i = 0; i < tmp16; i++)
		{
			USART1_Datas.TX_Buf[USART1_Datas.TX_index_in] = *ptr;
			crc += *ptr;
			ptr++;
			USART1_Datas.TX_data_cnt++;
			USART1_Datas.TX_index_in++;

			if(USART1_Datas.TX_index_in >= TX_BUF_SIZE)
			{
				USART1_Datas.TX_index_in = 0;
			}
		}

		icomm_data.adi++;

		usart1_put_byte(crc);
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);

		if(icomm_data.adi >= ICOMM_ADPCNT)
		{
			icomm_data.adi = 0;

			diag_get_data();
			ptr = (u8*)&Diagnostic_Data;
			for(i = 0; i < 164; i++)
				buf[i] = ptr[i];

			diag_reset_data();
		}
	}
	else if(icomm_data.mode == ICOMM_M_STM)
	{
		crc = 0;

		usart1_put_byte(ICOMM_PCKSTART);
		usart1_put_byte(ICOMM_PCKSTART);

		usart1_put_byte(icomm_data.adi);

		ptr = (u8*)&buf;
		ptr += 16 * icomm_data.adi;

		if(icomm_data.adi == ICOMM_ADPCNT - 1)
			tmp16 = 4;
		else
			tmp16 = 16;

		for(i = 0; i < tmp16; i++)
		{
			USART1_Datas.TX_Buf[USART1_Datas.TX_index_in] = *ptr;
			crc += *ptr;
			ptr++;
			USART1_Datas.TX_data_cnt++;
			USART1_Datas.TX_index_in++;

			if(USART1_Datas.TX_index_in >= TX_BUF_SIZE)
			{
				USART1_Datas.TX_index_in = 0;
			}
		}

		icomm_data.adi++;

		if(icomm_data.adi >= ICOMM_ADPCNT)
		{
			icomm_data.adi = 0;

			diag_get_data();
			ptr = (u8*)&Diagnostic_Data;
			for(i = 0; i < 164; i++)
				buf[i] = ptr[i];

			diag_reset_data();
		}

		usart1_put_byte(crc);
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
	}
}

void icomm_rx_routine(void)
{
	static u8 tmp1 = 0;
	u8 tmp2;

	if(icomm_data.mode == ICOMM_M_MTS)
	{
		if(usart1_check_RX())
		{
			tmp2 = usart1_get_byte();

			if(tmp1 == tmp2)
			{
				switch(tmp1)
				{
					case ICOMM_C_STM:
						icomm_data.mode = ICOMM_M_STM;
						break;
					case ICOMM_C_RST:
						TIM_PWM_Data.cmd = TIMER_CMD_RESET;
						break;
					default:
						break;
				}
			}

			tmp1 = tmp2;
		}
	}
	else if(icomm_data.mode == ICOMM_M_STM)
	{
		if(usart1_check_RX())
		{
			tmp2 = usart1_get_byte();

			if(tmp1 == tmp2)
			{
				switch(tmp1)
				{
					case ICOMM_C_MTS:
						if(TIM_PWM_Data.state == 0)
							icomm_data.mode = ICOMM_M_MTS;
						break;
					case ICOMM_C_RST:
						TIM_PWM_Data.cmd = TIMER_CMD_RESET;
						break;
					case ICOMM_C_OFF:
						TIM_PWM_Data.cmd = TIMER_CMD_DISABLE;
						break;
					case ICOMM_C_ON:
						TIM_PWM_Data.cmd = TIMER_CMD_ENABLE;
						break;
					case ICOMM_C_RUN:
						TIM_PWM_Data.cmd = TIMER_CMD_RUN;
						break;
					case ICOMM_C_INC_FREQ:
						timer_pwm_freq_inc(1);
						break;
					case ICOMM_C_DEC_FREQ:
						timer_pwm_freq_dec(1);
						break;
					case ICOMM_C_INC10_FREQ:
						timer_pwm_freq_inc(10);
						break;
					case ICOMM_C_DEC10_FREQ:
						timer_pwm_freq_dec(10);
						break;
					case ICOMM_C_INC_PHASE:
						timer_pwm_phase_inc(1);
						break;
					case ICOMM_C_DEC_PHASE:
						timer_pwm_phase_dec(1);
						break;
					case ICOMM_C_INC10_PHASE:
						timer_pwm_phase_inc(10);
						break;
					case ICOMM_C_DEC10_PHASE:
						timer_pwm_phase_dec(10);
						break;
					default:
						break;
				}
			}

			tmp1 = tmp2;
		}
	}
}
