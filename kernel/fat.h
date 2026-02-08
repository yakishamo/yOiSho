#ifndef FAT_H
#define FAT_H

typedef struct FatFileSystem_ *FatFileSystem;

FatFileSystem loadFat(void* data);
void printVolume(FatFileSystem fat);

#endif /* FAT_H */
