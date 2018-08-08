// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <ui/Control.h>
#include <string>

namespace ui
{
	class Static : public Control
	{
	public:		   			
		Static( HWND parent, HWND handle, create::IControl * createControl );

		virtual ~Static();
	};
}