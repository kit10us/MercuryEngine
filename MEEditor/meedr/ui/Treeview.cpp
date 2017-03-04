// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <meedr/ui/Treeview.h>
#include <Commctrl.h>

#pragma comment( lib, "Comctl32" )

using namespace meedr;
using namespace ui;

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

void Treeview::Create( HWND parent )
{
	INITCOMMONCONTROLSEX initCtrls{};
	initCtrls.dwSize = sizeof( this );
	initCtrls.dwICC = ICC_TREEVIEW_CLASSES;
	InitCommonControlsEx( &initCtrls );

	HWND handle = CreateWindowExW(
		0,
		WC_TREEVIEW,
		L"Tree...",
		WS_VISIBLE | WS_CHILD | WS_BORDER | WS_TABSTOP | WS_HSCROLL | WS_VSCROLL | TVS_HASLINES,
		GetActualX(),
		GetActualY(),
		GetActualWidth(),
		GetActualHeight(),
		parent,
		(HMENU)GetID(),
		0,
		0
	);
}

int Treeview::GetDefaultWidth() const
{
	return 160;
}

int Treeview::GetDefaultHeight() const
{
	return 420;
}
