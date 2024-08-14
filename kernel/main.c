#include "../common/types64.h"
#include "../common/frame_info.h"
#include "../common/def.h"
#include "frame.h"
#include "string.h"
#include "asmfunc.h"
#include "keyboard.h"
#include "file.h"

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
	WriteString("Hello yOiSho!!", 0, 0, &white);

	int i = 0;
	char line[0x100];
	memset(line, '\0', 0x100);
	int y = 16;
	while(1) {
		if((IoIn8(0x64) & 0x01) == 1) {
			int code = IoIn8(0x60);
			char ich = keycode[code];
			if(code < 0x80) {
				if(ich == '\b' && i > 0) {
					i--;
					WriteSquare(i*8, y, i*8+7, y+16, &black);
					line[i] = '\0';
				} else if(ich != '\b' && ich != '\n'){
					WriteAscii(ich, i*8, y, &white);
					line[i] = ich;
					i++;
				} else if(ich == '\n') {
					y += 16;
					if(strncmp("echo ", line, 5) == 0) {
						WriteString(line+5, 0, y, &white);
						y+=16;
					} else if(strncmp("ls", line, 3) == 0) {
						WriteString(FileList(), 0, y, &white);
						y+=16;
					} else if(strncmp("touch ", line, 6) == 0) {
						FILE *f = CreateFile(line+6, "", 0);
						if(f == NULL) {
							WriteString("failed to create file.", 0, y, &white);
							y+=16;
						} else {
							strcat(line+6, " created.");
							WriteString(line+6, 0, y, &white);
							y+=16;
						}
					}
					i = 0;
					memset(line, '\0', 0x100);
				}
			}
		}
	}

	hlt();
	return 0;
}
