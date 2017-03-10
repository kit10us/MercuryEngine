// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <ui/POD.h>

namespace ui
{
	namespace message
	{
		struct Params
		{
			Params() : wParam{ 0 }, lParam{ 0 } {}
			Params( WPARAM w, LPARAM l ) : wParam{ w }, lParam{ l } {}
			WPARAM wParam;
			LPARAM lParam;
		};
	}
}