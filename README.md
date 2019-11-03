# flame

This project is based off [this](https://github.com/cfenollosa/os-tutorial) tutorial written by cfenollosa.

The [osdev wiki](https://wiki.osdev.org) has also greatly helped me in expanding this codebase.

## Building

You need the i686-elf cross compiler as well as binutils. You can grab the list of dependencies from [here](https://wiki.osdev.org/GCC_Cross-Compiler) and the cross compiler itself from [lordmilko's repository](https://github.com/lordmilko/i686-elf-tools).

My cross compiler is located in /opt/cross, so  Iwould highly recommend that you put it there as well in order to avoid Makefile conflicts.

Once you have installed all the dependencies, including i686-elf and binutils, just run `make` and it will build `os-image.bin`.

## Running

I personally use qemu, but you can use any virtualization software that you want.

```
qemu-system-x86_64 -soundhw pcspk -fda os-image.bin
```

Even though this is a 32 bit kernel, 64bit qemu should work fine. Alternatively, if you're feeling brave enough, you can test it on real hardware.

I wouldn't do that if I were you

## Filesystem Structure

`boot`: bootloader

`cpu`: contains any code that interacts with the cpu (isr, idt, interrupts, ports, timer)

`drivers`: drivers, woo

`kernel`: "frontend" code for the kernel

`include`: header files for libc

`libc`: useful functions

## License

This project is licensed under the MIT License. You should have recieved a copy of it along with this software.