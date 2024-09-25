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

void Print(const char *str) {
	int len = strlen(str);
	WriteSquare(0,0,len*8+7, 15, &black);
	WriteString(str, 0,0,&white);
	Scroll(20);
	WriteSquare(0,0,len*8+7, 15, &black);
}

void command(char *line) {
	char line_buf[TERMINAL_LINE_LEN];
	const TOKEN_LIST *tl = Tokenize(line);
	memset(line_buf, 0, TERMINAL_LINE_LEN);
	if(strcmp(GetToken(tl,0), "echo") == 0) {
		for(int i = 1; i < GetTokenNum(tl); i++) {
			strcat(line_buf, GetToken(tl, i));
			strcat(line_buf, " ");
		}
		strcat (line_buf, "\0");
		Print(line_buf);
	}	else if(strcmp(GetToken(tl,0), "clear") == 0 ) {
		ClearScreen();
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
