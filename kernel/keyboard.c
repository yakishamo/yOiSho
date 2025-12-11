#include "keyboard.h"

char keycode_to_ascii[0x100] = "\000\0001234567890-^\b\0qwertyuiop@[\0\0asdfghjkl;:]\0\0zxcvbnm,./";

void InitializeKeycode () {

	for(int i = 54; i < 0x100; i++) {
		keycode_to_ascii[i] = '\0';
	}
	keycode_to_ascii[14] = '\b';
	keycode_to_ascii[43] = ']';
	keycode_to_ascii[28] = '\n';
	keycode_to_ascii[57] = ' ';
	keycode_to_ascii[115] = '\\';
	keycode_to_ascii[125] = '\\';
	return;
}

char TransrateKeycode(KeyCode code, int is_shift) {
	if(is_shift) {
		if(keycode_to_ascii[code] >= 'a' && keycode_to_ascii[code] <= 'z' ) {
			return keycode_to_ascii[code] + ('A' - 'a');
		}
	}
	return keycode_to_ascii[code];
}
