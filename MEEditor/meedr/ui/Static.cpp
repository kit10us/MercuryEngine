// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <meedr/ui/Static.h>

using namespace meedr;
using namespace ui;

Static::Static( std::wstring text, DefaultWidth, DefaultHeight, int id )
	: Control( id ), m_width{ DefaultWidth::Value() }, m_height{ DefaultHeight::Value() }, m_text{ text }
{
}
Static::Static( std::wstring text, DefaultWidth, FillHeight, int id )
	: Control( id ), m_width{ DefaultWidth::Value() }, m_height{ FillHeight::Value() }, m_text{ text }
{
}
Static::Static( std::wstring text, DefaultWidth, int height, int id )
	: Control( id ), m_width{ DefaultWidth::Value() }, m_height{ height }, m_text{ text }
{
}

Static::Static( std::wstring text, FillWidth, DefaultHeight, int id )
	: Control( id ), m_width{ FillWidth::Value() }, m_height{ DefaultHeight::Value() }, m_text{ text }
{
}
Static::Static( std::wstring text, FillWidth, FillHeight, int id )
	: Control( id ), m_width{ FillWidth::Value() }, m_height{ FillHeight::Value() }, m_text{ text }
{
}
Static::Static( std::wstring text, FillWidth, int height, int id )
	: Control( id ), m_width{ FillWidth::Value() }, m_height{ height }, m_text{ text }
{
}

Static::Static( std::wstring text, SizeToContentWidth, DefaultHeight, int id )
	: Control( id ), m_width{ SizeToContentWidth::Value() }, m_height{ DefaultHeight::Value() }, m_text{ text }
{
}
Static::Static( std::wstring text, SizeToContentWidth, FillHeight, int id )
	: Control( id ), m_width{ SizeToContentWidth::Value() }, m_height{ FillHeight::Value() }, m_text{ text }
{
}
Static::Static( std::wstring text, SizeToContentWidth, int height, int id )
	: Control( id ), m_width{ SizeToContentWidth::Value() }, m_height{ height }, m_text{ text }
{
}

Static::Static( std::wstring text, int width, DefaultHeight, int id )
	: Control( id ), m_width{ width }, m_height{ DefaultHeight::Value() }, m_text{ text }
{
}
Static::Static( std::wstring text, int width, FillHeight, int id )
	: Control( id ), m_width{ width }, m_height{ FillHeight::Value() }, m_text{ text }
{
}
Static::Static( std::wstring text, int width, int height, int id )
	: Control( id ), m_width{ width }, m_height{ height }, m_text{ text }
{
}

Static::~Static()
{
}

int Static::GetWidth()
{	
	if ( SizeToContentWidth() == m_width )
	{
		int charWidth = LOWORD( GetDialogBaseUnits() ) + 2;
		return charWidth * m_text.size();
	}
	else
	{
		return m_width;
	}
}

int Static::GetHeight()
{
	return m_height;
}

std::wstring Static::GetText()
{
	return m_text;
}

void Static::Create( int x, int y, int width, int height, HWND parent )
{
	HWND handle = CreateWindowW(
		L"Static",
		m_text.c_str(),
		WS_VISIBLE | WS_CHILD,
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

int Static::GetDefaultWidth()
{

	return 160;
}

int Static::GetDefaultHeight()
{
	return 20;
}
