#ifndef __INT_COMM_H__
#define __INT_COMM_H__

#include "usart.h"
#include "diagnostic.h"

#define ICOMM_ADPCNT	11

#define ICOMM_M_MTS		0
#define ICOMM_M_STM		1

#define ICOMM_PCKSTART 	255
#define ICOMM_PCKEND 	254

#define ICOMM_C_MTS		'm'
#define ICOMM_C_STM		's'
#define ICOMM_C_RST		'r'
#define ICOMM_C_OFF		'D'
#define ICOMM_C_ON		'E'
#define ICOMM_C_RUN		'R'

#define ICOMM_C_INC_FREQ  		'+'
#define ICOMM_C_DEC_FREQ  		'-'
#define ICOMM_C_INC10_FREQ 		'i'
#define ICOMM_C_DEC10_FREQ  	'd'
#define ICOMM_C_INC_PHASE   	'.'
#define ICOMM_C_DEC_PHASE   	','
#define ICOMM_C_INC10_PHASE 	'>'
#define ICOMM_C_DEC10_PHASE 	'<'

typedef struct {
	u8	mode;
	u8	state;
	u8	adi;
	u8 	cmd;
} icomm_data_type;

extern volatile icomm_data_type icomm_data;

void icomm_init(void);
void icomm_tx_routine(void);
void icomm_rx_routine(void);

#endif
