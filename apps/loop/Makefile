TARGET = loop.elf
CC = clang
CXXFLAGS = -O0 -Wall -g --target=x86_64-elf -ffreestanding -mno-red-zone -std=c17 \
						-fno-exceptions -fno-rtti
LDFLAGS = --entry main -z norelro --static

.PHONY: all
all: $(TARGET)

.PHONY: clean
clean:
	rm -rf *.o $(TARGET)

loop.elf: main.c Makefile
	$(CC) $(CXXFLAGS) -c -o loop.o $<
	ld.lld $(LDFLAGS) -o $@ loop.o
	chmod -x $@
