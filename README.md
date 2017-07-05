#Heaven's Gate 2.0

Unable to find any properly implemented/working code that works on Windows 10, I have decided to open the Heaven's Gate on my own.<br>
Yes, this one is working on an updated Windows 10 (as of 5th July 2017)<br>
Now donate some Bitcoin: <b>1LaLSsqSU1woJ72k9FByNjUF7dLzS6u443</b><br><br>

HeavensGate.h includes the following functions:

uint64_t <b>GetModuleHandle64(wchar_t *name)</b><br>
Retrieves the HANDLE (base address) of a loaded 64bit module<br><br>
uint64_t <b>LoadLibrary64(char *name)</b><br>
Loads a 64bit DLL using LdrLoadDll, returns its HANDLE (base address)<br><br>
uint64_t <b>GetProcAddress64(uint64_t dll, char *func)</b><br>
Retrieves the address of a function from a 64bit module (ordinals should work too, but untested)<br><br>
uint64_t <b>X64Call(uint64_t proc, uint64_t a, uint64_t b, uint64_t c, uint64_t d)</b><br>
Calls the specified 64bit function "proc" with arguments a,b,c,d. Only 4 arguments are supported because I don't need more. When calling functions that take less arguments, simply use 0 for the rest.<br><br><br>

Another functions:<br>
void <b>memcpy64(uint64_t dst, uint64_t src, uint64_t sz)</b><br>
Like normal memcpy, but supports 64bit addresses<br><br>
void <b>GetPEB64(void *peb)</b><br>
Retrieves pointer to the 64bit PEB (Process Environment Block)<br><br>
uint64_t <b>MakeUTFStr(char *in)</b><br>
Converts input string to UNICODE_STRING64 needed for the LdrLoadDll call.<br><br><br>

Everything is implemented in the HeavensGate.h, Source.cpp contains just some example code.