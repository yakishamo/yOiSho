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

int writeFile(FILE file, char *data) {
	kprintf("WriteFile() is not implemented!!\n\r");
	return 0;
}

int readFile(FILE file, char *buf) {
	getFileData(FAT_FS, file->dir_ent, buf);
	return 0;
}

void file_test() {
	DirEntry ent;
	ent = getDirEntryByName(FAT_FS, "kernel.elf");
	if(!ent) {
		kprintf("kernel.elf not found\n");
		return;
	}
	kprintf("kernel.elf found\n");
	kprintf("filesize: 0x%x\n", getFileSize(ent));
	char name[20];
	getDirName(ent, name);
	kprintf("filename: %s\n", name);
}
