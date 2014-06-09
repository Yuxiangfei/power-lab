/**************************************************************************//*****
 * @file     stdio.c
 * @brief    Implementation of newlib syscall
 ********************************************************************************/

#include <stdio.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "usart.h"

#undef errno
extern int errno;
extern int  _end;

caddr_t _sbrk ( int incr )
{
  static unsigned char *heap = NULL;
  unsigned char *prev_heap;

  if (heap == NULL) {
    heap = (unsigned char *)&_end;
  }
  prev_heap = heap;

  heap += incr;

  return (caddr_t) prev_heap;
}

int link(char *old, char *new) {
return -1;
}

int _close(int file)
{
  return -1;
}

int _fstat(int file, struct stat *st)
{
  st->st_mode = S_IFCHR;
  return 0;
}

int _isatty(int file)
{
  return 1;
}

int _lseek(int file, int ptr, int dir)
{
  return 0;
}

int _read(int file, char *ptr, int len)
{
  return 0;
}

int _write(int file, char *ptr, int len)
{
	int txCount;
	(void)file;

	for (txCount = 0; txCount < len; txCount++)
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

	return len;
}

void abort(void)
{
  /* Abort called */
  while(1);
}
          
/* --------------------------------- End Of File ------------------------------ */
