C_SOURCES = $(wildcard kernel/*.c drivers/*.c cpu/*.c libc/*.c kernel/commands/*.c )
HEADERS = $(wildcard kernel/*.h drivers/*.h cpu/*.h kernel/commands/*h)
OBJ = ${C_SOURCES:.c=.o cpu/interrupt.o} 

CC = /opt/cross/bin/i686-elf-gcc
GDB = /opt/cross/bin/i386-elf-gdb
CFLAGS = -g -m32 -nostdlib -fno-stack-protector -nostartfiles -nodefaultlibs \
		 -Wall -Wextra -Werror -Wno-unused-function -Wno-unused-variable -Wpedantic


os-image.bin: boot/bootsect.bin kernel.bin
	cat $^ > os.bin

kernel.bin: boot/kernel_entry.o ${OBJ}
	/home/meemr/opt/cross/bin/i686-elf-ld -o $@ -Ttext 0x1000 $^ --oformat binary

kernel.elf: boot/kernel_entry.o ${OBJ}
	/home/meemr/opt/cross/bin/i686-elf-ld -o $@ -Ttext 0x1000 $^ 

run: os-image.bin
	qemu-system-x86_64 -sound pcspk -fda os-image.bin

debug: os-image.bin kernel.elf
	qemu-system-i386 -s -fda os-image.bin -d guest_errors,int &
	${GDB} -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"

%.o: %.c ${HEADERS}
	${CC} -Iinclude ${CFLAGS} -ffreestanding -c $< -o $@ -std=gnu11

%.o: %.asm
	nasm $< -f elf -o $@

%.bin: %.asm
	nasm $< -f bin -o $@

clean:
	rm -rf kernel.bin *.dis *.o *.elf
	rm -rf kernel/*.o boot/*.bin drivers/*.o boot/*.o cpu/*.o libc/*.o
