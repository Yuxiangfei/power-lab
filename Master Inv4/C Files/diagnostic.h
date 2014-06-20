#ifndef __DIAGNOSTIC_H__
#define __DIAGNOSTIC_H__

#include "adc.h"
#include "timer.h"

#define	DIAG_MAX			0
#define	DIAG_MEAN			1
#define	DIAG_MIN			2

#define	DIAG_BUF_SIZE		1000

#define	DIAG_BUF_CNT_LIM	40000

typedef struct
{
	u32	Ph1_Is[3];
	u32	Ph2_Is[3];
	u32	Ph3_Is[3];
	u32	Ph4_Is[3];
//48
	u32	IPhase1[3];
	u32	IPhase2[3];
	u32	IPhase3[3];
	u32	IPhase4[3];
//96
	u32	Vdc1[3];
	u32	Vdc2[3];
//120
	u32	Period[3];
	u32	VPhase[3];
//144
	s32	Temp1;
	s32	Temp2;
//152
	u32	Faults;
	u32	State;
	u32	CntVal;
//164
	u32	Ph1_Is_s[3];
	u32	Ph2_Is_s[3];
	u32	Ph3_Is_s[3];
	u32	Ph4_Is_s[3];
//212
	u32	IPhase1_s[3];
	u32	IPhase2_s[3];
	u32	IPhase3_s[3];
	u32	IPhase4_s[3];
//260
	u32	Vdc1_s[3];
	u32	Vdc2_s[3];
//284
	u32	Period_s[3];
	u32	VPhase_s[3];
//308
	s32	Temp1_s;
	s32	Temp2_s;
//316
	u32	Faults_s;
	u32	State_s;
	u32	CntVal_s;
//328
	s32 I12_mag[3];
	s32 I12_phase[3];
	s32 I34_mag[3];
	s32 I34_phase[3];
	s32 Io_mag[3];
	s32 Io_phase[3];
//400
	u32 kp_f;
	u32 ki_f;
	u32 kp_ph;
	u32 ki_ph;
	u32 precision;
//420
	s32	buffer1[DIAG_BUF_SIZE];
	s32	buffer2[DIAG_BUF_SIZE];
	u32	bcnt;
	u8	benable;

	u32	cnt;
	u32	cnt_check;
	u8	enable;
} Diagnostic_Data_Type;

volatile extern Diagnostic_Data_Type Diagnostic_Data;

void diag_store_data(void);
void diag_reset_data(void);
void diag_get_data(void);

void diag_store_buffer(s32 A, s32 B);
void diag_reset_buffer(void);

volatile extern u8 temp1;

#endif
