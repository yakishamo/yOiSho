#include "file.h"

#include "../common/types64.h"

#include "string.h"
#include "fat.h"
#include "kprintf.h"

#define DIRECTORY_MAX 4

struct _FILE {
	char name[FILE_NAME_MAX_SIZE];
	int size;
	DirEntry dir_ent;
};

static FatFilesystem FAT_FS = NULL;

void initFilesystem(void *fat_image) {
	FAT_FS = loadFat(fat_image);
}

void FileList() {
	printRootDir(FAT_FS);
}

int WriteFile(FILE file, char *data) {
	kprintf("WriteFile() is not implemented!!\n\r");
	return 0;
}

int ReadFile(FILE file, char *buf) {
	getFileData(FAT_FS, file->dir_ent, buf);
	return 0;
}
