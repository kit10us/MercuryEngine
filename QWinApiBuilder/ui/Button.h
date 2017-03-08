// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <ui/Control.h>
#include <string>

namespace ui
{
	class Button : public Control
	{
	public:		   			
		Button( HWND parent, HWND handle, create::IControl * createControl );

		virtual ~Button();
	};
}