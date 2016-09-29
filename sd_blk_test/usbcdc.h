#ifndef USBCDC_H
#define USBCDC_H

#include <stdbool.h>
#include <stdint.h>

void usbcdc_init(void);
void usbcdc_poll(void);
int usbcdc_send(void* buf, int len);
int usbcdc_sends(char *s);
int usbcdc_send_hex(uint8_t* buf, int len);
int usbcdc_have_rx(void);
uint8_t* usbcdc_rx(int *len);

#endif // USBCDC_H
