#include <stdint.h>
#include <Windows.h>

#include "HeavensGate.h"
#include "Loader.h"

int main()
{	
	HANDLE f = CreateFile("payload-sockets.exe", GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
	size_t sz=SetFilePointer(f, 0, 0, FILE_END);
	SetFilePointer(f, 0, 0, FILE_BEGIN);

	char *buf = (char*)HeapAlloc(GetProcessHeap(), 0, sz);
	ReadFile(f, buf, sz, 0, 0);
	CloseHandle(f);

	uint64_t entry;
	char *base = load64(buf, &entry);

	X64Call(entry, 0);
	return 0;
}