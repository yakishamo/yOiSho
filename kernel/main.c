#include "../common/types64.h"
#include "../common/frame_info.h"
#include "../common/def.h"
#include "frame.h"
#include "string.h"
#include "asmfunc.h"
#include "keyboard.h"
#include "file.h"
#include "cursor.h"
#include "parse.h"
#include "editor.h"

extern FrameInfo *frame_info;
extern char keycode[];

void hlt() {
	while(1) asm("hlt");
}

const Color white = {255,255,255,0};
const Color black = {0,0,0,0};

void Print(const char *str) {
	int len = strlen(str);
	WriteSquare(0,0,len*8+7, 15, &black);
	WriteString(str, 0,0,&white);
	Scroll(20);
	WriteSquare(0,0,len*8+7, 15, &black);
}

__attribute__((ms_abi))
int KernelMain(FrameInfo *fi){

	frame_info = fi;
	InitializeKeycode();

	ClearScreen();

	CURSOR cur_;
	CURSOR *cur = &cur_;
	cur = InitializeCursor(cur, &white);
	MoveCursor(cur, 0, 0);
	PrintCursor(cur);
	int i = 0;
	char line[0x100];
	memset(line, '\0', 0x100);
	int x = 0;
	int y = 0;
	WriteString("> ", x, y, &white);
	x+=16;
	CursorNext(cur);
	CursorNext(cur);
	while(1) {
		if((IoIn8(0x64) & 0x01) == 1) {
			int code = IoIn8(0x60);
			char ich = keycode[code];
			if(code < 0x80 && ich != '\0') {
				if(ich == '\b' && i > 0) {
					i--;
					CursorBack(cur);
					x-=8;
					WriteSquare(x, y, x+7 , y+15, &black);
					line[i] = '\0';
				} else if(ich != '\b' && ich != '\n'){
					WriteAscii(ich, x, y, &white);
					x+=8;
					CursorNext(cur);
					line[i] = ich;
					i++;
				} else if(ich == '\n') {
					EraseCursor(cur);
					Scroll(20);
					WriteSquare(0,y,x+7,y+15, &black);
					const TOKEN_LIST *tl = Tokenize(line);
					if(strcmp(GetToken(tl, 0), "echo") == 0) {
						memset(line, '\0', strlen(line));
						for(int i = 1; i < GetTokenNum(tl); i++) {
							strcat(line, GetToken(tl, i));
							strcat(line, " ");
						}
						Print(line);
					} else if(strcmp(GetToken(tl, 0), "ls") == 0) {
						strcpy(line, FileList());
						Print(line);
					} else if(strcmp(GetToken(tl, 0), "touch") == 0) {
						FILE *f = CreateFile(GetToken(tl,1), "", 0);
						if(f == NULL) {
							memset(line, '\0', strlen(line));
							strcat(line, "failed to create ");
							strcat(line, GetToken(tl, 1));
							Print(line);
						} else {
							strcat(line, " created.");
							Print(line+6);
						}
					} else if(strcmp(GetToken(tl, 0), "rm") == 0) {
						if(DeleteFile(line+3) == 0) {
							Print("delete successfully.");
						} else {
							Print("failed to delete.");
						}
					}	else if(strcmp(GetToken(tl, 0), "write") == 0) {
						memset(line, '\0', strlen(line));
						int ret = WriteFile(GetToken(tl, 1), GetToken(tl, 2));
						if(ret == 0) {
							Print("write successfully.");
						} else {
							Print("failed to write.");
						}
					} else if(strcmp(GetToken(tl, 0), "cat") == 0) {
						memset(line, '\0', strlen(line));
						const char *str = ReadFile(GetToken(tl, 1));
						if(str == NULL) Print("ReadFile() failed.");
						else Print(str);
					} else if(strcmp(GetToken(tl, 0), "edit") == 0) {
						editor(GetToken(tl, 1), 0, 0);
						Scroll(20);
						WriteSquare(0,0,frame_info->horizontal_resolution*4,15,&black);
					} else if(strcmp(GetToken(tl, 0), "clear") == 0) {
						ClearScreen();
					}
					MoveCursor(cur, 0, 0);
					PrintCursor(cur);
					i = 0;
					x = 0;
					WriteString("> ", x, y, &white);
					x+=16;
					CursorNext(cur);
					CursorNext(cur);
					memset(line, '\0', 0x100);
				}
			}
		}
	}

	hlt();
	return 0;
}
