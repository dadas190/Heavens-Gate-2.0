char *load64(char *adr, uint64_t *entry) {
	IMAGE_DOS_HEADER *dos = (IMAGE_DOS_HEADER*)adr;
	IMAGE_NT_HEADERS64 *nt = (IMAGE_NT_HEADERS64*)(adr + dos->e_lfanew);

	char *base = (char*)VirtualAlloc(0, nt->OptionalHeader.SizeOfImage + 65535, 0x3000, 0x40);
	memcpy64(base, adr, ((uint64_t)(nt + 1) + (nt->FileHeader.NumberOfSections * sizeof(IMAGE_SECTION_HEADER))) - (uint64_t)adr);

	IMAGE_SECTION_HEADER *sect = (IMAGE_SECTION_HEADER*)(nt + 1);
	for (uint32_t i = 0; i < nt->FileHeader.NumberOfSections; i++, sect++)memcpy64(base + sect->VirtualAddress, adr + sect->PointerToRawData, sect->SizeOfRawData);

	IMAGE_IMPORT_DESCRIPTOR *imp = (IMAGE_IMPORT_DESCRIPTOR*)(base + nt->OptionalHeader.DataDirectory[1].VirtualAddress);

	while (imp->FirstThunk) {
		uint64_t handle = LoadLibrary64(base + imp->Name);
		uint64_t *thunk = (uint64_t*)(base + imp->FirstThunk);

		while (*thunk) {
			if ((*thunk) & 0x8000000000000000)(*thunk) = GetProcAddress64(handle, ((*thunk) & 0xffff));
			else (*thunk) = GetProcAddress64(handle, (uint64_t)(base + (*thunk) + 2));
			thunk++;
		}
		imp++;
	}

	uint64_t fix = nt->OptionalHeader.ImageBase - ((uint64_t)base);
	IMAGE_BASE_RELOCATION *reloc = (IMAGE_BASE_RELOCATION*)(base + nt->OptionalHeader.DataDirectory[5].VirtualAddress);
	while (reloc->SizeOfBlock) {
		uint16_t *fixup = (uint16_t*)(reloc + 1);
		for (uint32_t i = 0; i < ((reloc->SizeOfBlock - 8) / 2); i++) {
			char typ = fixup[i] >> 12;
			if (!typ)continue;
			uint16_t offset = fixup[i] & 0x0fff;
			(*((uint64_t*)(base + reloc->VirtualAddress + offset))) -= fix;
		}
		reloc = (IMAGE_BASE_RELOCATION*)(((char*)reloc) + reloc->SizeOfBlock);
	}

	if (entry)(*entry) = (uint64_t)(base + nt->OptionalHeader.AddressOfEntryPoint);
	return base;
}