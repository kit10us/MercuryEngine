// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <meedr/ui/Listbox.h>

using namespace meedr;
using namespace ui;

Listbox::Listbox( DefaultWidth, DefaultHeight, int id )
	: Control( id, DefaultWidth::Value(), DefaultHeight::Value() ), m_sorted{ true }
{
}
Listbox::Listbox( DefaultWidth, FillHeight heightWeight, int id )
	: Control( id, DefaultWidth::Value(), FillHeight::Value() ), m_sorted{ true }
{
	SetFillHeightWeight( heightWeight.weight );
}
Listbox::Listbox( DefaultWidth, int height, int id )
	: Control( id, DefaultWidth::Value(), height ), m_sorted{ true }
{
}

Listbox::Listbox( FillWidth widthWeight, DefaultHeight, int id )
	: Control( id, FillWidth::Value(), DefaultHeight::Value() ), m_sorted{ true }
{
	SetFillWidthWeight( widthWeight.weight );
}
Listbox::Listbox( FillWidth widthWeight, FillHeight heightWeight, int id )
	: Control( id, FillWidth::Value(), FillHeight::Value() ), m_sorted{ true }
{
	SetFillWidthWeight( widthWeight.weight );
	SetFillHeightWeight( heightWeight.weight );
}
Listbox::Listbox( FillWidth widthWeight, int height, int id )
	: Control( id, FillWidth::Value(), height ), m_sorted{ true }
{
	SetFillWidthWeight( widthWeight.weight );
}

Listbox::Listbox( int width, DefaultHeight, int id )
	: Control( id, width, DefaultHeight::Value() ), m_sorted{ true }
{
}
Listbox::Listbox( int width, FillHeight heightWeight, int id )
	: Control( id, width, FillHeight::Value() ), m_sorted{ true }
{
	SetFillHeightWeight( heightWeight.weight );
}
Listbox::Listbox( int width, int height, int id )
	: Control( id, width, height ), m_sorted{ true }
{
}

Listbox::~Listbox()
{
}

void Listbox::Create( HWND parent )
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

int Listbox::GetDefaultWidth() const
{
	return 120;
}

int Listbox::GetDefaultHeight() const
{
	return 240;
}

Listbox * Listbox::SetSorted( bool sorted )
{
	m_sorted = sorted;
	return this;
}
