#include "fat.h"
#include "kprintf.h"
#include "kmalloc.h"
#include "string.h"

typedef uint32_t clus_num_t;
typedef uint64_t block_num_t;

typedef enum FILE_ATTR_ {
	ATTR_READ_ONLY = 0x01,
	ATTR_HIDDEN    = 0x02,
	ATTR_SYSTEM    = 0x04,
	ATTR_VOLUME_ID = 0x08,
	ATTR_DIRECTORY = 0x10,
	ATTR_ARCHIVE   = 0x20,
	ATTR_LONG_NAME = 0x0f,
} FILE_ATTR;

typedef struct BPB_ {
	uint8_t  BS_jmpBoot[3];
	uint8_t  BS_OEMName[8];
	uint16_t BPB_BytsPerSec;
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

typedef struct DirEntry_ {
	uint8_t  DIR_Name[11];
	uint8_t  DIR_Attr;
	uint8_t  DIR_NTRes;
	uint8_t  DIR_CrtTimeTenth;
	uint16_t DIR_CrtTime;
	uint16_t DIR_CrtDate;
	uint16_t DIR_LstAccDate;
	uint16_t DIR_FstClusHI;
	uint16_t DIR_WrtTime;
	uint16_t DIR_WrtDate;
	uint16_t DIR_FstClusLO;
	uint32_t DIR_FileSize;
}__attribute((packed)) DirEntry;

struct FatFileSystem_ {
	BPB *bpb;
	DirEntry *root_dir;
	uint64_t sec_per_clus;
};

void* getClus(FatFileSystem fat, clus_num_t clus) {
	BPB *bpb = fat->bpb;
	block_num_t head_blk = bpb->BPB_RsvdSecCnt + bpb->BPB_FATSz32 * bpb->BPB_NumFATs;
	block_num_t blk = head_blk + (clus-2) * bpb->BPB_SecPerClus;
	kprintf("offset: 0x%x\r\n", blk * bpb->BPB_BytsPerSec);
	return (void*)(blk * bpb->BPB_BytsPerSec + (uintptr_t)bpb);
}

FatFileSystem loadFat(void *data) {
	FatFileSystem fat = kmalloc(sizeof(struct FatFileSystem_));
	fat->bpb = data;
	fat->sec_per_clus = fat->bpb->BPB_SecPerClus;
	fat->root_dir = (DirEntry*)getClus(fat, (clus_num_t)fat->bpb->BPB_RootClus);
	return fat;
}

void printVolume(FatFileSystem fat) {
	BPB* bpb = fat->bpb;
	uint8_t *str = kmalloc(20);
	memcpy(str, bpb->BS_VolLab, 11);
	str[11] = 0;
	kprintf("volume name:%s\r\n", str);
	memset(str, 0, 20);
	DirEntry *ent = fat->root_dir;
	int i = 0;
	while(1) {
		uint8_t *ent_name = ent[i].DIR_Name;
		if(ent[i].DIR_Attr == ATTR_LONG_NAME) {
			i++;
			continue;
		} else if(ent_name[0] == 0) {
			break;
		} else if(ent_name[0] == 0xe5) {
			i++;
			continue;
		}
		memcpy(str, ent_name, 11);
		str[11] = 0;
		kprintf("%s\r\n", str);
		memset(str, 0, 20);
		i++;
	}
	kfree(str);
}
