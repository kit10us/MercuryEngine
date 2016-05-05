#pragma once

#include <D3DX11.h>
#include <Windows.h>
#include <d2d1.h>
#include <d2d1helper.h>
#include <DWrite.h>
#include <string>
#include <memory>
#include <vector>
#include "Display.h"

class WarApp
{
	HWND m_mainHwnd;
	CComPtr< ID2D1Factory > m_direct2DFactory;
	CComPtr< IDWriteFactory > m_directWriteFactory;
	float m_secondsPerCount;
	long long m_lastCount;
	float m_elapsed;
	
	std::vector< Display > m_displays;
	
	//CComPtr< ID2D1SolidColorBrush > m_textBrush;
	CComPtr< IDWriteTextFormat > m_textFormat;

	struct ActiveArea
	{
		D2D1_RECT_F hoverArea;
		RECT moveWindowArea;
	};

	std::vector< ActiveArea > m_activeAreas;

	bool m_warMode;

public:
	WarApp( HINSTANCE instance, char * cmdLine, int cmdShow, WNDPROC wndProc );
	~WarApp();
	void CreateDeviceDependentResources();
	void DiscardDeviceDependentResources();
	void Go();
	void Update();
	void Render();
};