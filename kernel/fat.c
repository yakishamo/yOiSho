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

typedef struct BPB_ *BPB;
struct BPB_ {
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
}__attribute__((packed));

struct DirEntry_ {
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
}__attribute((packed));

struct FatFilesystem_ {
	BPB bpb;
	DirEntry root_dir;
	uint64_t sec_per_clus;
	uint64_t bytes_per_clus;
	clus_num_t *clus_chain;
};

void* getClus(FatFilesystem fat, clus_num_t clus) {
	BPB bpb = fat->bpb;
	block_num_t head_blk = bpb->BPB_RsvdSecCnt + bpb->BPB_FATSz32 * bpb->BPB_NumFATs;
	block_num_t blk = head_blk + (clus-2) * bpb->BPB_SecPerClus;
	return (void*)(blk * bpb->BPB_BytsPerSec + (uintptr_t)bpb);
}

FatFilesystem loadFat(void *data) {
	FatFilesystem fat = kmalloc(sizeof(struct FatFilesystem_));
	fat->bpb = data;
	fat->sec_per_clus = fat->bpb->BPB_SecPerClus;
	fat->root_dir = (DirEntry)getClus(fat, (clus_num_t)fat->bpb->BPB_RootClus);
	fat->bytes_per_clus = fat->bpb->BPB_BytsPerSec * fat->sec_per_clus;
	fat->clus_chain = (clus_num_t*)(((uintptr_t)fat->bpb) + 
		fat->bpb->BPB_RsvdSecCnt * fat->bpb->BPB_BytsPerSec);
	return fat;
}

clus_num_t getStartClus(DirEntry dir_ent) {
	return (dir_ent->DIR_FstClusLO) | (dir_ent->DIR_FstClusHI << 16);
}

void printRootDir(FatFilesystem fat) {
	DirEntry root = fat->root_dir;
	int i = 0;
	while(1) {
		uint8_t name_head = root[i].DIR_Name[0];
		if(name_head == 0x00 || name_head == 0xE5) {
			break;
		} else if(name_head == 0x05) {
			i++;
			continue;
		}
		char str[12];
		strncpy(str, (char*)(root[i].DIR_Name), 11);
		str[11] = '\0';
		kprintf("%s\r\n", str);
		i++;
	}
}

// returns read file size
size_t getFileData(FatFilesystem fat, DirEntry dir_ent, char *buf) {
	clus_num_t clus = getStartClus(dir_ent);
	size_t clus_size = fat->bytes_per_clus;
	int read_size = 0;
	int remain_size = dir_ent->DIR_FileSize;
	while(clus < 0x0fffff80 && remain_size <= 0) {
		if(remain_size <= clus_size) {
			memcpy(&buf[read_size], getClus(fat, clus), remain_size);
			read_size += remain_size;
			break;
		}
		memcpy(&buf[read_size], getClus(fat, clus), clus_size);
		clus = fat->clus_chain[clus];
		read_size += clus_size;
	}
	return read_size;
}
