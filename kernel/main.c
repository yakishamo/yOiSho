#include "../common/types64.h"
#include "../common/frame_info.h"
#include "../common/def.h"
#include "frame.h"
#include "string.h"
#include "asmfunc.h"
#include "keyboard.h"
#include "file.h"
#include "cursor.h"

extern FrameInfo *frame_info;
extern char keycode[];

void hlt() {
	while(1) asm("hlt");
}

__attribute__((ms_abi))
int KernelMain(FrameInfo *fi){
	frame_info = fi;
	InitializeKeycode();

	ClearScreen();
	Color white = {255,255,255,0};
	Color black = {0,0,0,0};

	CURSOR *cur;
	cur = InitializeCursor(&white);
	MoveCursor(cur, 0, 16);
	PrintCursor(cur);
	int i = 0;
	char line[0x100];
	memset(line, '\0', 0x100);
	int y = 0;
	while(1) {
		if((IoIn8(0x64) & 0x01) == 1) {
			int code = IoIn8(0x60);
			char ich = keycode[code];
			if(code < 0x80) {
				if(ich == '\b' && i > 0) {
					i--;
					CursorBack(cur);
					WriteSquare(i*8, y, i*8+7, y+15, &black);
					line[i] = '\0';
				} else if(ich != '\b' && ich != '\n'){
					WriteAscii(ich, i*8, y, &white);
					CursorNext(cur);
					line[i] = ich;
					i++;
				} else if(ich == '\n') {
					EraseCursor(cur);
					Scroll(20);
					WriteSquare(0,y,i*8,y+16, &black);
					if(strncmp(line, "echo ", 5) == 0) {
						WriteString(line+5, 0, 0, &white);
						Scroll(20);
						WriteSquare(0,y,i*8,y+16, &black);
					}
					MoveCursor(cur, 0, 16);
					PrintCursor(cur);
					i = 0;
					memset(line, '\0', 0x100);
				}
			}
		}
	}

	hlt();
	return 0;
}
