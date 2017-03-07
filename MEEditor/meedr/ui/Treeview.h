// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <meedr/ui/Control.h>
#include <string>

namespace meedr
{
	namespace ui
	{
		class Treeview : public Control
		{
		public:		   			
			Treeview( HWND parent, HWND handle, create::IControl * createControl );
			~Treeview();
		};
	}
}