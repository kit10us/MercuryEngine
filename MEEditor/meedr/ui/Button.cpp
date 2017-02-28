// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <meedr/ui/Button.h>

using namespace meedr;
using namespace ui;

Button::Button( std::wstring text, DefaultWidth, DefaultHeight, int id )
	: Control( id ), m_width{ -1 }, m_height{ -1 }, m_text{ text }
{
}
Button::Button( std::wstring text, DefaultWidth, FillHeight heightWeight, int id )
	: Control( id ), m_width{ -1 }, m_height{ -2 }, m_text{ text }
{
	SetFillHeightWeight( heightWeight.weight );
}
Button::Button( std::wstring text, DefaultWidth, int height, int id )
	: Control( id ), m_width{ -1 }, m_height{ height }, m_text{ text }
{
}

Button::Button( std::wstring text, FillWidth widthWeight, DefaultHeight, int id )
	: Control( id ), m_width{ -2 }, m_height{ -1 }, m_text{ text }
{
	SetFillWidthWeight( widthWeight.weight );
}
Button::Button( std::wstring text, FillWidth widthWeight, FillHeight heightWeight, int id )
	: Control( id ), m_width{ -2 }, m_height{ -2 }, m_text{ text }
{
	SetFillWidthWeight( widthWeight.weight );
	SetFillHeightWeight( heightWeight.weight );
}
Button::Button( std::wstring text, FillWidth widthWeight, int height, int id )
	: Control( id ), m_width{ -2 }, m_height{ height }, m_text{ text }
{
	SetFillWidthWeight( widthWeight.weight );
}

Button::Button( std::wstring text, int width, DefaultHeight, int id )
	: Control( id ), m_width{ width }, m_height{ -1 }, m_text{ text }
{
}
Button::Button( std::wstring text, int width, FillHeight heightWeight, int id )
	: Control( id ), m_width{ width }, m_height{ -2 }, m_text{ text }
{
	SetFillHeightWeight( heightWeight.weight );
}
Button::Button( std::wstring text, int width, int height, int id )
	: Control( id ), m_width{ width }, m_height{ height }, m_text{ text }
{
}

Button::~Button()
{
}

int Button::GetWidth()
{
	return m_width;
}

int Button::GetHeight()
{
	return m_height;
}

std::wstring Button::GetText()
{
	return m_text;
}

void Button::Create( int x, int y, int width, int height, HWND parent )
{
	HWND handle = CreateWindowW(
		L"Button",
		m_text.c_str(),
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		x,
		y,
		width,
		height,
		parent,
		(HMENU)GetID(),
		0,
		0
	);
}

int Button::GetDefaultWidth()
{
	return 120;
}

int Button::GetDefaultHeight()
{
	return 26;
}
