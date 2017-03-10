// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <ui/Control.h>
#include <CommCtrl.h>
#include <string>

namespace ui
{
	class Button : public Control
	{
	public:		   			
		Button( HWND parent, HWND handle, create::IControl * createControl );

		virtual ~Button();

		enum class Event : unsigned int 
		{
			HotItemChange = BCN_HOTITEMCHANGE,
			Clicked = BN_CLICKED,
			SoubleClicked = BN_DOUBLECLICKED,
			Disable = BN_DISABLE,
			Hilite = BN_HILITE,
			KillFocus = BN_KILLFOCUS,
			Paint = BN_PAINT,
			SetFocus = BN_SETFOCUS,
			Unhilite = BN_UNHILITE
		};
	};
}