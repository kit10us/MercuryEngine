// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <wab/ui/Listbox.h>

using namespace ui;

Listbox::Listbox( HWND parent, HWND handle, create::IControl * createControl )
	: Control( parent, handle, createControl )
{
}

Listbox::~Listbox()
{
}

int Listbox::AddFile( std::string filename )
{
	return SendMessageA( GetHandle(), LB_ADDFILE, 0, (LPARAM)filename.c_str() );
}

int Listbox::AddString( std::string text )
{
	return SendMessageA( GetHandle(), LB_ADDSTRING, 0, (LPARAM)text.c_str() );
}

int Listbox::DeleteString( int index )
{
	return SendMessageA( GetHandle(), LB_DELETESTRING, index, 0 );
}

int Listbox::GetCurSel()
{
	return SendMessageA( GetHandle(), LB_GETCURSEL, 0, 0 );
}

int Listbox::ResetContent()
{
	return SendMessageA( GetHandle(), LB_RESETCONTENT, 0, 0 );
}

int Listbox::SetCurSel( int sel )
{
	return SendMessageA( GetHandle(), LB_SETCURSEL, sel, 0 );
}
