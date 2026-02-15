#ifndef FILE_H
#define FILE_H

#define FILE_NAME_MAX_SIZE 0x100
#define FILE_DATA_MAX_SIZE 0x100

typedef struct _FILE *FILE;

void initFilesystem(void *fat_image);
void FileList();
int WriteFile(FILE file, char *data);
int ReadFile(FILE file, char *data);

#endif /* FILE_H */
