// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <wab/create/Create_Combobox.h>
#include <wab/ui/Combobox.h>

using namespace create;

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
	HWND handle = CreateWindowW(
		GetType().c_str(),
		GetWantedText().c_str(),
		GetWantedStyle(),
		GetActualX(),
		GetActualY(),
		GetActualWidth(),
		( create::DefaultHeight() == m_wantedHeight ) ? GetDefaultHeight() : m_wantedHeight, 
		parent,
		(HMENU)GetID(),
		0,
		0
	);
	m_control.reset( new ui::Combobox( parent, handle, this ) );
}	 

int Combobox::GetDefaultWidth() const
{
	return 120;
}

int Combobox::GetDefaultHeight() const
{
	return 240;
}
