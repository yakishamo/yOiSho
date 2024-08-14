#ifndef FILE_H
#define FILE_H

#define FILE_NAME_MAX_SIZE 0x100
#define FILE_DATA_MAX_SIZE 0x100

typedef struct _FILE FILE;

void InitializeFilesytem();
FILE *CreateFile(char name[], char data[], int size);
FILE *SearchFile(char name[]);
void DeleteFile(char name[]);
char *GetFileData(FILE *file);
char *FileList();

#endif /* FILE_H */
