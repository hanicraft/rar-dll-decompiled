#if !defined(_RAR_SUPP_H_0000F000_INCLUDED_)
#define _RAR_SUPP_H_0000F000_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <windows.h>

#pragma pack(push)
#pragma pack(1)

__declspec(align(16)) struct stRar
{
	UCHAR Header[0x1000];
	UCHAR Text[0xA000];
	UCHAR Rdata[0x1000];
	UCHAR Data[0x1000];
	UCHAR Reloc[0x1000];
	UCHAR Rsrc[0x1000];
};

#pragma pack(pop)

__declspec(align(16)) extern stRar Rar;

/* Data for initialization. */
extern UCHAR Rar_InitData[0xAF2];

extern HMODULE g_hRar;

/* Description: Initialize the DLL. */
BOOL Rar_Init();

/* Description: Load the DLL. */
BOOL Rar_LoadLibrary();

/* Description: Free the DLL. */
BOOL Rar_FreeLibrary();

/*
	Description: Access any address in the DLL.
	Parameters:
		rvaAddr: The RVA address you want to access.
*/
void* __stdcall Rar_RVA(DWORD rvaAddr);

#define Rar_VA(vaAddr) Rar_RVA((vaAddr) - 0x10000000)

/*
	Description: Retrieves the address of an exported function or variable in the DLL.
	Parameters:
		lpProcName: The function or variable name.
*/
FARPROC Rar_GetProcAddress(LPCSTR lpProcName);

#endif // !defined(_RAR_SUPP_H_0000F000_INCLUDED_)