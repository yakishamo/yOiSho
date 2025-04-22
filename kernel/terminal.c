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

// print string to terminal
void Print(const char *str) {
	int len = strlen(str);
	WriteSquare(0,frame_info->vertical_resolution-21,
    frame_info->horizontal_resolution-1, frame_info->vertical_resolution-1, &black);
	WriteString(str, 0, frame_info->vertical_resolution-21, &white);
	ScrollUp(20);
  WriteSquare(0,frame_info->vertical_resolution-21,
    frame_info->horizontal_resolution-1, frame_info->vertical_resolution-1, &black);
}

void Print_int(const char *val_name, uint64_t a, unsigned int radix) {
	char str[25] = "";
	char *p = str;
	unsigned int v = a;
	int n = 1;
	while(v >= radix) {
		v/=radix;
		n++;
	}
	p = str + n;
	v = a;
	*p = 0;
	do {
		p--;
		*p = v % radix + (char)'0';
		if(*p > (char)'9') {
			*p = v % radix - 10 + 'A';
		}
		v /= radix;
	} while(p != str);
	char print_str[200] = "";
	strcat(print_str, val_name);
	strcat(print_str, str);
	Print(print_str);
}


void clear(TOKEN_LIST *tl) {
	ClearScreen();
}

void ls(TOKEN_LIST *tl) {
	Print(FileList());
}

void touch(TOKEN_LIST *tl) {
	FILE *f = NULL;
  char *file_name = GetToken(tl, 1);
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

void rm(TOKEN_LIST *tl) {
  char *file_name = GetToken(tl, 1);
  if(file_name == NULL) {
    Print("\"rm\" need 1 arg");
    return;
  }
	if(DeleteFile(file_name) == 0) {
		Print("successfully deleted");
	} else {
		Print("failed to delete");
	}
}

void echo(TOKEN_LIST *tl) {
	char line_buf[TERMINAL_LINE_LEN];
	memset(line_buf, 0, TERMINAL_LINE_LEN);
  int token_num = GetTokenNum(tl);
	for(int i = 1; i < token_num; i++) {
		strcat(line_buf, GetToken(tl, i));
		strcat(line_buf, " ");
	}
	Print(line_buf);
}

void cat(TOKEN_LIST *tl) {
  char *file_name = GetToken(tl, 1);
  if(file_name == NULL) {
    return;
  }
	const char *str = ReadFile(file_name);
	if(str == NULL) {
		Print("ReadFile failed");
	} else {
		Print(str);
	}
}

static void cpuid(TOKEN_LIST *tl) {
  char buf[13];
  CpuidGetVendor(buf);
  buf[12] = '\0';
  Print(buf);
}

void command(char *line) {
	TOKEN_LIST *tl = Tokenize(line);
  DumpTokenList(tl);
	char *first_token = GetToken(tl, 0);
  if(first_token == NULL) {
    FreeTokenList(tl);
    return;
  }

	if(strcmp(first_token, "echo") == 0) {
		echo(tl);
	}	else if(strcmp(first_token, "clear") == 0 ) {
		clear(tl);
	} else if(strcmp(first_token, "ls") == 0 ) {
		ls(tl);
	} else if(strcmp(first_token, "touch") == 0) {
		touch(tl);
	} else if(strcmp(first_token, "rm") == 0) {
		rm(tl);
	} else if(strcmp(first_token, "cat") == 0) {
		cat(tl);
	} else if(strcmp(first_token, "edit") == 0) {
		editor(GetToken(tl, 1), 0, 0);
	} else if(strcmp(first_token, "cpuid") == 0) {
    cpuid(tl);
  }
  FreeTokenList(tl);
}

// fontsize x:8, y:20
#define FONTSIZE_X 8
#define FONTSIZE_Y 20
void terminal() {
	char line[TERMINAL_LINE_LEN];
	int x = 0, y = 0; // charcter coordinate, not frame coordinate
//  const int x_max = frame_info->vertical_resolution / FONTSIZE_X - 1;
//  const int y_max = frame_info->horizontal_resolution / FONTSIZE_Y - 1;
	int i = 0; // line iter
	int is_shift = 0;
	CURSOR cur_;
	CURSOR *cur = &cur_;

	cur = InitializeCursor(cur,&white);
	MoveCursor(cur, 0, 0);
	PrintCursor(cur);

	memset(line, '\0', TERMINAL_LINE_LEN);
	WriteString("> ", 0, 0, &white);
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
					ScrollDown(20);
					WriteSquare(0,0,
            frame_info->horizontal_resolution*4, 15, &black);
					command(line);
					memset(line, '\0', TERMINAL_LINE_LEN);
					WriteSquare(0,0,
            frame_info->horizontal_resolution*4, 15, &black);
					WriteString("> ", 0, 0, &white);
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

void terminal_v2() {
  Print("terminal_v2");
  char line[TERMINAL_LINE_LEN];
  int x=0, y=0; //character coordinate, not frame coordinate
  const int x_max = (frame_info->horizontal_resolution / FONTSIZE_X) - 1;
  const int y_max = (frame_info->vertical_resolution / FONTSIZE_Y) - 1;
  const int x_max_frame = x_max * FONTSIZE_X;
  const int y_max_frame = y_max * FONTSIZE_Y;
  int is_shift = 0;
  int i = 0; //line iter
  CURSOR cur_;
  CURSOR *cur = &cur_;

  cur = InitializeCursor(cur,&white);
  MoveCursor(cur, 16, y_max_frame);
  PrintCursor(cur);

  memset(line, '\0', TERMINAL_LINE_LEN);
  WriteString("> ", 0, y_max_frame, &white);
  PrintCursor(cur);
  x = 2;
  y = y_max;

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
        if(ascii == '\b') {
          if(i > 0) {
            i--;
            x--;
            WriteSquare(x * FONTSIZE_X, y * FONTSIZE_Y,
                x*FONTSIZE_X + 7, y*FONTSIZE_Y + 15, &black);
            line[i] = '\0';
            CursorBack(cur);
          }
        } else if(ascii == '\n') {
          EraseCursor(cur);
          ScrollUp(20);
          WriteSquare(0,y_max*FONTSIZE_Y,
            x_max * FONTSIZE_X, (y_max+1)*FONTSIZE_Y,
            &black);
          command(line);
          memset(line,'\0', TERMINAL_LINE_LEN);
          WriteString("> ", 0, y_max * FONTSIZE_Y, &white);
          x = 2;
          y = y_max;
          i = 0;
          MoveCursor(cur, x*FONTSIZE_X, y*FONTSIZE_Y);
          PrintCursor(cur);
        } else if(i < TERMINAL_LINE_LEN - 1) {
          CursorNext(cur);
          line[i] = ascii;
          WriteAscii(ascii, x*FONTSIZE_X, y*FONTSIZE_Y, &white);
          x++;
          i++;
        }
      }
    }
  }
}
