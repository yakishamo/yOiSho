#ifndef FILE_H
#define FILE_H

#define FILE_NAME_MAX_SIZE 0x100
#define FILE_DATA_MAX_SIZE 0x100

typedef struct _FILE FILE;

void InitializeFilesytem();
FILE *CreateFile(const char name[], const char data[], int size);
FILE *SearchFile(const char name[]);
int DeleteFile(const char name[]);
const char *GetFileData(const FILE *file);
char *FileList();
int WriteFile(const char name[], const char data[]);
const char *ReadFile(const char name[]);

#endif /* FILE_H */
