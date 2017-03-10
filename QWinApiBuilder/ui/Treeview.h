// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <ui/Control.h>
#include <string>

namespace ui
{
	class Treeview : public Control
	{
	public:		   			
		Treeview( HWND parent, HWND handle, create::IControl * createControl );

		~Treeview();
	};
}