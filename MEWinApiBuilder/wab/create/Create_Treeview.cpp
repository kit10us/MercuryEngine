// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <wab/create/Create_Treeview.h>
#include <wab/ui/Treeview.h>
#include <Commctrl.h>

#pragma comment( lib, "Comctl32" )

using namespace create;

Treeview::Treeview( DefaultWidth fillWidth, FillHeight fillHeight )
	: Control(DefaultWidth::Value(), FillHeight::Value() )
{
	SetFillHeightWeight( fillHeight.weight );
}
Treeview::Treeview( DefaultWidth fillWidth, int height )
	: Control(DefaultWidth::Value(), height )
{
}

Treeview::Treeview( FillWidth fillWidth, FillHeight fillHeight )
	: Control(FillWidth::Value(), FillHeight::Value() )
{
	SetFillWidthWeight( fillWidth.weight );
	SetFillHeightWeight( fillHeight.weight );
}
Treeview::Treeview( FillWidth fillWidth, int height )
	: Control(FillWidth::Value(), height )
{
	SetFillWidthWeight( fillWidth.weight );
}

Treeview::Treeview( int width, FillHeight fillHeight )
	: Control(width, FillHeight::Value() )
{
	SetFillHeightWeight( fillHeight.weight );
}
Treeview::Treeview( int width, int height )
	: Control(width, height )
{
}

Treeview::~Treeview()
{
}	  

DWORD Treeview::GetWantedStyle() const
{
	DWORD style = Control::GetWantedStyle();
	return style |= WS_BORDER | WS_TABSTOP | WS_HSCROLL | WS_VSCROLL | TVS_HASLINES;
}

std::wstring Treeview::GetType() const
{
	return WC_TREEVIEWW;
}

void Treeview::Create( HWND parent )
{
	INITCOMMONCONTROLSEX initCtrls{};
	initCtrls.dwSize = sizeof( this );
	initCtrls.dwICC = ICC_TREEVIEW_CLASSES;
	InitCommonControlsEx( &initCtrls );

	HWND handle = CreateWindowExW(
		0,
		GetType().c_str(),
		GetWantedText().c_str(),
		GetWantedStyle(),
		GetActualX(),
		GetActualY(),
		GetActualWidth(),
		GetActualHeight(),
		parent,
		(HMENU)GetID(),
		0,
		0
	);

	m_control.reset( new ui::Treeview( parent, handle, this ) );
}

int Treeview::GetDefaultWidth() const
{
	return 160;
}

int Treeview::GetDefaultHeight() const
{
	return 420;
}
