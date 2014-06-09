#include "usart.h"

//initiate USART1 for 8b,1,n and no flow control
void usart1_init(void)
{
	USART_InitTypeDef USART_InitStructure;

    USART1_Datas.TX_index_in = 0;
	USART1_Datas.TX_index_out = 0;
	USART1_Datas.TX_data_cnt = 0;
    USART1_Datas.RX_index_in = 0;
	USART1_Datas.RX_index_out = 0;
	USART1_Datas.RX_data_cnt = 0;

	USART_InitStructure.USART_BaudRate = USART1_BAUD;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_2;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl =	USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

	USART_Init(USART1, &USART_InitStructure);

	/* Enable USART1 Receive interrupts */
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	USART_Cmd(USART1, ENABLE);
}

//initiate USART2 for 8b,1,n and no flow control
void usart2_init(void)
{
	USART_InitTypeDef USART_InitStructure;

    USART2_Datas.TX_index_in = 0;
	USART2_Datas.TX_index_out = 0;
	USART2_Datas.TX_data_cnt = 0;
    USART2_Datas.RX_index_in = 0;
	USART2_Datas.RX_index_out = 0;
	USART2_Datas.RX_data_cnt = 0;

	USART_InitStructure.USART_BaudRate = USART2_BAUD;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_2;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl =	USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

	USART_Init(USART2, &USART_InitStructure);

	/* Enable USART2 Receive interrupts */
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

	USART_Cmd(USART2, ENABLE);

	usatr2_tx_disable();
}

void usatr2_tx_enable(void)
{
	GPIO_SetBits(USART_TXEN2_PIN);
}

void usatr2_tx_disable(void)
{
	GPIO_ResetBits(USART_TXEN2_PIN);
}

//check for data in usart1 RX buf
u8 usart1_check_RX(void)
{
	if(USART1_Datas.RX_data_cnt)
	{
		return 1;
	}

	return 0;
}

//check for data in usart2 RX buf
u8 usart2_check_RX(void)
{
	if(USART2_Datas.RX_data_cnt)
	{
		return 1;
	}

	return 0;
}

//get byte from usart1 RX buf
u8 usart1_get_byte(void)
{
	u8 tmp;

	tmp = USART1_Datas.RX_Buf[USART1_Datas.RX_index_out++];

	USART1_Datas.RX_data_cnt--;

	if(USART1_Datas.RX_index_out >= RX_BUF_SIZE)
	{
		USART1_Datas.RX_index_out = 0;
	}

	return tmp;
}

//get byte from usart2 RX buf
u8 usart2_get_byte(void)
{
	u8 tmp;

	tmp = USART2_Datas.RX_Buf[USART2_Datas.RX_index_out++];

	USART2_Datas.RX_data_cnt--;

	if(USART2_Datas.RX_index_out >= RX_BUF_SIZE)
	{
		USART2_Datas.RX_index_out = 0;
	}

	return tmp;
}

//put byte into usart1 RX buf
void usart1_put_byte(u8 b)
{
	USART1_Datas.TX_Buf[USART1_Datas.TX_index_in] = b;
	USART1_Datas.TX_data_cnt++;
	USART1_Datas.TX_index_in++;

	if(USART1_Datas.TX_index_in >= TX_BUF_SIZE)
	{
		USART1_Datas.TX_index_in = 0;
	}
}

void USART1_IRQHandler(void)
{
	u16 tmp;

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		/* Read one byte from the receive data register */
		tmp = USART1->DR;
		USART1_Datas.RX_Buf[USART1_Datas.RX_index_in++] = (u8)tmp;

		if(USART1_Datas.RX_index_in >= RX_BUF_SIZE)
		{
			USART1_Datas.RX_index_in = 0;
		}

		USART1_Datas.RX_data_cnt++;
	}

	if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)
	{
		if(USART1_Datas.TX_data_cnt > 0)
		{
			USART_SendData(USART1, USART1_Datas.TX_Buf[USART1_Datas.TX_index_out]);
			USART1_Datas.TX_data_cnt--;
			USART1_Datas.TX_index_out++;
		}
		else
		{
			USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
		}

		if(USART1_Datas.TX_index_out >= TX_BUF_SIZE)
		{
			USART1_Datas.TX_index_out = 0;
		}
	}
}

void USART2_IRQHandler(void)
{
	u16 tmp;

	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		/* Read one byte from the receive data register */
		tmp = USART2->DR;
		USART2_Datas.RX_Buf[USART2_Datas.RX_index_in++] = (u8)tmp;

		if(USART2_Datas.RX_index_in >= RX_BUF_SIZE)
		{
			USART2_Datas.RX_index_in = 0;
		}

		USART2_Datas.RX_data_cnt++;
	}

	if(USART_GetITStatus(USART2, USART_IT_TXE) != RESET)
	{
		if(USART2_Datas.TX_data_cnt > 0)
		{
			USART_SendData(USART2, USART2_Datas.TX_Buf[USART2_Datas.TX_index_out]);
			USART2_Datas.TX_data_cnt--;
			USART2_Datas.TX_index_out++;
		}
		else
		{
			USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
			USART_ITConfig(USART2, USART_IT_TC, ENABLE);
		}

		if(USART2_Datas.TX_index_out >= TX_BUF_SIZE)
		{
			USART2_Datas.TX_index_out = 0;
		}
	}

	if(USART_GetITStatus(USART2, USART_IT_TC) != RESET)
	{
		USART_ITConfig(USART2, USART_IT_TC, DISABLE);
		usatr2_tx_disable();
	}
}
