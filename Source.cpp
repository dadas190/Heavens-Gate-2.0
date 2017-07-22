#include <stdint.h>
#include <Windows.h>

#include "HeavensGate.h"

int main()
{	
	uint64_t user32 = LoadLibrary64("user32.dll");
	uint64_t MessageBoxA = GetProcAddress64(user32, (uint64_t)"MessageBoxA");
	X64Call(MessageBoxA, 4, (uint64_t)0, (uint64_t)(unsigned)"testing", (uint64_t)(unsigned)"msg", 0);

	uint64_t kernel32 = GetModuleHandle64(L"kernel32.dll");
	uint64_t TerminateProcess = GetProcAddress64(kernel32, (uint64_t)"TerminateProcess");
	X64Call(TerminateProcess, 2, (uint64_t)-1, (uint64_t)0x69);
	return 0;
}