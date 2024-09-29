#include "../common/def.h"
#include "../common/frame_info.h"
#include "keyboard.h"
#include "file.h"
#include "editor.h"
#include "parse.h"
#include "asmfunc.h"
#include "string.h"
#include "frame.h"
#include "cursor.h"

#define TERMINAL_LINE_LEN 0x100

extern FrameInfo *frame_info;


void clear() {
	ClearScreen();
}

void ls() {
	Print(FileList());
}

void touch(const char *file_name) {
	FILE *f = NULL;
	if(file_name == NULL) {
		Print("filename is not specified");
		return;
	}
	f = CreateFile(file_name, "", 0);
	if(f == NULL) {
		Print("failed to create");
		return;
	}
	Print("created");
}

void rm(const char *file_name) {
	if(DeleteFile(file_name) == 0) {
		Print("successfully deleted");
	} else {
		Print("failed to delete");
	}
}

void echo(const TOKEN_LIST *tl) {
	char line_buf[TERMINAL_LINE_LEN];
	memset(line_buf, 0, TERMINAL_LINE_LEN);
	for(int i = 1; i < GetTokenNum(tl); i++) {
		strcat(line_buf, GetToken(tl, i));
		strcat(line_buf, " ");
	}
	Print(line_buf);
}

void cat(const char* file_name) {
	const char *str = ReadFile(file_name);
	if(str == NULL) {
		Print("ReadFile failed");
	} else {
		Print(str);
	}
}

void command(char *line) {
	const TOKEN_LIST *tl = Tokenize(line);
	const char *first_token = GetToken(tl, 0);
	if(strcmp(first_token, "echo") == 0) {
		echo(tl);
	}	else if(strcmp(first_token, "clear") == 0 ) {
		clear();
	} else if(strcmp(first_token, "ls") == 0 ) {
		ls();
	} else if(strcmp(first_token, "touch") == 0) {
		touch(GetToken(tl, 1));
	} else if(strcmp(first_token, "rm") == 0) {
		rm(GetToken(tl,1));
	} else if(strcmp(first_token, "cat") == 0) {
		cat(GetToken(tl, 1));
	} else if(strcmp(first_token, "edit") == 0) {
		editor(GetToken(tl, 1), 0, 0);
	}
}

void terminal() {
	char line[TERMINAL_LINE_LEN];
	int x = 0, y = 0;
	int i = 0;
	int is_shift = 0;
	CURSOR cur_;
	CURSOR *cur = &cur_;

	cur = InitializeCursor(cur,&white);
	MoveCursor(cur, 0, 0);
	PrintCursor(cur);

	memset(line, '\0', TERMINAL_LINE_LEN);
	WriteString("> ", x, y, &white);
	x+=2;
	CursorNext(cur);
	CursorNext(cur);
	while(1) {
		if((IoIn8(0x64) & 0x01) == 1) {
			unsigned char code = IoIn8(0x60);
			if(code == 0x2A || code == 0x36) {
				is_shift = 1;
				continue;
			} else if(code == 0xAA || code == 0xB6) {
				is_shift = 0;
				continue;
			}
			char ascii = TransrateKeycode(code, is_shift);
			if(code < 0x80 && ascii != '\0') {
				if(ascii == '\b' && x > 2) {
					i--;
					x--;
					WriteSquare(x*8, y*16, x*8+7, y*16+15, &black);
					line[i] = '\0';
					CursorBack(cur);
				} else if(ascii != '\b' && ascii != '\n') {
					CursorNext(cur);
					line[i] = ascii;
					WriteAscii(ascii, x*8, y*16, &white);
					x++;
					i++;
				} else if(ascii == '\n') {
					EraseCursor(cur);
					Scroll(20);
					WriteSquare(0,0,frame_info->horizontal_resolution*4, 15, &black);
					command(line);
					memset(line, '\0', TERMINAL_LINE_LEN);
					WriteSquare(0,0,frame_info->horizontal_resolution*4, 15, &black);
					WriteString("> ", x=0, y=0, &white);
					x = 2;
					y = 0;
					i = 0;
					MoveCursor(cur, x*8, y*16);
					PrintCursor(cur);
				}
			}
		}
	}
}
