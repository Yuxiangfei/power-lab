#include "simple_comm.h"

void simple_comm_routine(void)
{
	static u8 mode = 0;
	static u32 value = 0;
	u32 tmp;
	u8 ch;

	if(usart2_check_RX())
	{
		ch = usart2_get_byte();

		if(mode == 0)
		{
			switch(ch)
			{
				case SIMPLE_COMM_CMD_INC_FREQ:
					timer_pwm_freq_inc(1);
					break;
				case SIMPLE_COMM_CMD_DEC_FREQ:
					timer_pwm_freq_dec(1);
					break;
				case SIMPLE_COMM_CMD_INC10_FREQ:
					timer_pwm_freq_inc(10);
					break;
				case SIMPLE_COMM_CMD_DEC10_FREQ:
					timer_pwm_freq_dec(10);
					break;
				case SIMPLE_COMM_CMD_INC_PHASE:
					timer_pwm_phase_inc(1);
					break;
				case SIMPLE_COMM_CMD_DEC_PHASE:
					timer_pwm_phase_dec(1);
					break;
				case SIMPLE_COMM_CMD_INC10_PHASE:
					timer_pwm_phase_inc(10);
					break;
				case SIMPLE_COMM_CMD_DEC10_PHASE:
					timer_pwm_phase_dec(10);
					break;
				case SIMPLE_COMM_CMD_GET_ALL:
					simple_comm_send_all();
					break;
				case SIMPLE_COMM_CMD_GET_SMALL:
					//simple_comm_send_small();
					break;
				case SIMPLE_COMM_CMD_ENABLE:
					TIM_PWM_Data.cmd = TIMER_CMD_ENABLE;
					break;
				case SIMPLE_COMM_CMD_DISABLE:
					TIM_PWM_Data.cmd = TIMER_CMD_DISABLE;
					break;
				case SIMPLE_COMM_CMD_RUN:
					TIM_PWM_Data.cmd = TIMER_CMD_RUN;
					break;
				case SIMPLE_COMM_CMD_RESET:
					TIM_PWM_Data.cmd = TIMER_CMD_RESET;
					break;
				default:
					TIM_PWM_Data.cmd = ch;
					break;
			}
		}
		else
		{
			if(ch >= '0' && ch <= '9')
			{
				value *= 10;
				value += ch - '0';
			}
			else if(ch == SIMPLE_COMM_CMD_EXECUTE)
			{
				switch(mode)
				{
					case SIMPLE_COMM_CMD_SET_REF3:
						//TIM_Ctrl_Ph[2].ref_sp = value;
						break;
					default:
						break;
				}

				mode = 0;
			}
		}
	}
}

void simple_comm_send_all(void)
{
	u32 crc = 0;
	u32 i;
	u8 *ptr;

	diag_get_data();

	ptr = (u8*)&Diagnostic_Data;

	for(i = 0; i < 328; i++)
	{
		USART2_Datas.TX_Buf[USART2_Datas.TX_index_in] = *ptr;
		crc += *ptr;
		ptr++;
		USART2_Datas.TX_data_cnt++;
		USART2_Datas.TX_index_in++;

		if(USART2_Datas.TX_index_in >= TX_BUF_SIZE)
		{
			USART2_Datas.TX_index_in = 0;
		}
	}

	diag_reset_data();

	ptr = (u8*)&crc;

	for(i = 0; i < 4; i++)
	{
		USART2_Datas.TX_Buf[USART2_Datas.TX_index_in] = *ptr;
		ptr++;
		USART2_Datas.TX_data_cnt++;
		USART2_Datas.TX_index_in++;

		if(USART2_Datas.TX_index_in >= TX_BUF_SIZE)
		{
			USART2_Datas.TX_index_in = 0;
		}
	}

	usatr2_tx_enable();
	USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
}

void simple_comm_send_buffer(void)
{
	u32 crc = 0;
	u32 i;
	u8 *ptr;

	ptr = (u8*)&Diagnostic_Data.buffer1[0];

	for(i = 0; i < 8 * DIAG_BUF_SIZE; i++)
	{
		USART2_Datas.TX_Buf[USART2_Datas.TX_index_in] = *ptr;
		crc += *ptr;
		ptr++;
		USART2_Datas.TX_data_cnt++;
		USART2_Datas.TX_index_in++;

		if(USART2_Datas.TX_index_in >= TX_BUF_SIZE)
		{
			USART2_Datas.TX_index_in = 0;
		}
	}

	ptr = (u8*)&crc;

	for(i = 0; i < 4; i++)
	{
		USART2_Datas.TX_Buf[USART2_Datas.TX_index_in] = *ptr;
		ptr++;
		USART2_Datas.TX_data_cnt++;
		USART2_Datas.TX_index_in++;

		if(USART2_Datas.TX_index_in >= TX_BUF_SIZE)
		{
			USART2_Datas.TX_index_in = 0;
		}
	}

	usatr2_tx_enable();
	USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
}
