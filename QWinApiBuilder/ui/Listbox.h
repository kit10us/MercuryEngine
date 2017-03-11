// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <ui/Control.h>
#include <string>

namespace ui
{
	class Listbox : public Control
	{
	public:
		Listbox( HWND parent, HWND handle, create::IControl * createControl );

		virtual ~Listbox();
		  
		enum class Event
		{
			ErrSpace = LBN_ERRSPACE,
			SelChange = LBN_SELCHANGE,
			DblClick = LBN_DBLCLK,
			SelCancel = LBN_SELCANCEL,
			SetFocus = LBN_SETFOCUS,
			KillFocus = LBN_KILLFOCUS
		};

	public: // WinApi functions...	
		int AddFile( std::string filename );
		int AddString( std::string text );
		int DeleteString( int index );
		int GetCurSel();

		int ResetContent();
		int SetCurSel( int sel );
	};
}