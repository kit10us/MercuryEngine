#pragma once

#include <windows.h>
#include <D3D10_1.h>
#include <D2D1.h>
#include <atlbase.h>
#include "Graphic.h"

#pragma comment( lib, "d3d10_1.lib" )

class Display
{
	RECT m_area;

	POINT m_sourcePosition;
	SIZE m_size;
	BLENDFUNCTION m_blendFunction;
	UPDATELAYEREDWINDOWINFO m_layeredWindowInfo;
	CComPtr< ID3D10Device1 > m_device;
	CComPtr< ID3D10Texture2D > m_texture;
	CComPtr< ID2D1RenderTarget > m_target;
	CComPtr< IDXGISurface > m_surface;
	HDC m_DC;
	HWND m_hwnd;
	Graphic::shared_ptr m_rootGraphic;
	D2D1_COLOR_F m_clearColor;
	float m_opacity;

public:
	Display( const RECT & area );
	Display( const Display & display );
	virtual ~Display();

	HWND GetHwnd() const;
	const RECT & GetRect() const;
	int GetWidth() const;
	int GetHeight() const;
	D2D1_RECT_F GetRectF() const;
	ID2D1RenderTarget * GetTarget() const;
	Graphic * GetRootGraphic();
	void SetClearColor( const D2D1_COLOR_F & clearColor );
	const D2D1_COLOR_F & GetClearColor() const;

	void Create( HWND hwnd, ID2D1Factory * factory );
	virtual void CreateDeviceDependantResources( ID2D1Factory * factory );
	virtual void DiscardDeviceDependentResources();
	void Draw();

	void Hide();
};
