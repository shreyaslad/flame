C_SOURCES = $(wildcard kernel/*.c drivers/*.c cpu/*.c libc/*.c kernel/commands/*.c )
HEADERS = $(wildcard kernel/*.h drivers/*.h cpu/*.h kernel/commands/*h)
OBJ = ${C_SOURCES:.c=.o cpu/interrupt.o} 

CC = /opt/cross/bin/i686-elf-gcc
GDB = /opt/cross/bin/i686-elf-gdb
CFLAGS = -g -m32 -nostdlib -fno-stack-protector -nostartfiles -nodefaultlibs \
		 -Wall -Wextra -Werror -Wno-unused-function -Wno-unused-variable -Wpedantic


flame.bin: boot/bootsect.bin kernel.bin
	cat $^ > flame.bin
	rm -rf kernel.bin *.dis *.o
	rm -rf kernel/*.o boot/*.bin drivers/*.o boot/*.o cpu/*.o libc/*.o

kernel.bin: boot/kernel_entry.o ${OBJ}
	/opt/cross/bin/i686-elf-ld -o $@ -Ttext 0x1000 $^ --oformat binary

kernel.elf: boot/kernel_entry.o ${OBJ}
	/opt/cross/bin/i686-elf-ld -o $@ -Ttext 0x1000 $^ 

run: flame.bin
	qemu-system-x86_64 -fda flame.bin

debug: flame.bin kernel.elf
	qemu-system-i386 -s -fda flame.bin -d guest_errors,int &
	${GDB} -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"

clean:
	rm -rf kernel.bin *.dis *.o *.elf
	rm -rf kernel/*.o boot/*.bin drivers/*.o boot/*.o cpu/*.o libc/*.o

%.o: %.c ${HEADERS}
	${CC} -Iinclude ${CFLAGS} -ffreestanding -c $< -o $@ -std=gnu11

%.o: %.asm
	nasm $< -f elf -o $@

%.bin: %.asm
	nasm $< -f bin -o $@
