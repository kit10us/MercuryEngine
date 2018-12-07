// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <wab/create/Create_Static.h>
#include <wab/ui/Static.h>

using namespace create;

Static::Static( std::wstring text, DefaultWidth, DefaultHeight )
	: Control(DefaultWidth::Value(), DefaultHeight::Value(), text )
{
}
Static::Static( std::wstring text, DefaultWidth, FillHeight fillHeight )
	: Control(DefaultWidth::Value(), FillHeight::Value(), text )
{
	SetFillHeightWeight( fillHeight.weight );
}
Static::Static( std::wstring text, DefaultWidth, int height )
	: Control(DefaultWidth::Value(), height, text )
{
}

Static::Static( std::wstring text, FillWidth fillWidth, DefaultHeight )
	: Control(FillWidth::Value(), DefaultHeight::Value(), text )
{
	SetFillWidthWeight( fillWidth.weight );
}
Static::Static( std::wstring text, FillWidth fillWidth, FillHeight fillHeight )
	: Control(FillWidth::Value(), FillHeight::Value(), text )
{
	SetFillWidthWeight( fillWidth.weight );
	SetFillHeightWeight( fillHeight.weight );
}
Static::Static( std::wstring text, FillWidth fillWidth, int height )
	: Control(FillWidth::Value(), height, text )
{
	SetFillWidthWeight( fillWidth.weight );
}

Static::Static( std::wstring text, SizeToContentWidth, DefaultHeight )
	: Control(SizeToContentWidth::Value(), DefaultHeight::Value(), text )
{
}
Static::Static( std::wstring text, SizeToContentWidth, FillHeight fillHeight )
	: Control(SizeToContentWidth::Value(), FillHeight::Value(), text )
{
	SetFillHeightWeight( fillHeight.weight );
}
Static::Static( std::wstring text, SizeToContentWidth, int height )
	: Control(SizeToContentWidth::Value(), height, text )
{
}

Static::Static( std::wstring text, int width, DefaultHeight )
	: Control(width, DefaultHeight::Value(), text )
{
}
Static::Static( std::wstring text, int width, FillHeight )
	: Control(width, FillHeight::Value(), text )
{
}
Static::Static( std::wstring text, int width, int height )
	: Control(width, height, text )
{
}

Static::~Static()
{
}

DWORD Static::GetWantedStyle() const
{
	DWORD style = Control::GetWantedStyle();
	return style;
}

std::wstring Static::GetType() const
{
	return L"Static";
}

int Static::GetDefaultWidth() const
{
	return 160;
}

int Static::GetDefaultHeight() const
{
	return 20;
}

void Static::ComputePass1()
{
	Control::ComputePass1();

	if ( SizeToContentWidth() == GetWantedWidth() )
	{
		int charWidth = LOWORD( GetDialogBaseUnits() );
		m_actualWidth = charWidth * m_wantedText.size();
	}
}

void Static::Create( HWND parent )
{
	HWND handle = CreateWindowW(
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

	m_control.reset( new ui::Static( parent, handle, this ) );
}
