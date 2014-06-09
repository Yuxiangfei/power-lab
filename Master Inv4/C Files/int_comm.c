#include "int_comm.h"

void icomm_init(void)
{
	icomm_data.mode = ICOMM_M_MTS;
	icomm_data.adi = 0;
	icomm_data.state = 0;
	icomm_data.cmd = 0;
}

void icomm_tx_routine(void)
{
	u8* ptr;
	u32 tmp32;
	u16 tmp16;
	u8 crc;
	u8 i;

	if(icomm_data.mode == ICOMM_M_MTS)
	{
		if(icomm_data.cmd != 0)
		{
			usart1_put_byte(icomm_data.cmd);
			usart1_put_byte(icomm_data.cmd);
			USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
			icomm_data.cmd = 0;
		}
	}
	else if(icomm_data.mode == ICOMM_M_STM)
	{
		if(icomm_data.cmd != 0)
		{
			usart1_put_byte(icomm_data.cmd);
			usart1_put_byte(icomm_data.cmd);
			USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
			icomm_data.cmd = 0;
		}
		/*TIM_PWM_Data.state = 0;

		tmp32 = ADC_Measurements.Ph1_Is;
		tmp32 += ADC_Measurements.Ph2_Is;
		tmp32 += ADC_Measurements.Ph3_Is;
		tmp32 += ADC_Measurements.Ph4_Is;
		tmp32 >>= 2;
		tmp16 = (u16)tmp32;
		crc = 0;

		usart1_put_byte(255);
		usart1_put_byte(255);

		ptr = (u8*)&tmp16;
		crc += ptr[0];
		crc += ptr[1];
		usart1_put_byte(ptr[0]);
		usart1_put_byte(ptr[1]);

		tmp16 = ADC_Measurements.Vdc1;

		ptr = (u8*)&tmp16;
		crc += ptr[0];
		crc += ptr[1];
		usart1_put_byte(ptr[0]);
		usart1_put_byte(ptr[1]);

		if(icomm_data.adi >= ICOMM_ADPCNT)
			icomm_data.adi = ICOMM_PCKEND;

		crc += icomm_data.adi;
		usart1_put_byte(icomm_data.adi);

		if(icomm_data.adi != ICOMM_PCKEND)
		{
			ptr = (u8*)&Diagnostic_Data;
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
		}
		else
		{
			icomm_data.adi = 0;
		}

		usart1_put_byte(crc);*/
	}
}

void icomm_rx_routine(void)
{
	static u8 tmp1 = 0;
	static u8 cnt = 0;
	static u8 buf[20];
	static u8 crc = 0;
	u8 tmp2;
	u8 i;
	u8 *ptr;

	if(icomm_data.mode == ICOMM_M_MTS)
	{
		if(usart1_check_RX())
		{
			tmp2 = usart1_get_byte();

			switch(icomm_data.state)
			{
				case 0:
					if(tmp2 == ICOMM_PCKSTART)
						icomm_data.state = 1;
					break;
				case 1:
					if(tmp2 == ICOMM_PCKSTART)
					{
						icomm_data.state = 2;
						cnt = 0;
					}
					else
						icomm_data.state = 0;
					break;
				case 2:
					buf[cnt++] = tmp2;

					if(cnt >= 4)
						icomm_data.state = 3;
					break;
				case 3:
					icomm_data.adi = tmp2;
					if(tmp2 < ICOMM_ADPCNT - 1)
						icomm_data.state = 4;
					else if(tmp2 == ICOMM_ADPCNT - 1)
						icomm_data.state = 5;
					else
						icomm_data.state = 0;
					break;
				case 4:
					buf[cnt++] = tmp2;

					if(cnt >= 20)
						icomm_data.state = 6;
					break;
				case 5:
					buf[cnt++] = tmp2;

					if(cnt >= 8)
						icomm_data.state = 6;
					break;
				case 6:
					crc = 0;

					for(i = 0; i < cnt; i++)
						crc += buf[i];

					if(tmp2 == crc && Diagnostic_Data.enable)
					{
						ptr = (u8*)&TIM_PWM_Data.ifb;
						ptr[0] = buf[0];
						ptr[1] = buf[1];
						ptr = (u8*)&TIM_PWM_Data.vfb;
						ptr[0] = buf[2];
						ptr[1] = buf[3];

						ptr = (u8*)&Diagnostic_Data;
						ptr += icomm_data.adi * 16;
						ptr += 164;
						for(i = 4; i < cnt; i++)
							ptr[i - 4] = buf[i];
					}

					icomm_data.state = 0;
					break;
				default:
					icomm_data.state = 0;
					break;
			}
		}
	}
	else if(icomm_data.mode == ICOMM_M_STM)
	{
		if(usart1_check_RX())
		{
			tmp2 = usart1_get_byte();

			switch(icomm_data.state)
			{
				case 0:
					if(tmp2 == ICOMM_PCKSTART)
						icomm_data.state = 1;
					break;
				case 1:
					if(tmp2 == ICOMM_PCKSTART)
					{
						icomm_data.state = 3;
						cnt = 0;
					}
					else
						icomm_data.state = 0;
					break;
				case 3:
					icomm_data.adi = tmp2;
					if(tmp2 < ICOMM_ADPCNT - 1)
						icomm_data.state = 4;
					else if(tmp2 == ICOMM_ADPCNT - 1)
						icomm_data.state = 5;
					else
						icomm_data.state = 0;
					break;
				case 4:
					buf[cnt++] = tmp2;

					if(cnt >= 16)
						icomm_data.state = 6;
					break;
				case 5:
					buf[cnt++] = tmp2;

					if(cnt >= 4)
						icomm_data.state = 6;
					break;
				case 6:
					crc = 0;

					for(i = 0; i < cnt; i++)
						crc += buf[i];

					if(tmp2 == crc && Diagnostic_Data.enable)
					{
						ptr = (u8*)&Diagnostic_Data;
						ptr += icomm_data.adi * 16;
						ptr += 164;
						for(i = 0; i < cnt; i++)
							ptr[i] = buf[i];
					}

					icomm_data.state = 0;
					break;
				default:
					icomm_data.state = 0;
					break;
			}
		}
	}
}
