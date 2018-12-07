// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <wab/ui/Control.h>
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