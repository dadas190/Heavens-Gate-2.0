/*
Made by David Cernak - Dadas1337
Donate some Bitcoin : 1LaLSsqSU1woJ72k9FByNjUF7dLzS6u443
*/

#define uint64_t unsigned long long int
#define uint32_t unsigned
#define uint16_t unsigned short

void xmemcpy(void *dst, void *src, size_t sz) {
	for (size_t i = 0; i < sz; i++)((char*)dst)[i] = ((char*)src)[i];
}

void memcpy64(uint64_t dst, uint64_t src, uint64_t sz) {
	char inst[] = {
		/*32bit:
		push 0x33
		push _next_64bit_block
		retf*/
		0x6A, 0x33, 0x68, 0x44, 0x33, 0x22, 0x11, 0xCB,
		/*64bit:
		push rsi
		push rdi

		mov rsi,src
		mov rdi,dst
		mov rcx,sz
		rep movsb

		pop rdi
		pop rsi

		push 0x23
		push _next_32bit_block
		retfq*/
		0x56, 0x57, 0x48, 0xBE, 0x88, 0x77, 0x66, 0x55,
		0x44, 0x33, 0x22, 0x11, 0x48, 0xBF, 0x88, 0x77,
		0x66, 0x55, 0x44, 0x33, 0x22, 0x11, 0x48, 0xB9,
		0x88, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11,
		0xF3, 0xA4, 0x5F, 0x5E, 0x6A, 0x23, 0x68, 0x44,
		0x33, 0x22, 0x11, 0x48, 0xCB,
		/*32bit:
		ret*/
		0xC3
	};
	static char *r = NULL;
	if (!r) {
		r = (char*)VirtualAlloc(0, sizeof(inst), 0x3000, 0x40);
		xmemcpy(r, inst, sizeof(inst));
	}

	*(uint32_t*)(r + 3) = (uint32_t)(r + 8);
	*(uint64_t*)(r + 12) = (uint64_t)(src);
	*(uint64_t*)(r + 22) = (uint64_t)(dst);
	*(uint64_t*)(r + 32) = (uint64_t)(sz);
	*(uint32_t*)(r + 47) = (uint32_t)(r + 53);

	((void(*)(void))(r))();
}

void GetPEB64(void *peb) {
	char inst[] = {
		/*32bit:
		mov esi,peb
		push 0x33
		push _next_64bit_block
		retf*/
		0xBE, 0x44, 0x33, 0x22, 0x11, 0x6A, 0x33, 0x68,
		0x44, 0x33, 0x22, 0x11, 0xCB,
		/*64bit:
		mov rax,GS:[0x60]
		mov [esi],rax
		push 0x23
		push _next_32bit_block
		retfq*/
		0x65, 0x48, 0x8B, 0x04, 0x25, 0x60, 0x00, 0x00,
		0x00, 0x67, 0x48, 0x89, 0x06, 0x6A, 0x23, 0x68,
		0x44, 0x33, 0x22, 0x11, 0x48, 0xCB,
		/*32bit:
		ret*/
		0xC3
	};

	static char *r = NULL;
	if (!r) {
		r = (char*)VirtualAlloc(0, sizeof(inst), 0x3000, 0x40);
		xmemcpy(r, inst, sizeof(inst));
	}

	*(uint32_t*)(r + 1) = (uint32_t)(peb);
	*(uint32_t*)(r + 8) = (uint32_t)(r + 13);
	*(uint32_t*)(r + 29) = (uint32_t)(r + 35);

	((void(*)(void))(r))();
}

uint64_t GetModuleHandle64(wchar_t *name) {
	uint64_t ptr;
	GetPEB64(&ptr);
	memcpy64((uint64_t)&ptr, ptr + 24, 8);//PTR -> PPEB_LDR_DATA LoaderData;
	memcpy64((uint64_t)&ptr, ptr + 16, 8);//PTR -> LIST_ENTRY64 InLoadOrderModuleList.FirstBlink

	while (1) {
		uint64_t tmp;
		memcpy64((uint64_t)&tmp, ptr + 96, 8); //TMP -> UNICODE_STRING Basename -> Buffer
		if (!tmp)break;
		wchar_t kek[32];
		memcpy64((uint64_t)kek, tmp, 60); //KEK = Basename

		memcpy64((uint64_t)&tmp, ptr + 48, 8); //TMP = Module Base Address

		if (!lstrcmpW(name, kek))return tmp;
		memcpy64((uint64_t)&ptr, ptr, 8); //PTR -> Flink
	}
	return 0;
}

