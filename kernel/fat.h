#ifndef FAT_H
#define FAT_H

typedef struct FatFilesystem_ *FatFilesystem;

FatFilesystem loadFat(void* data);
void printRootDir(FatFilesystem fat);

#endif /* FAT_H */
