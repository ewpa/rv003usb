#include "ch32v003fun.h"
#include <stdio.h>
#include <string.h>
#include "rv003usb.h"

uint8_t rptmouse[4] = {0,0,0,0};

int main()
{
	SystemInit();
	usb_setup();
	while(1) {}
}

void hid_task(void) {
	int8_t const delta = 2;
	rptmouse[1] = 0; // Hold pointer position by default.
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
			break;
		case 1:
		case 2:
			// no button, left, no scroll pan
			rptmouse[1] = -delta;
			break;
	}

	count++;
}

void usb_handle_user_in_request( struct usb_endpoint * e, uint8_t * scratchpad, int endp, uint32_t sendtok, struct rv003usb_internal * ist )
{
	if( endp )
	{
		hid_task();
		usb_send_data( rptmouse, 4, 0, sendtok );
	}
	else
	{
		// If it's a control transfer, nak it.
		usb_send_empty( sendtok );
	}
}
