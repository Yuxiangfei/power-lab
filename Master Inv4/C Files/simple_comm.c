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
					if(icomm_data.mode == ICOMM_M_MTS)
						timer_pwm_freq_inc(1);
					else
						icomm_data.cmd = ICOMM_C_INC_FREQ;
					break;
				case SIMPLE_COMM_CMD_DEC_FREQ:
					if(icomm_data.mode == ICOMM_M_MTS)
						timer_pwm_freq_dec(1);
					else
						icomm_data.cmd = ICOMM_C_DEC_FREQ;
					break;
				case SIMPLE_COMM_CMD_INC10_FREQ:
					if(icomm_data.mode == ICOMM_M_MTS)
						timer_pwm_freq_inc(10);
					else
						icomm_data.cmd = ICOMM_C_INC10_FREQ;
					break;
				case SIMPLE_COMM_CMD_DEC10_FREQ:
					if(icomm_data.mode == ICOMM_M_MTS)
						timer_pwm_freq_dec(10);
					else
						icomm_data.cmd = ICOMM_C_DEC10_FREQ;
					break;
				case SIMPLE_COMM_CMD_INC_PHASE:
					if(icomm_data.mode == ICOMM_M_MTS)
						timer_pwm_phase_inc(1);
					else
						icomm_data.cmd = ICOMM_C_INC_PHASE;
					break;
				case SIMPLE_COMM_CMD_DEC_PHASE:
					if(icomm_data.mode == ICOMM_M_MTS)
						timer_pwm_phase_dec(1);
					else
						icomm_data.cmd = ICOMM_C_DEC_PHASE;
					break;
				case SIMPLE_COMM_CMD_INC10_PHASE:
					if(icomm_data.mode == ICOMM_M_MTS)
						timer_pwm_phase_inc(10);
					else
						icomm_data.cmd = ICOMM_C_INC10_PHASE;
					break;
				case SIMPLE_COMM_CMD_DEC10_PHASE:
					if(icomm_data.mode == ICOMM_M_MTS)
						timer_pwm_phase_dec(10);
					else
						icomm_data.cmd = ICOMM_C_DEC10_PHASE;
					break;
				case SIMPLE_COMM_CMD_GET_ALL:
					simple_comm_send_all();
					break;
				case SIMPLE_COMM_CMD_GET_SMALL:
					//simple_comm_send_small();
					break;
				case SIMPLE_COMM_CMD_ENABLE:
					if(icomm_data.mode == ICOMM_M_STM)
						icomm_data.cmd = ICOMM_C_ON;
					else
						TIM_PWM_Data.cmd = TIMER_CMD_ENABLE;
					break;
				case SIMPLE_COMM_CMD_DISABLE:
					TIM_PWM_Data.cmd = TIMER_CMD_DISABLE;
					icomm_data.cmd = ICOMM_C_OFF;
					break;
				case SIMPLE_COMM_CMD_RUN:
					if(icomm_data.mode == ICOMM_M_STM)
						icomm_data.cmd = ICOMM_C_RUN;
					else
						TIM_PWM_Data.cmd = TIMER_CMD_RUN;
					break;
				case SIMPLE_COMM_CMD_RESET:
					TIM_PWM_Data.cmd = TIMER_CMD_RESET;
					icomm_data.cmd = ICOMM_C_RST;
					break;
				case SIMPLE_COMM_CMD_MTS:
					if(Diagnostic_Data.State == 0 && Diagnostic_Data.State_s == 0)
					{
						icomm_data.mode = ICOMM_M_MTS;
						icomm_data.cmd = ICOMM_C_MTS;
					}
					break;
				case SIMPLE_COMM_CMD_STM:
					if(Diagnostic_Data.State == 0 && Diagnostic_Data.State_s == 0)
					{
						icomm_data.mode = ICOMM_M_STM;
						icomm_data.cmd = ICOMM_C_STM;
					}
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
	Diagnostic_Data.State += ((u32)icomm_data.mode) << 31;

	ptr = (u8*)&Diagnostic_Data;

	for(i = 0; i < 400; i++)
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
	Diagnostic_Data.State &= 0xFF;
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
