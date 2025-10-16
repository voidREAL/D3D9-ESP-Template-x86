#include "../include/dx.h"

int windowHeight, windowWidth;
HWND window = nullptr;
float viewMatrix[16] = { 0 };

LPDIRECT3DDEVICE9 pDevice = nullptr;
ID3DXLine* lineL = nullptr;
ID3DXFont* font = nullptr;

BOOL CALLBACK enumWind(HWND hwnd, LPARAM lParam) {
	DWORD procId;
	GetWindowThreadProcessId(hwnd, &procId);
	if (procId == GetCurrentProcessId()) {
		window = hwnd;
		return FALSE;
	}
	return TRUE;
}

HWND GetProcessWindow() {
	window = NULL;

	EnumWindows(enumWind, NULL);

	RECT size;

	if(!window) {
		return NULL;
	}

	GetClientRect(window, &size);
	windowHeight = size.bottom - size.top;
	windowWidth = size.right - size.left;

	return window;
}

bool GetD3D9DeviceVTable(void** pTable, size_t size) {
	if (!pTable) {
		return false;
	}

	IDirect3D9* pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if(!pD3D) {
		return false;
	}

	IDirect3DDevice9* dummyDevice = nullptr;

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = false;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = GetProcessWindow();

	HRESULT dummyDevCreated = pD3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		d3dpp.hDeviceWindow,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&dummyDevice
	);

	if (dummyDevCreated != S_OK) {
		d3dpp.Windowed = !d3dpp.Windowed;
		dummyDevCreated = pD3D->CreateDevice(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			d3dpp.hDeviceWindow,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&dummyDevice
		);

		if (dummyDevCreated != S_OK) {
			pD3D->Release();
			return false;
		}
	}

	memcpy(pTable, *(void***)(dummyDevice), size);
	dummyDevice->Release();
	pD3D->Release();
	return true;

}