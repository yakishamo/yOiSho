.PHONY all:
all: BOOTX64.EFI kernel.elf
	
BOOTX64.EFI:
	cd "$(PWD)/loader" && make

kernel.elf:
	cd "$(PWD)/kernel" && make

run:
	cp ./loader/BOOTX64.EFI ./fs/EFI/BOOT/BOOTX64.EFI
	cp ./kernel/kernel.elf ./fs/kernel.elf
	qemu-system-x86_64 \
		-m 4G \
		-drive if=pflash,format=raw,file=./ovmf/OVMF_CODE.fd,readonly=on \
		-drive if=pflash,format=raw,file=./ovmf/OVMF_VARS.fd \
		-drive if=ide,format=raw,file=fat:rw:fs,index=0,media=disk \
		-monitor stdio

init:
	mkdir -p fs/EFI/BOOT
