HW's STM32F4 projects

Playing with a little STM32F407VET board - this is what I came up with.

Remember to check out submodule:

  git submodule init
  git submodule update

Just install a proper toolchain (arm-none-eabi) and run "make" in a project folder.
Use dfu-util to flash via DFU after putting the device into bootloader mode (set BOOT0 high).
