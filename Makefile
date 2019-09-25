C_SOURCES = $(wildcard kernel/*.c drivers/*.c cpu/*.c include/*.c kernel/commands/*.c )
HEADERS = $(wildcard kernel/*.h drivers/*.h cpu/*.h include/*.h kernel/commands/*h)
OBJ = ${C_SOURCES:.c=.o cpu/interrupt.o} 

CC = /home/meemr/opt/cross/bin/i686-elf-gcc
CFLAGS = -g -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs \
		 -Wall -Wextra -Werror -Wno-unused-function -Wno-unused-variable

os-image.bin: boot/bootsect.bin kernel.bin
	cat $^ > os-image.bin

kernel.bin: boot/kernel_entry.o ${OBJ}
	/home/meemr/opt/cross/bin/i686-elf-ld -o $@ -Ttext 0x1000 $^ --oformat binary

kernel.elf: boot/kernel_entry.o ${OBJ}
	/home/meemr/opt/cross/bin/i686-elf-ld -o $@ -Ttext 0x1000 $^ 

run: os-image.bin
	qemu-system-i386 -fda os-image.bin

%.o: %.c ${HEADERS}
	${CC} ${CFLAGS} -ffreestanding -c $< -o $@

%.o: %.asm
	nasm $< -f elf -o $@

%.bin: %.asm
	nasm $< -f bin -o $@

clean:
	rm -rf kernel.bin *.dis *.o *.elf
	rm -rf kernel/*.o boot/*.bin drivers/*.o boot/*.o cpu/*.o include/*.o
