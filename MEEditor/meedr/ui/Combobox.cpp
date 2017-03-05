// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <meedr/ui/Combobox.h>

using namespace meedr;
using namespace ui;

Combobox::Combobox( DefaultWidth, DefaultHeight )
	: Control(DefaultWidth::Value(), DefaultHeight::Value() )
{
}
Combobox::Combobox( DefaultWidth, int height )
	: Control(DefaultWidth::Value(), height )
{
}

Combobox::Combobox( FillWidth widthWeight, DefaultHeight )
	: Control(FillWidth::Value(), DefaultHeight::Value() )
{
	SetFillWidthWeight( widthWeight.weight );
}
Combobox::Combobox( FillWidth widthWeight, int height )
	: Control(FillWidth::Value(), height )
{
	SetFillWidthWeight( widthWeight.weight );
}

Combobox::Combobox( int width, DefaultHeight )
	: Control(width, DefaultHeight::Value() )
{
}
Combobox::Combobox( int width, int height )
	: Control(width, height )
{
}

Combobox::~Combobox()
{
}

DWORD Combobox::GetWantedStyle() const
{
	return Control::GetWantedStyle() | CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_TABSTOP;
}

std::wstring Combobox::GetType() const
{
	return L"Combobox";
}

int Combobox::GetWantedHeight() const
{
	return 24;
}

void Combobox::Create( HWND parent )
{
	m_parentHandle = parent;
	m_handle = CreateWindowW(
		GetType().c_str(),
		GetWantedText().c_str(),
		GetWantedStyle(),
		GetActualX(),
		GetActualY(),
		GetActualWidth(),
		( ui::DefaultHeight() == m_wantedHeight ) ? GetDefaultHeight() : m_wantedHeight, 
		parent,
		(HMENU)GetID(),
		0,
		0
	);
}	 

int Combobox::GetDefaultWidth() const
{
	return 120;
}

int Combobox::GetDefaultHeight() const
{
	return 240;
}

void Combobox::ResetContent()
{
	SendMessageA( GetHandle(), CB_RESETCONTENT, 0, 0 );
}

void Combobox::AddString( std::string text )
{
	SendMessageA( GetHandle(), CB_ADDSTRING, 0, (LPARAM)text.c_str() );
}

void Combobox::SetCurSel( int sel )
{
	SendMessageA( GetHandle(), CB_SETCURSEL, sel, 0 );
}

int Combobox::GetCurSel() const
{
	return SendMessageA( GetHandle(), CB_GETCURSEL, 0, 0 );
}
