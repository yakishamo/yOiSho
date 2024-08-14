#include "file.h"

#include "../common/types64.h"

#include "string.h"

#define DIRECTORY_MAX 5

struct _FILE {
	char name[FILE_NAME_MAX_SIZE];
	char data[FILE_NAME_MAX_SIZE];
	int size;
	bool available;
};

static FILE filesystem[DIRECTORY_MAX];

void InitializeFilesystem() {
	memset((char*)filesystem, 0, sizeof(filesystem));
}

FILE *CreateFile(char name[], char data[], int size) {
	int i = 0;
	if(SearchFile(name) != NULL) return NULL;
	while(filesystem[i].available && i < DIRECTORY_MAX) i++;
	if(i == DIRECTORY_MAX) return NULL;

	strncpy(filesystem[i].name, name, FILE_NAME_MAX_SIZE);
	strncpy(filesystem[i].data, data, FILE_NAME_MAX_SIZE);
	filesystem[i].size = size;
	filesystem[i].available = true;
	return filesystem + i;
}	

FILE *SearchFile(char name[]) {
	for(int i = 0; i < DIRECTORY_MAX; i++) {
		if(strcmp(filesystem[i].name, name) == 0 && filesystem[i].available) {
			return filesystem + i;
		}
	}
	return NULL;
}

void DeleteFile(char name[]) {
	FILE *f = SearchFile(name);
	if(f == NULL) {
		return;
	}
	f->available = false;
	f->size = 0;
	return;
}

char *GetFileData(FILE *file) {
	return file->data;
}

char file_name_list[FILE_NAME_MAX_SIZE * (DIRECTORY_MAX+1)];
char *FileList() {
	memset(file_name_list, 0, sizeof(file_name_list));
	for(int i = 0; i < DIRECTORY_MAX; i++) {
		if(filesystem[i].available) {
			strcat(file_name_list, filesystem[i].name);
			strcat(file_name_list, " ");
		}
	}
	return file_name_list;
}
