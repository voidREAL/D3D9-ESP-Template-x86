#include <Windows.h>
#include <iostream>

#include "../framework/include/mem.h"
#include "../include/include.h"
#include "../include/dx.h"

void* d3d9Vtable[119];

BYTE presentByte[5] = { 0 };
BYTE resetByte[5] = { 0 };

_Present present = nullptr;
_Reset resetFunc = nullptr;

bool isInit = false;

HRESULT APIENTRY hookedReset(LPDIRECT3DDEVICE9 o_pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters) {
	return resetFunc(o_pDevice, pPresentationParameters);
}

HRESULT APIENTRY hookedPresent(LPDIRECT3DDEVICE9 o_pDevice, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion) {
	if (!pDevice) {
		pDevice = o_pDevice;
	}

	return present(o_pDevice, NULL, NULL, NULL, NULL);
}
DWORD WINAPI HackThread(HMODULE hModule) {
	AllocConsole();
	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);
	MemoryInternal mem;

	viewMatrix[0] = 1.0f; // UPDATE VIEW MATRIX FROM GAME HERE

	if (GetD3D9DeviceVTable(d3d9Vtable, sizeof(d3d9Vtable))) {
		memcpy(presentByte, (BYTE*)d3d9Vtable[17], 5);
		memcpy(resetByte, (BYTE*)d3d9Vtable[16], 5);
		present = (_Present)mem.trampolineHook((BYTE*)d3d9Vtable[17], (BYTE*)hookedPresent, 5);
		resetFunc = (_Reset)mem.trampolineHook((BYTE*)d3d9Vtable[16], (BYTE*)hookedReset, 5);
	}

	while (!GetAsyncKeyState(VK_END)) {}

	//Unhook and cleanup
	{
		mem.patch((BYTE*)d3d9Vtable[17], presentByte, 5);
		mem.patch((BYTE*)d3d9Vtable[16], resetByte, 5);

		VirtualFree(present, 5, MEM_FREE);
		VirtualFree(resetFunc, 5, MEM_FREE);

		FreeConsole();
		if (f) {
			fclose(f);
		}

		Sleep(1000);

		FreeLibraryAndExitThread(hModule, 0);
		return 0;
	}
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
		HANDLE thread = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, nullptr);
		if (thread) {
			CloseHandle(thread);
		}
    }
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

