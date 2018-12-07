// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <wab/ui/Combobox.h>

using namespace ui;

Combobox::Combobox( HWND parent, HWND handle, create::IControl * createControl  )
	: Control( parent, handle, createControl )
{
}

Combobox::~Combobox()
{
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
