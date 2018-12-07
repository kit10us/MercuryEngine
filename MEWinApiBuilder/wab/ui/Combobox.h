// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <wab/ui/Control.h>
#include <string>

namespace ui
{
	class Combobox : public Control
	{
	public:		   			
		Combobox( HWND parent, HWND handle, create::IControl * createControl );

		virtual ~Combobox();

		enum class Event
		{
			ErrSpace = CBN_ERRSPACE,
			SeChange = CBN_SELCHANGE,
			DblClick = CBN_DBLCLK,
			SetFocus = CBN_SETFOCUS,
			KillFocus = CBN_KILLFOCUS,
			EditChange = CBN_EDITCHANGE,
			EditUpdate = CBN_EDITUPDATE,
			DropDown = CBN_DROPDOWN,
			CloseUp = CBN_CLOSEUP,
			SelEndOk = CBN_SELENDOK,
			SelEndCancel = CBN_SELENDCANCEL
		};

	public: // WinApi functions...
		void ResetContent();
		void AddString( std::string text );
		void SetCurSel( int sel );
		int GetCurSel() const;
	};
}