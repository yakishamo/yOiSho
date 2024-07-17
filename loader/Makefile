TARGET = BOOTX64.efi

.PHONY all:
all: $(TARGET)

$(TARGET): main.c Makefile
	x86_64-w64-mingw32-gcc -Wall -Wextra -e efi_main -nostdinc -nostdlib \
         -fno-builtin -Wl,--subsystem,10 -o $@ $<

run:
	cp ./BOOTX64.efi ./fs/EFI/BOOT/BOOTX64.efi
	qemu-system-x86_64 \
         -drive if=pflash,format=raw,file=./OVMF_CODE.fd,readonly=on \
         -drive if=pflash,format=raw,file=./OVMF_VARS.fd \
         -drive if=ide,format=raw,file=fat:rw:fs,index=0,media=disk \
				 -monitor stdio
