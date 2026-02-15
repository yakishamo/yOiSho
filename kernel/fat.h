#ifndef FAT_H
#define FAT_H

#include "../common/types64.h"

typedef struct FatFilesystem_ *FatFilesystem;
typedef struct DirEntry_ *DirEntry;

FatFilesystem loadFat(void* data);
void printRootDir(FatFilesystem fat);
size_t getFileData(FatFilesystem fat, DirEntry dir_ent, char *buf);

#endif /* FAT_H */
