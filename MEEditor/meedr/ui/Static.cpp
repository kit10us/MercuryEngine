// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <meedr/ui/Static.h>

using namespace meedr;
using namespace ui;

Static::Static( std::wstring text, DefaultWidth, DefaultHeight, int id )
	: Control( id, DefaultWidth::Value(), DefaultHeight::Value() ), m_text{ text }
{
}
Static::Static( std::wstring text, DefaultWidth, FillHeight fillHeight, int id )
	: Control( id, DefaultWidth::Value(), FillHeight::Value() ), m_text{ text }
{
	SetFillHeightWeight( fillHeight.weight );
}
Static::Static( std::wstring text, DefaultWidth, int height, int id )
	: Control( id, DefaultWidth::Value(), height ), m_text{ text }
{
}

Static::Static( std::wstring text, FillWidth fillWidth, DefaultHeight, int id )
	: Control( id, FillWidth::Value(), DefaultHeight::Value() ), m_text{ text }
{
	SetFillWidthWeight( fillWidth.weight );
}
Static::Static( std::wstring text, FillWidth fillWidth, FillHeight fillHeight, int id )
	: Control( id, FillWidth::Value(), FillHeight::Value() ), m_text{ text }
{
	SetFillWidthWeight( fillWidth.weight );
	SetFillHeightWeight( fillHeight.weight );
}
Static::Static( std::wstring text, FillWidth fillWidth, int height, int id )
	: Control( id, FillWidth::Value(), height ), m_text{ text }
{
	SetFillWidthWeight( fillWidth.weight );
}

Static::Static( std::wstring text, SizeToContentWidth, DefaultHeight, int id )
	: Control( id, SizeToContentWidth::Value(), DefaultHeight::Value() ), m_text{ text }
{
}
Static::Static( std::wstring text, SizeToContentWidth, FillHeight fillHeight, int id )
	: Control( id, SizeToContentWidth::Value(), FillHeight::Value() ), m_text{ text }
{
	SetFillHeightWeight( fillHeight.weight );
}
Static::Static( std::wstring text, SizeToContentWidth, int height, int id )
	: Control( id, SizeToContentWidth::Value(), height ), m_text{ text }
{
}

Static::Static( std::wstring text, int width, DefaultHeight, int id )
	: Control( id, width, DefaultHeight::Value() ), m_text{ text }
{
}
Static::Static( std::wstring text, int width, FillHeight, int id )
	: Control( id, width, FillHeight::Value() ), m_text{ text }
{
}
Static::Static( std::wstring text, int width, int height, int id )
	: Control( id, width, height ), m_text{ text }
{
}

Static::~Static()
{
}

std::wstring Static::GetText()
{
	return m_text;
}

void Static::Create( HWND parent )
{
	HWND handle = CreateWindowW(
		L"Static",
		m_text.c_str(),
		WS_VISIBLE | WS_CHILD,
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
		int charWidth = LOWORD( GetDialogBaseUnits() ) + 2;
		m_actualWidth = charWidth * m_text.size();
	}
}
