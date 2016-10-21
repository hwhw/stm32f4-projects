Minimal example to show a bug in OTG USB handling for CDC-ACM driver

What I got so far:

- first, soon after connecting, some data is put into the USB output queue.

(1)
- a transmit is triggered for the OTG device, data is handed over to the TX FIFO
  via otg_fifo_write_from_buffer()
(2)
- host PC (ArchLinux, kernel version is 4.7.4) puts USB device to suspend soon thereafter.
  probably rightly so, since no terminal program is connected (yet) to the ACM tty.
- this triggers the callback in usbcfg.c:290 ...
- which will then call sduDisconnectI(&SDU1) ...
- which will - among other things - call obqResetI() on its output queue ...
- which will reset the queue, as its name suggests.

(3)
- at some point, a terminal program is connected to the ACM TTY on the host PC.
- this triggers a wakeup callback. Does nothing in default config in usbcfg.c.

(4)
- at *this* point, the OTG device actually sends the data from its FIFO and asserts
  the TX DONE interrupt (as per a usbmon Wireshark dump)
- this will be handled by sduDataTransmitted() ...
- which will dutyfully try to free the buffer using obqReleaseEmptyBufferI() ...
- which fails and triggers chSysHalt().

There is some critical timing and host action involved:
- at (1), the transmit must actually have been triggered
- at (2), the host PC must put the device into USB SUSPEND state before acknowledging the data
  In many cases - sometimes debugging was enough - at this point I did not get the "right"
  (or rather wrong) timing, so the TX DONE was signalled before the USB SUSPEND got in - all
  fine in those cases, no bug.

Rather dirty workaround for the bug:
- don't handle USB SUSPEND. See usbcfg.c for an experimental #ifndef.

Where's the bug?
- not sure if Linux should put the device into USB SUSPEND at that point
- in any case, the STM32 should not lock up even then
- OTG device might serve a TX DONE interrupt veeeeeery late.
  Note: the data actually has been transmit by the OTG device indeed right before the interrupt.
  So the OTG device probably isn't to blame
- ChibiOS' sduDisconnectI()
  - might have to make sure to prevent the OTG device from transmitting data later on?
  - probably just isn't the right thing to do when we get put into USB SUSPEND anyway
    (this is code from the demos/STM32/RT-STM32F407-OLIMEX_E407-LWIP-FATFS-USB example)
