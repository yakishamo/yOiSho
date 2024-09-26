#include "cursor.h"
#include "string.h"
#include "asmfunc.h"
#include "frame.h"
#include "file.h"
#include "keyboard.h"

#include "editor.h"

extern const Color black;
extern const Color white;

int editor(const char *name, unsigned int x, unsigned int y) {
	CURSOR cur_;
	CURSOR *cur = &cur_;
	char line[0x100] = "";
	int i = 0;
	cur = InitializeCursor(cur, &white);
	MoveCursor(cur, 0, 0);
	PrintCursor(cur);
	FILE *file = SearchFile(name);
	if(file == NULL) {
		if(name == NULL || strcmp(name,"") == 0) return -1;
		file = CreateFile(name, "", 0);
		if(file == NULL) {
			return 1;
		}
	} else {
		strcpy(line, ReadFile(name));
		WriteString(line, 0,0,&white);
		i = strlen(line);
		for(int j = 0; j < i; j++) CursorNext(cur);
	}
	int is_shift = 0;
	while(1) {
		if((IoIn8(0x64) & 0x01) == 1) {
			unsigned char code  = IoIn8(0x60);
			if(code == 0x2A || code == 0x36) {
				is_shift = 1;
				continue;
			} else if (code == 0xAA || code == 0xB6) {
				is_shift = 0;
				continue;
			}
			char ich =TransrateKeycode(code, is_shift);
			if(code < 0x80 && ich != '\0') {
				if(ich == '\n') {
					EraseCursor(cur);
					int ret = WriteFile(name, line);
					if(ret != 0) {
						return 2;
					} else {
						return 0;
					}
				} else if(ich == '\b') {
					if(i != 0) {
						if(line[--i] != '\0') {
							int len = strlen(line+i+1);
							memmove(line+i, line+i+1, strlen(line+i+1)+1);
							CursorBack(cur);
							WriteSquare(cur->x, cur->y, cur->x+len*8+7, cur->y+15, &black);
							WriteString(line+i, cur->x, cur->y, &white);
						}
					}
				} else {
					int len = strlen(line+i);
					memmove(line+i+1, line+i, len+1);
					line[i++] = ich;
					WriteSquare(cur->x, cur->y, cur->x+len*8+7, cur->y+15, &black);
					WriteString(line+i-1, cur->x, cur->y, &white);
					CursorNext(cur);
				}
			} else if (code == 0xE0) {
				code = IoIn8(0x60);
				if(code == 0x4b && i != 0) {
					CursorBack(cur);
					i--;
				} else if (code == 0x4d && (i == 0x100 || line[i] != '\0')) {
						CursorNext(cur);
						i++;
				}
			}
		}
	}
}
