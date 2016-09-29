HW's STM32F4 projects
-------------------

I'm experimenting with cheap China-sourced STM32F407VET6 boards
(about 10-12 EUR per board, with uSD reader, micro USB, LDO and
some extra stuff). Here I collect firmware examples.

First steps:
------------

* install an ARM-none-eabi toolchain
* optional: install ARM-none-eabi GDB
* install software that allows you to put your firmware onto the
  controller (you only need ONE of these):
  * dfu-util (this is recommended)
  * st-link (if you have an st-link programmer)
  * openocd (if you have an st-link programmer - as an alternative -
    or some other more obscure tool)
  * stm32flash (when you're going to flash via UART)
  * or visit a friend who will flash a USB bootloader for you
    (see below/in usb_dfu)

* initialize libopencm3 submodule:

# submodule init
# submodule update

* build libopencm3

# cd libopencm3 && make

* build projects you're interested in

# cd blinky && make
(or some other project folder)

Flashing:
---------

When using the USB bootloader, just run

# make dfu-flash

(this will build the firmware and runs dfu-flash with config for the
USB bootloader)

In order to activate the (USB, plus UART and others) bootloader:

* set BOOT0 to 1, BOOT1 to 0 (boot from internal memory - ROM bootloader)


List of projects:
-----------------

blinky:

  Just blink the onboard LED. Start with this!

sd_blk_test:

  Run a few tests on a uSD card. Controlled via USB (CDC-ACM).


Your own work?
--------------

See libopencm3-examples (https://github.com/libopencm3/libopencm3-examples/)
to see how to get things going. You'll probably need to adapt GPIO pins
(at least) for your target platform.

