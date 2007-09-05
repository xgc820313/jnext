#ifdef _WINDOWS

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#pragma warning(disable : 4996)
#include <windows.h>
#include <windowsx.h>

#ifdef NPRN_EXPORTS
#define NPRN_API __declspec(dllexport)
#else
#define NPRN_API __declspec(dllimport)
#endif

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}

#endif
////////////////////////////////////////////////////////////
