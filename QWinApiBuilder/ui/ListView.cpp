// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#define _CRT_SECURE_NO_WARNINGS

#include <ui/ListView.h>
#include <CommCtrl.h>


using namespace ui;

ListView::ListView( HWND parent, HWND handle, create::IControl * createControl )
	: Control( parent, handle, createControl )
{
}

ListView::~ListView()
{
}

bool ListView::AddColumn( int column, std::string text, int width )
{
	LVCOLUMNA lvc{};

	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	// Add the columns.
	CHAR szText[ 256 ];
	strcpy( szText, text.c_str() );
	
	lvc.iSubItem = column;
	lvc.pszText = szText;
	lvc.cx = width;

	if ( column < 2 )
	{
		lvc.fmt = LVCFMT_LEFT;  // Left-aligned column.
	}
	else
	{
		lvc.fmt = LVCFMT_RIGHT; // Right-aligned column.
	}

	// Insert the columns into the list view.
	if ( ListView_InsertColumn( GetHandle(), column, &lvc ) == -1 )
	{
		return false;
	}
	return true;
}

bool ListView::AddColumn( int column, std::wstring text, int width )
{
	LVCOLUMNW lvc{};

	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	WCHAR szText[ 256 ];
	wcscpy( szText, text.c_str() );
	
	lvc.iSubItem = column;
	lvc.pszText = szText;
	lvc.cx = width;

	if ( column < 2 )
	{
		lvc.fmt = LVCFMT_LEFT;  // Left-aligned column.
	}
	else
	{
		lvc.fmt = LVCFMT_RIGHT; // Right-aligned column.
	}

	// Insert the columns into the list view.
	if ( ListView_InsertColumn( GetHandle(), column, &lvc ) == -1 )
	{
		return false;
	}
	return false;
}

bool ListView::InsertItem( int column, int index, std::string text )
{
	if ( column == 0 )
	{
		LVITEMA lvI{};

		// Initialize LVITEM members that are common to all items.
		CHAR szText[ 256 ];
		strcpy( szText, text.c_str() );

		lvI.pszText = szText; // Sends an LVN_GETDISPINFO message.
		lvI.mask = LVIF_TEXT /*| LVIF_IMAGE |LVIF_STATE*/;
		lvI.stateMask = 0;
		lvI.iSubItem = 0;
		lvI.state = 0;
		lvI.cColumns = column;

		lvI.iItem = index;
		//lvI.iImage = index;

		// Insert items into the list.
		if ( ListView_InsertItem( GetHandle(), &lvI ) == -1 )
		{
			return false;
		}							 
	}
	else
	{
		CHAR szText[ 256 ];
		strcpy( szText, text.c_str() );
		LV_ITEMA _macro_lvi;
		_macro_lvi.iSubItem = column;
		_macro_lvi.pszText = szText;
		::SendMessageA( GetHandle(), LVM_SETITEMTEXT, (WPARAM)(index), (LPARAM)(LV_ITEM *)&_macro_lvi );
	}

    return true;
}

bool ListView::InsertItem( int column, int index, std::wstring text )
{
	int result = 0;
	if ( column == 0 )
	{
		LVITEMW lvI{};

		// Initialize LVITEM members that are common to all items.
		WCHAR szText[ 256 ];
		wcscpy( szText, text.c_str() );

		lvI.pszText = szText; // Sends an LVN_GETDISPINFO message.
		lvI.mask = LVIF_TEXT | LVIF_STATE;
		lvI.stateMask = 0;
		lvI.iSubItem = 0;
		lvI.state = 0;
		lvI.cColumns = column;

		lvI.iItem = index;
		//lvI.iImage = index;

		// Insert items into the list.
		if ( ListView_InsertItem( GetHandle(), &lvI ) == -1 )
		{
			return false;
		}

		ListView_Update( GetHandle(), index );
	}
	else
	{
		WCHAR szText[ 256 ];
		wcscpy( szText, text.c_str() );
		LV_ITEMW item{};
		item.mask = LVIF_TEXT;
		item.iSubItem = column;
		item.pszText = szText;

		if ( ! ::SendMessageW( GetHandle(), LVM_SETITEMTEXT, (WPARAM)(index), (LPARAM)&item ) )
		{
			return false;
		}
	}

    return true;
}

bool ListView::SetItemText( std::wstring text )
{
	WCHAR szText[ 256 ];
	wcscpy( szText, text.c_str() );
	ListView_SetItemText( GetHandle(), 0, 1, szText );
	return true;
}

int ListView::GetSelectedItem() const
{
	return GetNextItem( -1, LVNI_SELECTED );
}

int ListView::DeleteAllItems()
{
	return SendMessageA( GetHandle(), LVM_DELETEALLITEMS, 0, 0 );
}

unsigned int ListView::GetSelectedColumn() const
{
	return ListView_GetSelectedColumn( GetHandle() );
}

int ListView::GetNextItem( int start, UINT flags ) const
{		   
	return ListView_GetNextItem( GetHandle(), start, flags );
}