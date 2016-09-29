#include <libopencm3/cm3/scb.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#include "usbcdc.h"
#include "sdio.h"

volatile uint32_t system_millis;

void sys_tick_handler(void) {
	++system_millis;
	if (system_millis == 0) {
		scb_reset_system();
	}
}

int main(void)
{
	rcc_clock_setup_hse_3v3(&rcc_hse_25mhz_3v3[RCC_CLOCK_3V3_120MHZ]);

	rcc_periph_clock_enable(RCC_GPIOB);
	gpio_mode_setup(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO9);
	gpio_toggle(GPIOB, GPIO9); // switch LED off

	systick_set_reload(168000);
	systick_clear();
	systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
	systick_counter_enable();
	systick_interrupt_enable();

	sdio_init();
	usbcdc_init();

	char* d;
	int count = 0;
	uint8_t blk[512];
	while (1) {
		usbcdc_poll();
		d = (char*) usbcdc_rx(&count);
		if(count) {
			if(d[0] == 'i') {
				if(sdio_card_init()) {
					usbcdc_sends("OK\r\n");
				} else {
					usbcdc_sends("Error.\r\n");
				}
			} else if(d[0] == 'r') {
				if(sdio_read_block(0, (uint32_t*)blk)) {
					usbcdc_send_hex(&blk[0x1be], 30);
				} else {
					usbcdc_sends("Error.\r\n");
				}
			} else {
				usbcdc_sends("invalid cmd. Valid: i,r\r\n");
			}
		}
	}

	return 0;
}
