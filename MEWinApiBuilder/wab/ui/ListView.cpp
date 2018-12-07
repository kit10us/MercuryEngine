// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#define _CRT_SECURE_NO_WARNINGS

#include <wab/ui/ListView.h>
#include <wab/ui/Edit.h>


using namespace ui;

void ListView::SetExStyle( DWORD style, bool enable )
{
	DWORD myStyle = ListView_GetExtendedListViewStyle( GetHandle() );

	if ( enable )
	{
		myStyle |= style;
	}
	else
	{
		myStyle &= ~style;
	}
	ListView_SetExtendedListViewStyle( GetHandle(), myStyle );
}

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

void ListView::SetGridLines( bool enable )				{ SetExStyle( LVS_EX_GRIDLINES				, enable ); }
void ListView::SetSubItemImages( bool enable )			{ SetExStyle( LVS_EX_SUBITEMIMAGES			, enable ); }
void ListView::SetCheckBoxes( bool enable )				{ SetExStyle( LVS_EX_CHECKBOXES				, enable ); }
void ListView::SetTrackSelect( bool enable )			{ SetExStyle( LVS_EX_TRACKSELECT			, enable ); }
void ListView::SetHeaderDrawDrop( bool enable )			{ SetExStyle( LVS_EX_HEADERDRAGDROP			, enable ); }
void ListView::SetFullRowSelect( bool enable )			{ SetExStyle( LVS_EX_FULLROWSELECT			, enable ); }
void ListView::SetOnClickActivate( bool enable )		{ SetExStyle( LVS_EX_ONECLICKACTIVATE		, enable ); }
void ListView::SetTwoClickActivate( bool enable )		{ SetExStyle( LVS_EX_TWOCLICKACTIVATE		, enable ); }
void ListView::SetFlatSB( bool enable )					{ SetExStyle( LVS_EX_FLATSB					, enable ); }
void ListView::SetRegional( bool enable )				{ SetExStyle( LVS_EX_REGIONAL				, enable ); }
void ListView::SetInfoTip( bool enable )				{ SetExStyle( LVS_EX_INFOTIP				, enable ); }
void ListView::SetUnderlineHot( bool enable )			{ SetExStyle( LVS_EX_UNDERLINEHOT			, enable ); }
void ListView::SetUnderlineCold( bool enable )			{ SetExStyle( LVS_EX_UNDERLINECOLD			, enable ); }
void ListView::SetMultiWorkAreas( bool enable )			{ SetExStyle( LVS_EX_MULTIWORKAREAS			, enable ); }
void ListView::SetLabelTop( bool enable )				{ SetExStyle( LVS_EX_LABELTIP				, enable ); }
void ListView::SetBorderSelect( bool enable )			{ SetExStyle( LVS_EX_BORDERSELECT			, enable ); }
void ListView::SetDoubleBuffer( bool enable )			{ SetExStyle( LVS_EX_DOUBLEBUFFER			, enable ); }
void ListView::SetHideLabels( bool enable )				{ SetExStyle( LVS_EX_HIDELABELS				, enable ); }
void ListView::SetSIngleRow( bool enable )				{ SetExStyle( LVS_EX_SINGLEROW				, enable ); }
void ListView::SetSnapToGrid( bool enable )				{ SetExStyle( LVS_EX_SNAPTOGRID				, enable ); }
void ListView::SetSimpleSelect( bool enable )			{ SetExStyle( LVS_EX_SIMPLESELECT			, enable ); }
void ListView::SetJustifyColumns( bool enable )			{ SetExStyle( LVS_EX_JUSTIFYCOLUMNS			, enable ); }
void ListView::SetTransparentBkgnd( bool enable )		{ SetExStyle( LVS_EX_TRANSPARENTBKGND		, enable ); }
void ListView::SetTransparentShadowText( bool enable )	{ SetExStyle( LVS_EX_TRANSPARENTSHADOWTEXT	, enable ); }
void ListView::SetAutoArrange( bool enable )			{ SetExStyle( LVS_EX_AUTOAUTOARRANGE		, enable ); }
void ListView::SetHeaderInAllViews( bool enable )		{ SetExStyle( LVS_EX_HEADERINALLVIEWS		, enable ); }
void ListView::SetAutoCheckSelect( bool enable )		{ SetExStyle( LVS_EX_AUTOCHECKSELECT		, enable ); }
void ListView::SetAutoSizeColumns( bool enable )		{ SetExStyle( LVS_EX_AUTOSIZECOLUMNS		, enable ); }
void ListView::SetColumnSnapPoints( bool enable )		{ SetExStyle( LVS_EX_COLUMNSNAPPOINTS		, enable ); }
void ListView::SetColumnOverFlow( bool enable )			{ SetExStyle( LVS_EX_COLUMNOVERFLOW			, enable ); }

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

IControl::ptr ListView::GetEditControl()
{
	HWND edit = ListView_GetEditControl( GetHandle() );
	HWND parent = GetParent( edit );
	return IControl::ptr( new Edit( parent, edit, 0 ) );
}

bool ListView::GetItemRect( int item, RECT* rect, LVIR code ) const
{															 
	return ListView_GetItemRect( GetHandle(), item, rect, (int)code ) ? true : false;
}

bool ListView::GetSubItemRect( int item, int subItem, RECT* rect, LVIR code ) const
{
	return ListView_GetSubItemRect( GetHandle(), item, subItem, (int)code, rect ) ? true : false;
}

bool ListView::SetColumnOrderArray( int count, int * array )
{
	return ListView_SetColumnOrderArray( GetHandle(), count, array ) ? true : false;
}