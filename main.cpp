#include <Windows.h>
#include <stdio.h>
#include <iostream>

void print_buffer(const char *src, size_t len) {
	for (unsigned i = 0; i < len; ++i) {
		SHORT scan = VkKeyScanA(src[i]);
		BYTE mods = HIBYTE(scan);
		bool shift = mods & 1;
		bool alt = mods & 4;
		if (shift) keybd_event(VK_SHIFT, 0, KEYEVENTF_EXTENDEDKEY, 0);
		if (alt) keybd_event(VK_MENU, 0, KEYEVENTF_EXTENDEDKEY, 0);
		keybd_event(LOBYTE(scan), 0, 0, 0);
		keybd_event(LOBYTE(scan), 0, KEYEVENTF_KEYUP, 0);
		if (shift) keybd_event(VK_SHIFT, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
		if (alt) keybd_event(VK_MENU, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
	}
}

void set_status(char *src, size_t len, unsigned i) {
	static const char base [] = "/@ ";
	print_buffer(base, strlen(base));
	size_t width = len - i;
	len = min(len, 14);
	print_buffer(src + i, min(width, len));
	print_buffer(src, max(0, (int) 14 - (int) width));
	keybd_event(VK_RETURN, 0, 0, NULL);
	keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, NULL);
}

int wmain(int argc, wchar_t **argv) {
	char buf[4096];
	size_t len;
	bool caps;
	while (printf("Message to loop over: "), std::cin.getline(buf, 4096), len = strlen(buf)) {
		printf("Parsing \"%s\"...\n", buf);
		for (;;) {
			Sleep(1000);
			for (unsigned i = 0; !(caps = GetKeyState(VK_CAPITAL) & 0x0001) && i < len; ++i) {
				set_status(buf, len, i);
				Sleep(200);
			}
			if (caps) break;
			set_status(buf, len, 0);
		}
	}
	printf("--END OF PROGRAM--\n");
	return 0;
}
