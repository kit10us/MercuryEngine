// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <meedr/ui/Static.h>

using namespace meedr;
using namespace ui;

Static::Static( std::wstring text, DefaultWidth, DefaultHeight )
	: Control(DefaultWidth::Value(), DefaultHeight::Value() ), m_text{ text }
{
}
Static::Static( std::wstring text, DefaultWidth, FillHeight fillHeight )
	: Control(DefaultWidth::Value(), FillHeight::Value() ), m_text{ text }
{
	SetFillHeightWeight( fillHeight.weight );
}
Static::Static( std::wstring text, DefaultWidth, int height )
	: Control(DefaultWidth::Value(), height ), m_text{ text }
{
}

Static::Static( std::wstring text, FillWidth fillWidth, DefaultHeight )
	: Control(FillWidth::Value(), DefaultHeight::Value() ), m_text{ text }
{
	SetFillWidthWeight( fillWidth.weight );
}
Static::Static( std::wstring text, FillWidth fillWidth, FillHeight fillHeight )
	: Control(FillWidth::Value(), FillHeight::Value() ), m_text{ text }
{
	SetFillWidthWeight( fillWidth.weight );
	SetFillHeightWeight( fillHeight.weight );
}
Static::Static( std::wstring text, FillWidth fillWidth, int height )
	: Control(FillWidth::Value(), height ), m_text{ text }
{
	SetFillWidthWeight( fillWidth.weight );
}

Static::Static( std::wstring text, SizeToContentWidth, DefaultHeight )
	: Control(SizeToContentWidth::Value(), DefaultHeight::Value() ), m_text{ text }
{
}
Static::Static( std::wstring text, SizeToContentWidth, FillHeight fillHeight )
	: Control(SizeToContentWidth::Value(), FillHeight::Value() ), m_text{ text }
{
	SetFillHeightWeight( fillHeight.weight );
}
Static::Static( std::wstring text, SizeToContentWidth, int height )
	: Control(SizeToContentWidth::Value(), height ), m_text{ text }
{
}

Static::Static( std::wstring text, int width, DefaultHeight )
	: Control(width, DefaultHeight::Value() ), m_text{ text }
{
}
Static::Static( std::wstring text, int width, FillHeight )
	: Control(width, FillHeight::Value() ), m_text{ text }
{
}
Static::Static( std::wstring text, int width, int height )
	: Control(width, height ), m_text{ text }
{
}

Static::~Static()
{
}

std::wstring Static::GetText()
{
	return m_text;
}

int Static::GetDefaultWidth() const
{
	return 160;
}

int Static::GetDefaultHeight() const
{
	return 20;
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

void Static::ComputePass1()
{
	Control::ComputePass1();

	if ( SizeToContentWidth() == GetWantedWidth() )
	{
		int charWidth = LOWORD( GetDialogBaseUnits() ) + 2;
		m_actualWidth = charWidth * m_text.size();
	}
}
