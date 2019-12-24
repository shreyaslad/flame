C_SOURCES = $(wildcard kernel/*.c drivers/*.c cpu/*.c libc/*.c kernel/commands/*.c )
HEADERS = $(wildcard kernel/*.h drivers/*.h cpu/*.h kernel/commands/*h)
S_SOURCES = $(wildcard *.s)
OBJ = ${C_SOURCES:.c=.o cpu/interrupt.o boot.o	} 

CC = /opt/cross/bin/i686-elf-gcc
GDB = /opt/cross/bin/i686-elf-gdb
CFLAGS = -g -m32 -nostdlib -fno-stack-protector -nostartfiles -nodefaultlibs \
		 -Wall -Wextra -Werror -Wno-unused-function -Wno-unused-variable -Wpedantic
O_LEVEL = 3

myos.iso: kernel.elf
	grub-file --is-x86-multiboot kernel.elf
	mkdir -p isodir/boot/grub
	cp kernel.elf isodir/boot/flame.bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o flame.iso isodir
	rm -rf kernel.bin *.dis *.o *.elf
	rm -rf *.o kernel/*.o boot/*.bin drivers/*.o boot/*.o cpu/*.o libc/*.o

kernel.bin: kernel.elf
	objcopy -O binary $^ $@

kernel.elf: ${OBJ}
	/opt/cross/bin/i686-elf-ld -melf_i386 -o $@ -T linker.ld $^

run: flame.bin
	qemu-system-x86_64 -d guest_errors -serial stdio -soundhw pcspk -m 1G -device isa-debug-exit,iobase=0xf4,iosize=0x04 -boot menu=on -cdrom flame.iso -hda flamedisk.img

debug: flame.bin kernel.elf
	qemu-system-i386 -s -fda flame.bin -d guest_errors,int &
	${GDB} -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"

clean:
	rm -rf kernel.bin *.dis *.o *.elf
	rm -rf kernel/*.o boot/*.bin drivers/*.o boot/*.o cpu/*.o libc/*.o

%.o: %.c ${HEADERS}
	${CC} -Iinclude ${CFLAGS} -ffreestanding -c $< -o $@ -std=gnu11

%.o: %.s
	${CC} -Werror -Wall -Wextra -Wpedantic -O${O_LEVEL} -g -MD -c $< -o $@

%.o: %.asm
	nasm $< -f elf -o $@

#%.bin: %.asm
#	nasm $< -f bin -o $@
