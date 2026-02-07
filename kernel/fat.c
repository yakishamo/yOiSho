#include "fat.h"
#include "kprintf.h"
#include "kmalloc.h"
#include "string.h"

typedef struct BPB_ {
	uint8_t  BS_jmpBoot[3];
	uint8_t  BS_OEMName[8];
	uint16_t BPB_BytesPerSec;
	uint8_t  BPB_SecPerClus;
	uint16_t BPB_RsvdSecCnt;
	uint8_t  BPB_NumFATs;
	uint16_t BPB_RootEntCnt;
	uint16_t BPB_TotSec16;
	uint8_t  BPB_Media;
	uint16_t BPB_FATSz16;
	uint16_t BPB_SecPerTrk;
	uint16_t BPB_NumHeads;
	uint32_t BPB_HiddSec;
	uint32_t BPB_TotSec32;
	uint32_t BPB_FATSz32;
	uint16_t BPB_ExtFlags;
	uint16_t BPB_FSVer;
	uint32_t BPB_RootClus;
	uint16_t BPB_FSInfo;
	uint16_t BPB_BkBootSec;
	uint8_t  BPB_Reserved[12];
	uint8_t  BS_DrvNum;
	uint8_t  BS_Reserved1;
	uint8_t  BS_BootSig;
	uint32_t BS_VolID;
	uint8_t  BS_VolLab[11];
	uint8_t  BS_FilSysType[8];
}__attribute__((packed)) BPB;

struct FatFileSystem_ {
	BPB *bpb;
};

FatFileSystem loadFat(void *data) {
	FatFileSystem fat = kmalloc(sizeof(struct FatFileSystem_));
	fat->bpb = data;
	return fat;
}

void printVolumeName(FatFileSystem fat) {
	BPB* bpb = fat->bpb;
	char *str = kmalloc(20);
	memcpy(str, bpb->BS_VolLab, 11);
	str[11] = 0;
	kprintf("volume name: %s\r\n", str);
	kfree(str);
}
