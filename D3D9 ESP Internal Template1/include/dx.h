#pragma once
#include "include.h"

using _Present = HRESULT(APIENTRY*)(LPDIRECT3DDEVICE9 pDevice, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion);
using _Reset = HRESULT(APIENTRY*)(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);

bool GetD3D9DeviceVTable(void** pTable, size_t size);

HWND GetProcessWindow();

extern HWND window;

extern int windowHeight, windowWidth;

extern LPDIRECT3DDEVICE9 pDevice;
extern ID3DXLine* lineL;
extern ID3DXFont* font;
extern float viewMatrix[16];