uint64_t X64Call(uint64_t proc, uint64_t a, uint64_t b, uint64_t c, uint64_t d) {
	uint64_t ret;
	char inst[] = {
		/*32bit:
		push 0x33
		push _next_64bit_block
		retf*/
		0x6A, 0x33, 0x68, 0x44, 0x33, 0x22, 0x11, 0xCB,
		/*64bit:
		push rbx
		mov rbx,rsp
		and rbx,0xf
		sub rsp,rbx

		mov rcx,a
		mov rdx,b
		mov r8,c
		mov r9,d
		mov rax,proc
		
		sub rsp,32
		call rax
		add rsp,32

		add rsp,rbx
		pop rbx

		push rsi
		mov rsi,&ret
		mov [rsi],rax
		pop rsi

		push 0x23
		push _next_32bit_block
		retfq
		*/
		0x53, 0x48, 0x89, 0xE3, 0x48, 0x83, 0xE3, 0x0F,
		0x48, 0x29, 0xDC, 0x48, 0xB9, 0x88, 0x77, 0x66,
		0x55, 0x44, 0x33, 0x22, 0x11, 0x48, 0xBA, 0x88,
		0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11, 0x49,
		0xB8, 0x88, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22,
		0x11, 0x49, 0xB9, 0x88, 0x77, 0x66, 0x55, 0x44,
		0x33, 0x22, 0x11, 0x48, 0xB8, 0x88, 0x77, 0x66,
		0x55, 0x44, 0x33, 0x22, 0x11, 0x48, 0x83, 0xEC,
		0x20, 0xFF, 0xD0, 0x48, 0x83, 0xC4, 0x20, 0x48,
		0x01, 0xDC, 0x5B, 0x56, 0x48, 0xBE, 0x88, 0x77,
		0x66, 0x55, 0x44, 0x33, 0x22, 0x11, 0x48, 0x89,
		0x06, 0x5E, 0x6A, 0x23, 0x68, 0x44, 0x33, 0x22,
		0x11, 0x48, 0xCB,
		/*32bit:
		ret*/
		0xC3
	};

	static char *r = NULL;
	if (!r) {
		r = (char*)VirtualAlloc(0, sizeof(inst), 0x3000, 0x40);
		xmemcpy(r, inst, sizeof(inst));
	}

	*(uint32_t*)(r + 3) = (uint32_t)(r + 8);
	*(uint64_t*)(r + 21) = (uint64_t)(a);
	*(uint64_t*)(r + 31) = (uint64_t)(b);
	*(uint64_t*)(r + 41) = (uint64_t)(c);
	*(uint64_t*)(r + 51) = (uint64_t)(d);
	*(uint64_t*)(r + 61) = (uint64_t)(proc);
	*(uint64_t*)(r + 86) = (uint64_t)(&ret);
	*(uint32_t*)(r + 101) = (uint32_t)(r + 107);

	((void(*)(void))(r))();
	return ret;
}

uint64_t MakeANSIStr(char *in) {
	uint32_t len = lstrlenA(in);

	char *out = (char*)VirtualAlloc(0, 17 + len, 0x3000, 0x40);

	*(uint16_t*)(out) = (uint16_t)(len); //Length
	*(uint16_t*)(out + 2) = (uint16_t)(len + 1); //Max Length

	lstrcpyA(out+16, in);
	*(uint64_t*)(out + 8) = (uint64_t)(out+16);
	return (uint64_t)out;
}

uint64_t GetProcAddress64(uint64_t module, uint64_t func) {
	static uint64_t LdrGetProcedureAddress = 0;
	if (!LdrGetProcedureAddress) {
		uint64_t ntdll = GetModuleHandle64(L"ntdll.dll");
		IMAGE_DOS_HEADER dos;
		memcpy64((uint64_t)&dos, ntdll, sizeof(dos));

		IMAGE_NT_HEADERS64 nt;
		memcpy64((uint64_t)&nt, ntdll + dos.e_lfanew, sizeof(nt));

		IMAGE_EXPORT_DIRECTORY exp;
		memcpy64((uint64_t)&exp, ntdll + nt.OptionalHeader.DataDirectory[0].VirtualAddress, sizeof(exp));

		for (DWORD i = 0; i < exp.NumberOfNames; i++) {
			DWORD nameptr;
			memcpy64((uint64_t)&nameptr, ntdll + exp.AddressOfNames + (4 * i), 4);
			char name[64];
			memcpy64((uint64_t)name, ntdll + nameptr, 64);
			if (!lstrcmpA(name, "LdrGetProcedureAddress")) {
				WORD ord;
				memcpy64((uint64_t)&ord, ntdll + exp.AddressOfNameOrdinals + (2 * i), 2);
				uint32_t adr;
				memcpy64((uint64_t)&adr, ntdll + exp.AddressOfFunctions + (4 * ord), 4);
				LdrGetProcedureAddress = ntdll + adr;
				return GetProcAddress64(module, func);
			}
		}
	}
	uint64_t ret;
	if ((func & 0xffff)==func)X64Call(LdrGetProcedureAddress, module, 0, func, (uint64_t)&ret);
	else {
		uint64_t ansi = MakeANSIStr((char*)func);
		X64Call(LdrGetProcedureAddress, module, ansi, 0, (uint64_t)&ret);

		VirtualFree((void*)ansi, 0, MEM_RELEASE);
	}
	return ret;
}

uint64_t MakeUTFStr(char *in) {
	uint32_t len = lstrlenA(in);

	char *out = (char*)VirtualAlloc(0, 18 + ((len+1)*2), 0x3000, 0x40);

	*(uint16_t*)(out) = (uint16_t)(len*2); //Length
	*(uint16_t*)(out + 2) = (uint16_t)((len+1)*2); //Max Length

	WORD *outstr = (WORD*)(out + 16);
	for (uint32_t i = 0; i <= len; i++)outstr[i] = in[i];
	*(uint64_t*)(out + 8) = (uint64_t)(out + 16);
	return (uint64_t)out;
}

uint64_t LoadLibrary64(char *name) {
	static uint64_t LdrLoadDll = 0;
	if (!LdrLoadDll)LdrLoadDll = GetProcAddress64(GetModuleHandle64(L"ntdll.dll"), (uint64_t)"LdrLoadDll");

	uint64_t handle;
	uint64_t unicode = MakeUTFStr(name);
	X64Call(LdrLoadDll, 0, 0, unicode, (uint64_t)&handle);

	VirtualFree((void*)unicode, 0, MEM_RELEASE);

	return handle;
}