// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <wab/create/Create_Listbox.h>
#include <wab/ui/Listbox.h>

using namespace create;

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

void Listbox::Create( HWND parent )
{
	HWND handle = CreateWindowW(
		GetType().c_str(),
		GetWantedText().c_str(),
		GetWantedStyle(),
		GetActualX(),
		GetActualY(),
		GetActualWidth(),
		GetActualHeight(), 
		parent,
		(HMENU)GetID(),
		0,
		0
	);

	m_control.reset( new ui::Listbox( parent, handle, this ) );
}
