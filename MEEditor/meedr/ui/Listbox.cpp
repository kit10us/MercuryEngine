// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <meedr/ui/Listbox.h>

using namespace meedr;
using namespace ui;

Listbox::Listbox( DefaultWidth, DefaultHeight )
	: Control(DefaultWidth::Value(), DefaultHeight::Value() ), m_sorted{ false }
{
}
Listbox::Listbox( DefaultWidth, FillHeight heightWeight )
	: Control(DefaultWidth::Value(), FillHeight::Value() ), m_sorted{ false }
{
	SetFillHeightWeight( heightWeight.weight );
}
Listbox::Listbox( DefaultWidth, int height )
	: Control(DefaultWidth::Value(), height ), m_sorted{ false }
{
}

Listbox::Listbox( FillWidth widthWeight, DefaultHeight )
	: Control(FillWidth::Value(), DefaultHeight::Value() ), m_sorted{ false }
{
	SetFillWidthWeight( widthWeight.weight );
}
Listbox::Listbox( FillWidth widthWeight, FillHeight heightWeight )
	: Control(FillWidth::Value(), FillHeight::Value() ), m_sorted{ false }
{
	SetFillWidthWeight( widthWeight.weight );
	SetFillHeightWeight( heightWeight.weight );
}
Listbox::Listbox( FillWidth widthWeight, int height )
	: Control(FillWidth::Value(), height ), m_sorted{ false }
{
	SetFillWidthWeight( widthWeight.weight );
}

Listbox::Listbox( int width, DefaultHeight )
	: Control(width, DefaultHeight::Value() ), m_sorted{ false }
{
}
Listbox::Listbox( int width, FillHeight heightWeight )
	: Control(width, FillHeight::Value() ), m_sorted{ false }
{
	SetFillHeightWeight( heightWeight.weight );
}
Listbox::Listbox( int width, int height )
	: Control(width, height ), m_sorted{ false }
{
}

Listbox::~Listbox()
{
}

DWORD Listbox::GetWantedStyle() const
{
	DWORD style = Control::GetWantedStyle();
	style |= WS_TABSTOP;
	style |= LBS_NOTIFY | WS_VSCROLL | WS_BORDER;
	if ( m_sorted )
	{
		style |= LBS_SORT;
	}
	return style;
}

std::wstring Listbox::GetType() const
{
	return L"Listbox";
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

int Listbox::AddFile( std::string filename )
{
	int index = SendMessageA( GetHandle(), LB_ADDFILE, 0, (LPARAM)filename.c_str() );
	return index;
}

int Listbox::AddString( std::string text )
{
	int index = SendMessageA( GetHandle(), LB_ADDSTRING, 0, (LPARAM)text.c_str() );
	return index;
}

int Listbox::DeleteString( int index )
{
	int count = SendMessageA( GetHandle(), LB_DELETESTRING, index, 0 );
	return count;
}

int Listbox::GetCurSel()
{
	return SendMessageA( GetHandle(), LB_GETCURSEL, 0, 0 );
}

void Listbox::ResetContent()
{
	SendMessageA( GetHandle(), LB_RESETCONTENT, 0, 0 );
}

void Listbox::SetCurSel( int sel )
{
	SendMessageA( GetHandle(), LB_SETCURSEL, sel, 0 );
}
