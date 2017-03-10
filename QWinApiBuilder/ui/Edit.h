// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <ui/Control.h>
#include <string>

namespace ui
{
	class Edit : public Control
	{
	public:		   			
		Edit( HWND parent, HWND handle, create::IControl * createControl );

		virtual ~Edit();

		enum class Event {
			SetFocus = EN_SETFOCUS,
			KillFocus = EN_KILLFOCUS,
			Change = EN_CHANGE,
			Update = EN_UPDATE,
			ErrSpace= EN_ERRSPACE,
			MaxText = EN_MAXTEXT,
			HScroll = EN_HSCROLL,
			VScroll = EN_VSCROLL
		};

	public: // WinApi functions...
		bool SetReadonly( bool readonly );
	};
}