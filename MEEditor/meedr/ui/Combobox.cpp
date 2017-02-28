// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <meedr/ui/Combobox.h>

using namespace meedr;
using namespace ui;

Combobox::Combobox( DefaultWidth, DefaultHeight, int id )
	: Control( id ), m_width{ -1 }, m_height{ -1 }
{
}
Combobox::Combobox( DefaultWidth, int height, int id )
	: Control( id ), m_width{ -1 }, m_height{ height }
{
}

Combobox::Combobox( FillWidth widthWeight, DefaultHeight, int id )
	: Control( id ), m_width{ -2 }, m_height{ -1 }
{
	SetFillWidthWeight( widthWeight.weight );
}
Combobox::Combobox( FillWidth widthWeight, int height, int id )
	: Control( id ), m_width{ -2 }, m_height{ height }
{
	SetFillWidthWeight( widthWeight.weight );
}

Combobox::Combobox( int width, DefaultHeight, int id )
	: Control( id ), m_width{ width }, m_height{ -1 }
{
}
Combobox::Combobox( int width, int height, int id )
	: Control( id ), m_width{ width }, m_height{ height }
{
}

Combobox::~Combobox()
{
}

int Combobox::GetWidth()
{
	return m_width;
}

int Combobox::GetHeight()
{
	return 24;
}

void Combobox::Create( int x, int y, int width, int height, HWND parent )
{
	HWND handle = CreateWindowW(
		L"Combobox",
		L"",
		CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_TABSTOP | WS_VISIBLE | WS_CHILD,
		x,
		y,
		width,
		( ui::DefaultHeight() == m_height ) ? GetDefaultHeight() : height,
		parent,
		(HMENU)GetID(),
		0,
		0
	);
}

int Combobox::GetDefaultWidth()
{
	return 120;
}

int Combobox::GetDefaultHeight()
{
	return 240;
}
