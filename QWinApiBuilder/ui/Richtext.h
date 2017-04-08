// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <ui/Control.h>
#include <string>

namespace ui
{
	class Richtext : public Control
	{
	public:		   			
		Richtext( HWND parent, HWND handle, create::IControl * createControl );

		virtual ~Richtext();
	};
}