// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <meedr/ui/Listbox.h>

using namespace meedr;
using namespace ui;

Listbox::Listbox( DefaultWidth, DefaultHeight, int id )
	: Control( id ), m_width{ -1 }, m_height{ -1 }, m_sorted{ true }
{
}
Listbox::Listbox( DefaultWidth, FillHeight heightWeight, int id )
	: Control( id ), m_width{ -1 }, m_height{ -2 }, m_sorted{ true }
{
	SetFillHeightWeight( heightWeight.weight );
}
Listbox::Listbox( DefaultWidth, int height, int id )
	: Control( id ), m_width{ -1 }, m_height{ height }, m_sorted{ true }
{
}

Listbox::Listbox( FillWidth widthWeight, DefaultHeight, int id )
	: Control( id ), m_width{ -2 }, m_height{ -1 }, m_sorted{ true }
{
	SetFillWidthWeight( widthWeight.weight );
}
Listbox::Listbox( FillWidth widthWeight, FillHeight heightWeight, int id )
	: Control( id ), m_width{ -2 }, m_height{ -2 }, m_sorted{ true }
{
	SetFillWidthWeight( widthWeight.weight );
	SetFillHeightWeight( heightWeight.weight );
}
Listbox::Listbox( FillWidth widthWeight, int height, int id )
	: Control( id ), m_width{ -2 }, m_height{ height }, m_sorted{ true }
{
	SetFillWidthWeight( widthWeight.weight );
}

Listbox::Listbox( int width, DefaultHeight, int id )
	: Control( id ), m_width{ width }, m_height{ -1 }, m_sorted{ true }
{
}
Listbox::Listbox( int width, FillHeight heightWeight, int id )
	: Control( id ), m_width{ width }, m_height{ -2 }, m_sorted{ true }
{
	SetFillHeightWeight( heightWeight.weight );
}
Listbox::Listbox( int width, int height, int id )
	: Control( id ), m_width{ width }, m_height{ height }, m_sorted{ true }
{
}

Listbox::~Listbox()
{
}

int Listbox::GetWidth()
{
	return m_width;
}

int Listbox::GetHeight()
{
	return m_height;
}

void Listbox::Create( int x, int y, int width, int height, HWND parent )
{
	DWORD style = WS_TABSTOP | WS_VISIBLE | WS_CHILD;
	style |= LBS_NOTIFY | WS_VSCROLL | WS_BORDER;
	if ( m_sorted )
	{
		style |= LBS_SORT;
	}

	HWND handle = CreateWindowW(
		L"Listbox",
		L"",
		style,
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

int Listbox::GetDefaultWidth()
{
	return 120;
}

int Listbox::GetDefaultHeight()
{
	return 240;
}

Listbox * Listbox::SetSorted( bool sorted )
{
	m_sorted = sorted;
	return this;
}
