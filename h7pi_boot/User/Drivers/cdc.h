#ifndef _CDC_H_
#define _CDC_H_

#include "stm32h7xx_hal.h"
#include "usbd_cdc_if.h"



#define CDC_BUFFER_SIZE (256)
#define CDC_CMD_LENGTH   16



typedef struct
{
	uint16_t rxLength;
	uint8_t  rxEnd;
	uint8_t  rxBuffer[CDC_BUFFER_SIZE];
}cdc_t;


extern cdc_t cdc;

void cdc_send(uint8_t* Buf, uint16_t Len);
void cdc_printf(const char *format, ...);
void cdc_clear_buffer(void);
#endif

