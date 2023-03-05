#include "stdafx.h"
#include "Rar.h"


HMODULE g_hRar;
BOOL (WINAPI *Rar_DllEntryPoint)(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved);

extern UCHAR Rar_text[0xA000];
extern UCHAR Rar_rdata[0x1000];
extern UCHAR Rar_data[0x1000];

__declspec(align(16)) stRar Rar;

void* __stdcall Rar_RVA(DWORD rvaAddr)
{
	return (UCHAR*)&Rar + rvaAddr;
}

BOOL Rar_Init()
{
	HMODULE hDll;
	DWORD oldProtect;
	void (*fInitData)(HMODULE,void*,void*);

	g_hRar = GetModuleHandle(0);

	oldProtect = PAGE_EXECUTE_READWRITE;
	VirtualProtect(&Rar,sizeof(Rar),PAGE_EXECUTE_READWRITE,&oldProtect);

	memcpy(&Rar.Text,&Rar_text,sizeof(Rar_text));
	memcpy(&Rar.Rdata,&Rar_rdata,sizeof(Rar_rdata));
	memcpy(&Rar.Data,&Rar_data,sizeof(Rar_data));

	hDll = ::LoadLibraryA("OLEAUT32.dll");
	if(!hDll)
	{
#ifdef _DEBUG
		MessageBoxA(NULL,
			"Please copy the dependent DLL \"OLEAUT32.dll\" to the working directory.",
			"Load DLL Failed",MB_OK|MB_ICONERROR);
#endif
		ASSERT(0);
		return FALSE;
	}
	*(FARPROC*)Rar_VA(0x1000B03C) = ::GetProcAddress(hDll,(LPCSTR)0x96);
	if(!*(FARPROC*)Rar_VA(0x1000B03C))
		return FALSE;
	*(FARPROC*)Rar_VA(0x1000B040) = ::GetProcAddress(hDll,(LPCSTR)0x9);
	if(!*(FARPROC*)Rar_VA(0x1000B040))
		return FALSE;
	*(FARPROC*)Rar_VA(0x1000B044) = ::GetProcAddress(hDll,(LPCSTR)0x2);
	if(!*(FARPROC*)Rar_VA(0x1000B044))
		return FALSE;

	hDll = ::LoadLibraryA("MSVCRT.dll");
	if(!hDll)
	{
#ifdef _DEBUG
		MessageBoxA(NULL,
			"Please copy the dependent DLL \"MSVCRT.dll\" to the working directory.",
			"Load DLL Failed",MB_OK|MB_ICONERROR);
#endif
		ASSERT(0);
		return FALSE;
	}
	*(FARPROC*)Rar_VA(0x1000B000) = ::GetProcAddress(hDll,"free");
	if(!*(FARPROC*)Rar_VA(0x1000B000))
		return FALSE;
	*(FARPROC*)Rar_VA(0x1000B004) = ::GetProcAddress(hDll,"realloc");
	if(!*(FARPROC*)Rar_VA(0x1000B004))
		return FALSE;
	*(FARPROC*)Rar_VA(0x1000B008) = ::GetProcAddress(hDll,"??2@YAPAXI@Z");
	if(!*(FARPROC*)Rar_VA(0x1000B008))
		return FALSE;
	*(FARPROC*)Rar_VA(0x1000B00C) = ::GetProcAddress(hDll,"memcmp");
	if(!*(FARPROC*)Rar_VA(0x1000B00C))
		return FALSE;
	*(FARPROC*)Rar_VA(0x1000B010) = ::GetProcAddress(hDll,"__CxxFrameHandler");
	if(!*(FARPROC*)Rar_VA(0x1000B010))
		return FALSE;
	*(FARPROC*)Rar_VA(0x1000B014) = ::GetProcAddress(hDll,"_CxxThrowException");
	if(!*(FARPROC*)Rar_VA(0x1000B014))
		return FALSE;
	*(FARPROC*)Rar_VA(0x1000B018) = ::GetProcAddress(hDll,"malloc");
	if(!*(FARPROC*)Rar_VA(0x1000B018))
		return FALSE;
	*(FARPROC*)Rar_VA(0x1000B01C) = ::GetProcAddress(hDll,"exit");
	if(!*(FARPROC*)Rar_VA(0x1000B01C))
		return FALSE;
	*(FARPROC*)Rar_VA(0x1000B020) = ::GetProcAddress(hDll,"_except_handler3");
	if(!*(FARPROC*)Rar_VA(0x1000B020))
		return FALSE;
	*(FARPROC*)Rar_VA(0x1000B024) = ::GetProcAddress(hDll,"memmove");
	if(!*(FARPROC*)Rar_VA(0x1000B024))
		return FALSE;
	*(FARPROC*)Rar_VA(0x1000B028) = ::GetProcAddress(hDll,"??1type_info@@UAE@XZ");
	if(!*(FARPROC*)Rar_VA(0x1000B028))
		return FALSE;
	*(FARPROC*)Rar_VA(0x1000B02C) = ::GetProcAddress(hDll,"_adjust_fdiv");
	if(!*(FARPROC*)Rar_VA(0x1000B02C))
		return FALSE;
	*(FARPROC*)Rar_VA(0x1000B030) = ::GetProcAddress(hDll,"?terminate@@YAXXZ");
	if(!*(FARPROC*)Rar_VA(0x1000B030))
		return FALSE;
	*(FARPROC*)Rar_VA(0x1000B034) = ::GetProcAddress(hDll,"_initterm");
	if(!*(FARPROC*)Rar_VA(0x1000B034))
		return FALSE;

	VirtualProtect(&Rar_InitData, sizeof(Rar_InitData), PAGE_EXECUTE_READWRITE, &oldProtect);

	*(FARPROC*)&fInitData = (FARPROC)&Rar_InitData[0];
	fInitData(g_hRar,Rar_RVA,NULL);

	*(FARPROC*)&Rar_DllEntryPoint = (FARPROC)Rar_VA(0x1000A24A);
	return TRUE;
}

BOOL Rar_LoadLibrary()
{
	return Rar_DllEntryPoint(g_hRar, DLL_PROCESS_ATTACH, 0);
}

BOOL Rar_FreeLibrary()
{
	return Rar_DllEntryPoint(g_hRar, DLL_PROCESS_DETACH, 0);
}

FARPROC Rar_GetProcAddress(LPCSTR lpProcName)
{
	if(lstrcmpA(lpProcName,"CreateObject")==0)
		return (FARPROC)Rar_RVA(0x1006);
	if(lstrcmpA(lpProcName,"GetMethodProperty")==0)
		return (FARPROC)Rar_RVA(0x1175);
	if(lstrcmpA(lpProcName,"GetNumberOfMethods")==0)
		return (FARPROC)Rar_RVA(0x1166);

	return NULL;
}
