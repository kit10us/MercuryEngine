// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <meedr/ui/Button.h>

using namespace meedr;
using namespace ui;

Button::Button( std::wstring text, DefaultWidth, DefaultHeight, int id )
	: Control( id, DefaultWidth::Value(), DefaultHeight::Value() ), m_text{ text }
{
}
Button::Button( std::wstring text, DefaultWidth, FillHeight heightWeight, int id )
	: Control( id, DefaultWidth::Value(), FillHeight::Value() ), m_text{ text }
{
	SetFillHeightWeight( heightWeight.weight );
}
Button::Button( std::wstring text, DefaultWidth, int height, int id )
	: Control( id, DefaultWidth::Value(), height ), m_text{ text }
{
}

Button::Button( std::wstring text, FillWidth widthWeight, DefaultHeight, int id )
	: Control( id, FillWidth::Value(), DefaultHeight::Value() ), m_text{ text }
{
	SetFillWidthWeight( widthWeight.weight );
}
Button::Button( std::wstring text, FillWidth widthWeight, FillHeight heightWeight, int id )
	: Control( id, FillWidth::Value(), FillHeight::Value() ), m_text{ text }
{
	SetFillWidthWeight( widthWeight.weight );
	SetFillHeightWeight( heightWeight.weight );
}
Button::Button( std::wstring text, FillWidth widthWeight, int height, int id )
	: Control( id, FillWidth::Value(), height ), m_text{ text }
{
	SetFillWidthWeight( widthWeight.weight );
}

Button::Button( std::wstring text, int width, DefaultHeight, int id )
	: Control( id, width, DefaultHeight::Value() ), m_text{ text }
{
}
Button::Button( std::wstring text, int width, FillHeight heightWeight, int id )
	: Control( id, width, FillHeight::Value() ), m_text{ text }
{
	SetFillHeightWeight( heightWeight.weight );
}
Button::Button( std::wstring text, int width, int height, int id )
	: Control( id, width, height ), m_text{ text }
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
