#pragma once
#include "../main.h"


// you could add https://github.com/ekknod/logitech-cve instead of this
	
namespace input
{
	static BYTE ntusersendinput_bytes[30];
	HMODULE user32 = 0;
	HMODULE win32u = 0;

	using NtUserSendInput_t = NTSTATUS(NTAPI*)(UINT, LPINPUT, int);
	inline NtUserSendInput_t real_ntusersendinput = nullptr;


	static BOOLEAN WINAPI init() {
		FARPROC addr = GetProcAddress(LoadLibraryA("win32u.dll"), "NtUserSendInput");
		if (!addr) return FALSE;
		real_ntusersendinput = reinterpret_cast<NtUserSendInput_t>(addr);
		return TRUE;
	}

	static BOOLEAN WINAPI ntusersendinput(UINT cinputs, LPINPUT pinputs, int cbsize) {
		if (!real_ntusersendinput) return FALSE;
		NTSTATUS result = real_ntusersendinput(cinputs, pinputs, cbsize);
		return (result > 0);
	}

	static BOOLEAN WINAPI move_mouse(int x, int y) {
		INPUT input = { 0 };
		input.type = INPUT_MOUSE;
		input.mi.dx = x;
		input.mi.dy = y;
		input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_VIRTUALDESK;
		return ntusersendinput(1, &input, sizeof(input));
	}
}