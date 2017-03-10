// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <ui/Control.h>
#include <string>

namespace ui
{
	class ListView : public Control
	{
	public:		   			
		ListView( HWND parent, HWND handle, create::IControl * createControl );

		virtual ~ListView();

		bool AddColumn( int column, std::string text, int width );
		bool AddColumn( int column, std::wstring text, int width );

		bool InsertItem( int column, int index, std::string text );
		bool InsertItem( int column, int index, std::wstring text );

		bool SetItemText( std::wstring text );

		int GetSelectedItem() const;

	public: // WinApi functions...	
		int DeleteAllItems();
		unsigned int GetSelectedColumn() const;
		int GetNextItem( int strat, UINT flags ) const;
	};
}