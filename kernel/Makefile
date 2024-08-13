TARGET = kernel.elf
OBJS = main.o

CXXFLAGS += -O0 -Wall -g --target=x86_64-elf -ffreestanding -mno-red-zone \
						-fno-exceptions -fno-rtti
LDFLAGS += --entry KernelMain -z norelro --image-base 0x100000 --static

.PHONY: all
all: $(TARGET)

.PHONY: clean
clean:
	rm -rf *.o

kernel.elf: $(OBJS) Makefile
	ld.lld $(LDFLAGS) -o kernel.elf $(OBJS)

%.o: %.c Makefile
	gcc $(CXXFLAGS) -c $<
