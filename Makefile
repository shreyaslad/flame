.PHONY: run clean debug format

C_SOURCES = $(shell find . -type f -name '*.c')
HEADERS = $(shell find . -type f -name '*.h')
ASM_SOURCES = $(shell find . -type f -name '*.asm')
OBJ = ${C_SOURCES:.c=.o} ${ASM_SOURCES:.asm=.o} 

ARCH=x86_64
CROSS=/opt/cross/bin

CC = ${CROSS}/${ARCH}-elf-gcc
GDB = gdb
CFLAGS = -ggdb \
		 -nostdlib \
		 -fno-stack-protector \
		 -nostartfiles \
		 -nodefaultlibs \
		 -Wall \
		 -Wextra \
		 -Wno-unused-function \
		 -Wno-unused-variable \
		 -Wpedantic \
		 -ffreestanding \
		 -std=gnu11 \
		 -mcmodel=kernel
		 
QEMUFLAGS = -soundhw pcspk -m 1G \
			-device isa-debug-exit,iobase=0xf4,iosize=0x04 \
			-boot menu=on \
			-cdrom flame.iso \
			-hda flamedisk.img \
O_LEVEL = 2

LDFLAGS = -ffreestanding -O${O_LEVEL} -nostdlib -z max-page-size=0x1000

LAI_DIR = drivers/acpi/lai
LAI_URL = https://github.com/qword-os/lai.git

flame.iso: ${LAI_DIR} kernel32.elf
	mkdir -p isodir/boot/grub
	cp kernel32.elf isodir/boot/flame.bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o flame.iso isodir
	make clean

${LAI_DIR}:
	git submodule add ${LAI_URL} ${LAI_DIR}

kernel.bin: kernel32.elf
	objcopy -O binary $^ $@

kernel32.elf: kernel.elf
	objcopy -O elf32-i386 kernel.elf kernel32.elf

kernel.elf: ${OBJ}
	${CC} -Wl,-z,max-page-size=0x1000 -nostdlib -o $@ -T linker.ld $^

%.o: %.c ${HEADERS}
	${CC} -Iinclude -Iinclude/libc -Idrivers/acpi/lai/include ${CFLAGS} -c $< -o $@

%.o: %.asm
	nasm -f elf64 $< -o $@

run:
	qemu-system-${ARCH} ${QEMUFLAGS} -serial stdio -enable-kvm

debug: flame.iso
	qemu-system-${ARCH} -s -S ${QEMUFLAGS} &
	${GDB} -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"

clean:
	find . -type f -name '*.o' -delete
	rm -rf kernel32.elf isodir/ .vscode/

format:
	clang-format-9 -style=file -i **/*.[hc]