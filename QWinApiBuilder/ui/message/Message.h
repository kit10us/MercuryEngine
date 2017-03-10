// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <ui/POD.h>

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