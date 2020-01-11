# flame

![alt text](https://cdn.discordapp.com/attachments/404064439571316747/665670368769605632/unknown.png "flameOS")

This project is based off [this](https://github.com/cfenollosa/os-tutorial) tutorial written by cfenollosa.

The [osdev wiki](https://wiki.osdev.org) has also greatly helped me in expanding this codebase.

## Building

You need the i686-elf cross compiler as well as binutils. You can grab the list of dependencies from [here](https://wiki.osdev.org/GCC_Cross-Compiler) and the cross compiler itself from [lordmilko's repository](https://github.com/lordmilko/i686-elf-tools).

Once you have installed all the dependencies, including i686-elf, binutils, and grub2, run `make` and it will build `flame.iso`.

## Running

```
qemu-system-x86_64 -d guest_errors -serial stdio -soundhw pcspk -m 1G -device isa-debug-exit,iobase=0xf4,iosize=0x04 -boot menu=on -cdrom flame.iso -hda flamedisk.img
```

or 

```
make run
```

You can also flash the iso to a USB drive and boot off that instead.

## Filesystem Structure

`cpu`: contains any code that interacts with the cpu (isr, idt, interrupts, ports, timer)

`drivers`: drivers=

`fs`: HDD I/O and Filesystem

`kernel`: "frontend" code for the kernel

`libc`: useful functions

## License

This project is licensed under the MIT License. You should have recieved a copy of it along with this software.