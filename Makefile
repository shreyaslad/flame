C_SOURCES = $(wildcard kernel/*.c drivers/*.c cpu/*.c libc/*.c kernel/commands/*.c fs/*.c mm/*.c )
HEADERS = $(wildcard kernel/*.h drivers/*.h cpu/*.h kernel/commands/*h fs/*h mm/*.h )
OBJ = ${C_SOURCES:.c=.o cpu/interrupt.o	} 

ARCH=x86_64
CROSS=/opt/cross/bin

CC = ${CROSS}/${ARCH}-elf-gcc
GDB = gdb
CFLAGS = -ggdb -nostdlib -fno-stack-protector -nostartfiles -nodefaultlibs \
		 -Wall -Wextra -Wno-unused-function -Wno-unused-variable -Wpedantic -ffreestanding -ggdb -std=gnu11 -mcmodel=kernel
O_LEVEL = 2

LDFLAGS = -ffreestanding -O${O_LEVEL} -nostdlib -z max-page-size=0x1000

flame.iso: kernel32.elf
	mkdir -p isodir/boot/grub
	cp kernel32.elf isodir/boot/flame.bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o flame.iso isodir
	make clean

kernel.bin: kernel32.elf
	objcopy -O binary $^ $@

kernel32.elf: kernel.elf
	objcopy -O elf32-i386 kernel.elf kernel32.elf

kernel.elf: startup64.o boot.o ${OBJ}
	${CC} -Wl,-z,max-page-size=0x1000 -nostdlib -o $@ -T linker.ld $^

cpu/interrupt.o: cpu/interrupt.asm
	nasm -f elf64 $< -o $@

%.o: %.c ${HEADERS}
	${CC} -Iinclude ${CFLAGS} -c $< -o $@

boot.o: boot.asm
	nasm -f elf64 boot.asm -o boot.o

startup64.o: startup64.asm
	nasm -f elf64 startup64.asm -o startup64.o

run: flame.iso # -serial stdio
	qemu-system-${ARCH} -serial stdio -soundhw pcspk -m 1G -device isa-debug-exit,iobase=0xf4,iosize=0x04 -boot menu=on -cdrom flame.iso -hda flamedisk.img

debug: flame.iso kernel.elf
	qemu-system-${ARCH} -s -S -d guest_errors,int -serial stdio -soundhw pcspk -m 1G -device isa-debug-exit,iobase=0xf4,iosize=0x04 -boot menu=on -cdrom flame.iso -hda flamedisk.img &
	${GDB} -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"

clean:
	rm -rf kernel.bin *.dis *.o
	rm -rf kernel/*.o boot/*.bin drivers/*.o boot/*.o cpu/*.o libc/*.o fs/*.o mm/*.o
	rm -rf flame.bin flame.elf kernel32.elf isodir/