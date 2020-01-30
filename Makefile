C_SOURCES = $(wildcard kernel/*.c drivers/*.c cpu/*.c libc/*.c kernel/commands/*.c fs/*.c )
HEADERS = $(wildcard kernel/*.h drivers/*.h cpu/*.h kernel/commands/*h fs/*h )
OBJ = ${C_SOURCES:.c=.o cpu/interrupt.o	} 

ARCH=x86_64
CROSS=/opt/cross/bin

CC = ${CROSS}/${ARCH}-elf-gcc
GDB = gdb
CFLAGS = -ggdb -nostdlib -fno-stack-protector -nostartfiles -nodefaultlibs \
		 -Wall -Wextra -Wno-unused-function -Wno-unused-variable -Wpedantic -ffreestanding -ggdb -std=gnu11 -mcmodel=kernel
O_LEVEL = 2

LDFLAGS = -ffreestanding -O${O_LEVEL} -nostdlib -z max-page-size=0x1000

flame.iso: flame.bin
	mkdir -p isodir/boot/grub
	cp flame.bin isodir/boot/flame.bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o flame.iso isodir
	make clean

flame.bin: flame.elf
	objcopy -O binary $^ $@

flame.elf: boot.elf kernel.elf
	cat $^ > flame.elf

kernel.elf: ${OBJ}
	${CC} -T linker.ld -o $@ ${LDFLAGS} $^ -lgcc

cpu/interrupt.o: cpu/interrupt.asm
	nasm -f elf64 $< -o $@

%.o: %.c ${HEADERS}
	${CC} -Iinclude ${CFLAGS} -c $< -o $@

boot.elf: boot.asm
	nasm -f elf64 $< -o boot.o
	${CC} -T linker.ld -o boot.elf ${LDFLAGS} boot.o -lgcc

run: flame.iso # -serial stdio
	qemu-system-${ARCH} -no-reboot -no-shutdown -d int -monitor stdio -soundhw pcspk -m 1G -device isa-debug-exit,iobase=0xf4,iosize=0x04 -boot menu=on -cdrom flame.iso -hda flamedisk.img

debug: flame.iso kernel.elf
	qemu-system-${ARCH} -s -S -d guest_errors,int -serial stdio -soundhw pcspk -m 1G -device isa-debug-exit,iobase=0xf4,iosize=0x04 -boot menu=on -cdrom flame.iso -hda flamedisk.img &
	${GDB} -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"

clean:
	rm -rf kernel.bin *.dis *.o
	rm -rf kernel/*.o boot/*.bin drivers/*.o boot/*.o cpu/*.o libc/*.o fs/*.o