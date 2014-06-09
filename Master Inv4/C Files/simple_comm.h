#ifndef __SIMPLE_COMM_H__
#define __SIMPLE_COMM_H__

#include "usart.h"
#include "timer.h"
#include "diagnostic.h"
#include "logic.h"
#include "int_comm.h"

#define SIMPLE_COMM_CMD_GET_ALL  		'A'
#define SIMPLE_COMM_CMD_GET_SMALL  		'a'
#define SIMPLE_COMM_CMD_GET_BUFFER 		'B'
#define SIMPLE_COMM_CMD_TRG_BUFFER 		'I'
#define SIMPLE_COMM_CMD_SET_REF1  		'1'
#define SIMPLE_COMM_CMD_SET_REF2  		'2'
#define SIMPLE_COMM_CMD_SET_REF3  		'3'
#define SIMPLE_COMM_CMD_EXECUTE  		'X'

#define SIMPLE_COMM_CMD_INC_FREQ  		'+'
#define SIMPLE_COMM_CMD_DEC_FREQ  		'-'
#define SIMPLE_COMM_CMD_INC10_FREQ 		'i'
#define SIMPLE_COMM_CMD_DEC10_FREQ  	'd'
#define SIMPLE_COMM_CMD_INC_PHASE   	'.'
#define SIMPLE_COMM_CMD_DEC_PHASE   	','
#define SIMPLE_COMM_CMD_INC10_PHASE 	'>'
#define SIMPLE_COMM_CMD_DEC10_PHASE 	'<'

#define SIMPLE_COMM_CMD_ENABLE  		'E'
#define SIMPLE_COMM_CMD_DISABLE  		'D'
#define SIMPLE_COMM_CMD_RUN  			'R'
#define SIMPLE_COMM_CMD_RESET  			'r'

#define SIMPLE_COMM_CMD_MTS				']'
#define SIMPLE_COMM_CMD_STM				'['

void simple_comm_routine(void);

void simple_comm_send_all(void);
void simple_comm_send_buffer(void);

#endif
