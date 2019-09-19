# OS

I don't really know what to call this but OS is fine.

I followed [this](https://github.com/cfenollosa/os-tutorial) tutorial to get a better understanding of how operating systems function.

## Building

The Makefile uses the path of my cross compiler, but I'm working on moving it to a more universal address. 

If you want to commit, just make sure to untrack the Makefile.

```
make
```
will produce a file called `os-image.bin`, if all goes well.

## Running

I personally use qemu, but you can use any virtualization software that you want.

```
qemu-system-x86_64 -fda os-image.bin
```

Even though this is a 32 bit kernel, 64bit qemu should work fine. Alternatively, if you're feeling brave enough, you can test it on real hardware.

I wouldn't do that if I were you

## Filesystem Structure

`boot`: contains bootloader code

`cpu`: contains any code that interacts with the cpu (isr, idt, interrupts, ports, timer)

`drivers`: drivers, woo

`kernel`: "frontend" code for the kernel

`libc`: useful header files and functions

## License

This project is licensed under the MIT License.