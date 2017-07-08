#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <Windows.h>

#include "HeavensGate.h"

int main()
{	
	uint64_t user32 = LoadLibrary64("user32.dll");
	uint64_t MessageBoxA = GetProcAddress64(user32, (uint64_t)"MessageBoxA");
	X64Call(MessageBoxA, 0, (uint64_t)"testing", (uint64_t)"msg", 0);

	uint64_t kernel32 = GetModuleHandle64(L"kernel32.dll");
	uint64_t TerminateProcess = GetProcAddress64(kernel32, (uint64_t)"TerminateProcess");
	X64Call(TerminateProcess, -1, 0x69, 0, 0);
	return 0;
}