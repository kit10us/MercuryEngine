// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <wab/create/Create_ListView.h>
#include <wab/ui/ListView.h>

#include <CommCtrl.h>

using namespace create;

ListView::ListView( DefaultWidth, DefaultHeight )
	: Control( DefaultWidth::Value(), DefaultHeight::Value() ), m_sorted{ false }, m_fullRowSelect{ false }, m_oneClickActivate{ false }
{
}
ListView::ListView( DefaultWidth, FillHeight heightWeight )
	: Control(DefaultWidth::Value(), FillHeight::Value() ), m_sorted{ false }, m_fullRowSelect{ false }, m_oneClickActivate{ false }
{
	SetFillHeightWeight( heightWeight.weight );
}
ListView::ListView( DefaultWidth, int height )
	: Control(DefaultWidth::Value(), height ), m_sorted{ false }, m_fullRowSelect{ false }, m_oneClickActivate{ false }
{
}

ListView::ListView( FillWidth widthWeight, DefaultHeight )
	: Control(FillWidth::Value(), DefaultHeight::Value() ), m_sorted{ false }, m_fullRowSelect{ false }, m_oneClickActivate{ false }
{
	SetFillWidthWeight( widthWeight.weight );
}
ListView::ListView( FillWidth widthWeight, FillHeight heightWeight )
	: Control(FillWidth::Value(), FillHeight::Value() ), m_sorted{ false }, m_fullRowSelect{ false }, m_oneClickActivate{ false }
{
	SetFillWidthWeight( widthWeight.weight );
	SetFillHeightWeight( heightWeight.weight );
}
ListView::ListView( FillWidth widthWeight, int height )
	: Control(FillWidth::Value(), height ), m_sorted{ false }, m_fullRowSelect{ false }, m_oneClickActivate{ false }
{
	SetFillWidthWeight( widthWeight.weight );
}

ListView::ListView( int width, DefaultHeight )
	: Control(width, DefaultHeight::Value() ), m_sorted{ false }, m_fullRowSelect{ false }, m_oneClickActivate{ false }
{
}
ListView::ListView( int width, FillHeight heightWeight )
	: Control(width, FillHeight::Value() ), m_sorted{ false }, m_fullRowSelect{ false }, m_oneClickActivate{ false }
{
	SetFillHeightWeight( heightWeight.weight );
}
ListView::ListView( int width, int height )
	: Control(width, height ), m_sorted{ false }, m_fullRowSelect{ false }, m_oneClickActivate{ false }
{
}

ListView::~ListView()
{
}

DWORD ListView::GetWantedStyle() const
{
	DWORD style = Control::GetWantedStyle();
	style |= WS_TABSTOP;
	style |= LVS_REPORT | LVS_EDITLABELS | WS_BORDER | LVS_SINGLESEL;
	if ( m_sorted )
	{
		style |= LBS_SORT;
	}
	return style;
}

std::wstring ListView::GetType() const
{
	return WC_LISTVIEWW;
}

int ListView::GetDefaultWidth() const
{
	return 120;
}

int ListView::GetDefaultHeight() const
{
	return 240;
}

ListView* ListView::SetSorted( bool sorted )
{
	m_sorted = sorted;
	return this;
}	  

ListView* ListView::SetFullRowSelect( bool fullRowSelect )
{
	m_fullRowSelect = fullRowSelect;
	return this;
}

ListView* ListView::SetOneClickActivate( bool oneClickActivate )
{
	m_oneClickActivate = oneClickActivate;
	return this;
}	  

void ListView::Create( HWND parent )
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

	DWORD exStyle = LVS_EX_FULLROWSELECT;
	if ( m_oneClickActivate )
	{
		exStyle |= LVS_EX_ONECLICKACTIVATE;
	}
	ListView_SetExtendedListViewStyle( handle, exStyle );


	m_control.reset( new ui::ListView( parent, handle, this ) );
}
