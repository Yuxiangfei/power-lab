#ifndef __USART_H__
#define __USART_H__

#include "stm32f4xx_usart.h"
#include "stm32f4xx_gpio.h"
#include "stdio.h"
#include "config.h"

#define USART1_BAUD				115200
#define USART2_BAUD				115200
#define TX_BUF_SIZE				1024
#define RX_BUF_SIZE				1024

#define USART_TXEN2_PIN			GPIOA, GPIO_Pin_1

typedef struct
{
	u16	TX_index_in;
	u16	TX_index_out;
	u16	TX_data_cnt;
	u16	RX_index_in;
	u16	RX_index_out;
	u16	RX_data_cnt;
	u8	TX_Buf[TX_BUF_SIZE];
	u8	RX_Buf[RX_BUF_SIZE];
}USART2_Datas_Type;

typedef struct
{
	u16	TX_index_in;
	u16	TX_index_out;
	u16	TX_data_cnt;
	u16	RX_index_in;
	u16	RX_index_out;
	u16	RX_data_cnt;
	u8	TX_Buf[TX_BUF_SIZE];
	u8	RX_Buf[RX_BUF_SIZE];
}USART1_Datas_Type;

volatile extern USART2_Datas_Type USART2_Datas;
volatile extern USART1_Datas_Type USART1_Datas;

//initiate USART1 for 8b,1,n and no flow control
void usart1_init(void);

//initiate USART2 for 8b,1,n and no flow control
void usart2_init(void);

void usatr2_tx_enable(void);
void usatr2_tx_disable(void);

//check for data in usart1 RX buf
u8 usart1_check_RX(void);

//check for data in usart2 RX buf
u8 usart2_check_RX(void);

//get byte from usart1 RX buf
u8 usart1_get_byte(void);

//get byte from usart2 RX buf
u8 usart2_get_byte(void);

//put byte into usart1 RX buf
void usart1_put_byte(u8 b);

void USART1_IRQHandler(void);
void USART2_IRQHandler(void);

#endif
