#include "file.h"

#include "../common/types64.h"

#include "string.h"
#include "fat.h"
#include "kmalloc.h"
#include "kprintf.h"

#define DIRECTORY_MAX 4

struct _FILE {
	char name[FILE_NAME_MAX_SIZE];
	uint32_t size;
	void *data;
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
	kprintf("WriteFile() is not implemented!!\n");
	return 0;
}

int readFile(FILE file, char *buf, uint32_t size) {
	if(!file) {
		kprintf("file is NULL\n");
		return 1;
	}
	int read_size = size > file->size ? file->size : size;
	kprintf("read_size: %d\n", read_size);
	memcpy(buf, file->data, read_size);
	return 0;
}

FILE openFile(const char *name) {
	DirEntry ent;
	ent = getDirEntryByName(FAT_FS, name);
	if(!ent) {
		kprintf("openFile() Failed.");
		return NULL;
	}
	FILE file = kmalloc(sizeof(FILE));
	if(!file) {
		kprintf("openFile: kmalloc failed(%d).", __LINE__);
		return NULL;
	} 
	strncpy(file->name, name, FILE_NAME_MAX_SIZE);
	file->data = kmalloc(file->size);
	file->size = getEntSize(ent);
	if(!(file->data)) {
		kprintf("openFile: kmalloc failed(%d).\n", __LINE__);
		return NULL;
	}
	getFileData(FAT_FS, ent, file->data);
	file->dir_ent = ent;
	
	return file;
} 

uint32_t getFileSize(FILE file) {
	if(file) {
		return file->size;
	} else {
		return 0;
	}
}

void file_test() {
	DirEntry ent;
	ent = getDirEntryByName(FAT_FS, "kernel.elf");
	if(!ent) {
		kprintf("kernel.elf not found\n");
		return;
	}
	kprintf("kernel.elf found\n");
	kprintf("filesize: 0x%x\n", getEntSize(ent));
	char name[20];
	getDirName(ent, name);
	kprintf("filename: %s\n", name);
}
