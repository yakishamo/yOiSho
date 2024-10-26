TARGET = kernel.elf
OBJS = asmfunc.o main.o hankaku.o frame.o string.o keyboard.o file.o cursor.o parse.o editor.o \
			 terminal.o memory.o

CC=clang
CXXFLAGS += -O0 -Wall -g --target=x86_64-elf -ffreestanding -mno-red-zone \
						-fno-exceptions -fno-rtti -Werror
LDFLAGS += --entry KernelEntryPoint -z norelro --image-base 0x100000 --static

.PHONY: all
all: $(TARGET)

.PHONY: clean
clean:
	rm -rf *.o *.elf

kernel.elf: $(OBJS) Makefile
	ld.lld $(LDFLAGS) -o kernel.elf $(OBJS)

string.o: string.c Makefile
	$(CC) -O3 -Wall -g --target=x86_64-elf -ffreestanding -mno-red-zone -fno-exceptions -fno-rtti -c -o string.o string.c

asmfunc.o: asmfunc.asm
	nasm -f elf64 -o asmfunc.o asmfunc.asm

%.o: %.c Makefile
	$(CC) $(CXXFLAGS) -c -o $@ $<
