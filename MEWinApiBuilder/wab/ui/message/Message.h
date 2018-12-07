// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <wab/ui/POD.h>

namespace ui
{
	namespace message
	{
		struct Message
		{
			HWND handle;
			UINT message; 
			WPARAM wParam;
			LPARAM lParam;
		};
	}
}