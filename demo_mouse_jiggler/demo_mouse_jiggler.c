// Use LED on pin D4 (requires 'unbricking' to reprogram afterward).
//#define USE_D4

#include "ch32v003fun.h"
#include <stdio.h>
#include <string.h>
#include "rv003usb.h"

uint8_t rptmouse[4] = {0,0,0,0};
bool usb_active = false;

void led(unsigned short id, unsigned short on)
{
	switch (id)
	{
		case 0 : // A2
			if (on) GPIOA->BSHR = (1<<(2+16));
			else GPIOA->BSHR = (1<<2);
			break;
		case 1 : // D6
			if (on) GPIOD->BSHR = (1<<(6+16));
			else GPIOD->BSHR = (1<<6);
			break;
		#ifdef USE_D4
		case 2 : // D4
			if (on) GPIOD->BSHR = (1<<(4+16));
			else GPIOD->BSHR = (1<<4);
			break;
		#endif
	}
}

void gpio_setup(void)
{
	// Enable port module clocks.
	RCC->APB2PCENR |= RCC_IOPAEN|RCC_IOPDEN;
	// LEDS on PA2 and PD6 are P-P 2 MHz outputs.
	GPIOA->CFGLR &= ~(0b1111 << (2<<2));
	GPIOA->CFGLR |= (0b0010 << (2<<2));
	GPIOD->CFGLR &= ~(0b1111 << (6<<2));
	GPIOD->CFGLR |= (0b0010 << (6<<2));
	#ifdef USE_D4
	GPIOD->CFGLR &= ~(0b1111 << (4<<2));
	GPIOD->CFGLR |= (0b0010 << (4<<2));
	led(2, 1);
	#endif
	led(0, 0);
	led(1, 0);
}

int main()
{
	SystemInit();
	usb_setup();
	gpio_setup();
	while(1)
	{
		if (usb_active)
		{
			usb_active = false;
			led(1, 1); Delay_Ms(200);
		}
		else
		{
			led(1, 0); Delay_Ms(200);
			led(1, 1); Delay_Ms(200);
		}
	}
}

void hid_task(void) {
	int8_t const delta = 2;
	rptmouse[1] = 0; // Hold pointer position by default.
	usb_active = true;
	// Poll every 100ms
	const uint32_t interval_ms = 100;
	static uint32_t start_ms = 0;
	static uint32_t count = 0;

	if (SysTick->CNT - start_ms * DELAY_MS_TIME < interval_ms * DELAY_MS_TIME)
		return; // not enough time
	start_ms += interval_ms;

	switch (count & 0x1ff) {
		case 0:
		case 3:
			// no button, right, no scroll pan
			rptmouse[1] = delta;
			led(0, 1);
			break;
		case 1:
		case 2:
			// no button, left, no scroll pan
			rptmouse[1] = -delta;
			break;
		case 4:
			led(0, 0);
			break;
	}

	count++;
}

void usb_handle_user_in_request( struct usb_endpoint * e, uint8_t * scratchpad, int endp, uint32_t sendtok, struct rv003usb_internal * ist )
{
	if( endp )
	{
		hid_task();
		if (rptmouse[1])
			usb_send_data( rptmouse, 4, 0, sendtok );
		else
			usb_send_empty( sendtok );
	}
	else
	{
		// If it's a control transfer, nak it.
		usb_send_empty( sendtok );
	}
}
