#include "led.h"

/********************************************************
 *
 *LED …Ë÷√◊¥Ã¨
 *
 ********************************************************/
void led(uint8_t num, uint8_t state)
{
	switch(num)
	{
		case 1:
			HAL_GPIO_WritePin(LED1,(GPIO_PinState)state);
			break;
		case 2:
			HAL_GPIO_WritePin(LED2,(GPIO_PinState)state);
			break;
	}
}