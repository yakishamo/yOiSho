#include "file.h"

#include "../common/types64.h"

#include "string.h"
#include "fat.h"
#include "kprintf.h"

#define DIRECTORY_MAX 4

struct _FILE {
	char name[FILE_NAME_MAX_SIZE];
	char data[FILE_DATA_MAX_SIZE];
	int size;
	bool available;
};

static FILE filesystem[DIRECTORY_MAX];

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
	kprintf("ReadFile() is not implemented!!\n\r");
	return 0;
}
