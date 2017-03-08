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
	};
}