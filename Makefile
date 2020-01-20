C_SOURCES = $(wildcard kernel/*.c drivers/*.c cpu/*.c libc/*.c kernel/commands/*.c fs/*.c )
HEADERS = $(wildcard kernel/*.h drivers/*.h cpu/*.h kernel/commands/*h fs/*h )
S_SOURCES = $(wildcard *.s)
OBJ = ${C_SOURCES:.c=.o cpu/interrupt.o boot.o startup64.o	} 

ARCH=x86_64
CROSS=/opt/cross/bin

CC = ${CROSS}/${ARCH}-elf-gcc
GDB = ${ARCH}-elf-gdb
CFLAGS = -ggdb -nostdlib -fno-stack-protector -nostartfiles -nodefaultlibs \
		 -Wall -Wextra -Wno-unused-function -Wno-unused-variable -Wpedantic -ffreestanding -ggdb -std=gnu11
O_LEVEL = 2

LDFLAGS = -ffreestanding -O2 -nostdlib -z max-page-size=0x1000

myos.iso: kernel.elf
	mkdir -p isodir/boot/grub
	cp kernel.elf isodir/boot/flame.bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o flame.iso isodir
	make clean

kernel.bin: kernel.elf
	objcopy -O binary $^ $@

kernel.elf: ${OBJ}
	${CC} -T linker.ld -o $@ ${LDFLAGS} $^ -lgcc

run: flame.iso # -serial stdio
	qemu-system-${ARCH} -monitor stdio -soundhw pcspk -m 1G -device isa-debug-exit,iobase=0xf4,iosize=0x04 -boot menu=on -cdrom flame.iso -hda flamedisk.img

debug: flame.iso kernel.elf
	qemu-system-${ARCH} -s -S -no-reboot -no-shutdown -d guest_errors -serial stdio -soundhw pcspk -m 1G -device isa-debug-exit,iobase=0xf4,iosize=0x04 -boot menu=on -cdrom flame.iso -hda flamedisk.img &
    ${GDB} -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"

clean:
	rm -rf kernel.bin *.dis *.o
	rm -rf kernel/*.o boot/*.bin drivers/*.o boot/*.o cpu/*.o libc/*.o fs/*.o

cpu/interrupt.o: cpu/interrupt.asm
	nasm -f elf64 $< -o $@

%.o: %.c ${HEADERS}
	${CC} -Iinclude ${CFLAGS} -c $< -o $@

%.o: %.asm
	nasm $< -f elf64 -o $@