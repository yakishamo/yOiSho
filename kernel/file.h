#ifndef FILE_H
#define FILE_H

#define FILE_NAME_MAX_SIZE 0x100
#define FILE_DATA_MAX_SIZE 0x100

#include "../common/types64.h"

typedef struct _FILE *FILE;

void initFilesystem(void *fat_image);
void FileList();
FILE openFile(const char *name);
int writeFile(FILE file, char *data);
int readFile(FILE file, char *buf, uint32_t size);
void file_test();

#endif /* FILE_H */
