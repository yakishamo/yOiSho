#include "file.h"

#include "../common/types64.h"

#include "string.h"

#define DIRECTORY_MAX 4

struct _FILE {
	char name[FILE_NAME_MAX_SIZE];
	char data[FILE_DATA_MAX_SIZE];
	int size;
	bool available;
};

static FILE filesystem[DIRECTORY_MAX];

void InitializeFilesystem() {
	memset((char*)filesystem, 0, sizeof(filesystem));
}

FILE *CreateFile(const char name[], const char data[], int size) {
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

FILE *SearchFile(const char name[]) {
	for(int i = 0; i < DIRECTORY_MAX; i++) {
		if(strcmp(filesystem[i].name, name) == 0 && filesystem[i].available) {
			return filesystem + i;
		}
	}
	return NULL;
}

int DeleteFile(const char name[]) {
	FILE *f = SearchFile(name);
	if(f == NULL) {
		return 1;
	}
	f->available = false;
	f->size = 0;
	return 0;
}

const char *GetFileData(const FILE *file) {
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

int WriteFile(const char name[], const char data[]) {
	FILE *file = SearchFile(name);
	if(strlen(data) > FILE_DATA_MAX_SIZE) return 1;
	strcpy(file->data, data);
	return 0;
}

char read_file_buf[0x100];
const char *ReadFile(const char name[]) {
	FILE *file = SearchFile(name);
	if(file == NULL) return NULL;
	strcpy(read_file_buf, file->data);
	return read_file_buf;
}
