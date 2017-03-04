// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <meedr/ui/Listbox.h>

using namespace meedr;
using namespace ui;

Listbox::Listbox( DefaultWidth, DefaultHeight )
	: Control(DefaultWidth::Value(), DefaultHeight::Value() ), m_sorted{ true }
{
}
Listbox::Listbox( DefaultWidth, FillHeight heightWeight )
	: Control(DefaultWidth::Value(), FillHeight::Value() ), m_sorted{ true }
{
	SetFillHeightWeight( heightWeight.weight );
}
Listbox::Listbox( DefaultWidth, int height )
	: Control(DefaultWidth::Value(), height ), m_sorted{ true }
{
}

Listbox::Listbox( FillWidth widthWeight, DefaultHeight )
	: Control(FillWidth::Value(), DefaultHeight::Value() ), m_sorted{ true }
{
	SetFillWidthWeight( widthWeight.weight );
}
Listbox::Listbox( FillWidth widthWeight, FillHeight heightWeight )
	: Control(FillWidth::Value(), FillHeight::Value() ), m_sorted{ true }
{
	SetFillWidthWeight( widthWeight.weight );
	SetFillHeightWeight( heightWeight.weight );
}
Listbox::Listbox( FillWidth widthWeight, int height )
	: Control(FillWidth::Value(), height ), m_sorted{ true }
{
	SetFillWidthWeight( widthWeight.weight );
}

Listbox::Listbox( int width, DefaultHeight )
	: Control(width, DefaultHeight::Value() ), m_sorted{ true }
{
}
Listbox::Listbox( int width, FillHeight heightWeight )
	: Control(width, FillHeight::Value() ), m_sorted{ true }
{
	SetFillHeightWeight( heightWeight.weight );
}
Listbox::Listbox( int width, int height )
	: Control(width, height ), m_sorted{ true }
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
