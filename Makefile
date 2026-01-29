.PHONY all:
all: BOOTX64.EFI kernel.elf
	
BOOTX64.EFI:
	cd "$(PWD)/loader" && make

kernel.elf:
	cd "$(PWD)/kernel" && make

DISK_IMG=disk.img
MOUNT_POINT=mnt
UNAME_S := $(shell uname -s)

cp:
	cp ./loader/BOOTX64.EFI $(MOUNT_POINT)/EFI/BOOT/BOOTX64.EFI
	cp ./kernel/kernel.elf ./$(MOUNT_POINT)/kernel.elf

mount-macos:
	@DEV=`hdiutil attach -nomount $(DISK_IMG) | head -n1 | awk '{print $$1}'` ; \
	echo "using $$DEV" ; \
	sudo mount_msdos $$DEV $(MOUNT_POINT)

mount-linux:
	sudo mount $(DISK_IMG) $(MOUNT_POINT)

umount-linux:
	sudo umount $(DISK_IMG)

umount-macos:
	@DEV=`mount | grep "$(PWD)/$(MOUNT_POINT)" | awk '{print $$1}'` ; \
	sudo umount $(MOUNT_POINT) ; \
	hdiutil detach $$DEV

run:
ifeq ($(UNAME_S),Darwin)
	make mount-macos
	make cp
	make umount-macos
else ifeq ($(UNAME_S),Linux)
	make mount-linux
	make cp
	make umount-linux
else
	@echo "Unsupported OS: $(UNAME_S)"
	@exit1
endif
	qemu-system-x86_64 \
    -no-reboot \
		-m 4G \
		-drive if=pflash,format=raw,file=./ovmf/OVMF_CODE.fd,readonly=on \
		-drive if=pflash,format=raw,file=./ovmf/OVMF_VARS.fd \
		-drive if=ide,index=0,media=disk,format=raw,file=$(DISK_IMG) \
    -device nec-usb-xhci,id=xhciv \
		-nographic \
    -d int \
    -D qemu_debug.log

init:
	mkdir -p fs/EFI/BOOT || :
	mkdir ovmf || :
	mkdir $(MOUNT_POINT) || :
	qemu-img create -f raw $(DISK_IMG) 200M
	mkfs.fat -n 'YOISHO' -s 2 -f 2 -R 32 -F 32 $(DISK_IMG)
ifeq ($(UNAME_S),Darwin)
	make mount-macos
	mkdir -p mnt/EFI/BOOT
	make umount-macos
else ifeq ($(UNAME_S),Linux)
	make init-linux
else
	@echo "Unsupported OS: $(UNAME_S)"
	@exit1
endif


