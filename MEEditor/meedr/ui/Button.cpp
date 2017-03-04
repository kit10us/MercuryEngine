// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <meedr/ui/Button.h>

using namespace meedr;
using namespace ui;

Button::Button( std::wstring text, DefaultWidth, DefaultHeight )
	: Control(DefaultWidth::Value(), DefaultHeight::Value() ), m_text{ text }
{
}
Button::Button( std::wstring text, DefaultWidth, FillHeight heightWeight )
	: Control(DefaultWidth::Value(), FillHeight::Value() ), m_text{ text }
{
	SetFillHeightWeight( heightWeight.weight );
}
Button::Button( std::wstring text, DefaultWidth, int height )
	: Control(DefaultWidth::Value(), height ), m_text{ text }
{  
}

Button::Button( std::wstring text, FillWidth widthWeight, DefaultHeight )
	: Control(FillWidth::Value(), DefaultHeight::Value() ), m_text{ text }
{
	SetFillWidthWeight( widthWeight.weight );
}
Button::Button( std::wstring text, FillWidth widthWeight, FillHeight heightWeight )
	: Control(FillWidth::Value(), FillHeight::Value() ), m_text{ text }
{
	SetFillWidthWeight( widthWeight.weight );
	SetFillHeightWeight( heightWeight.weight );
}
Button::Button( std::wstring text, FillWidth widthWeight, int height )
	: Control(FillWidth::Value(), height ), m_text{ text }
{
	SetFillWidthWeight( widthWeight.weight );
}

Button::Button( std::wstring text, SizeToContentWidth, DefaultHeight )
	: Control(SizeToContentWidth::Value(), DefaultHeight::Value() ), m_text{ text }
{
}
Button::Button( std::wstring text, SizeToContentWidth, FillHeight heightWeight )
	: Control(SizeToContentWidth::Value(), FillHeight::Value() ), m_text{ text }
{
	SetFillHeightWeight( heightWeight.weight );
}
Button::Button( std::wstring text, SizeToContentWidth, int height )
	: Control(SizeToContentWidth::Value(), height ), m_text{ text }
{
}

Button::Button( std::wstring text, int width, DefaultHeight )
	: Control(width, DefaultHeight::Value() ), m_text{ text }
{
}
Button::Button( std::wstring text, int width, FillHeight heightWeight )
	: Control(width, FillHeight::Value() ), m_text{ text }
{
	SetFillHeightWeight( heightWeight.weight );
}
Button::Button( std::wstring text, int width, int height )
	: Control(width, height ), m_text{ text }
{
}

Button::~Button()
{
}

std::wstring Button::GetText()
{
	return m_text;
}

int Button::GetDefaultWidth() const
{
	return 120;
}

int Button::GetDefaultHeight() const
{
	return 26;
}

void Button::Create( HWND parent )
{
	HWND handle = CreateWindowW(
		L"Button",
		m_text.c_str(),
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
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

void Button::ComputePass1()
{
	Control::ComputePass1();

	if ( SizeToContentWidth() == GetWantedWidth() )
	{
		int charWidth = LOWORD( GetDialogBaseUnits() ) + 2;
		m_actualWidth = charWidth * m_text.size();
	}
}