// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <wab/ui/POD.h>
#include <wab/ui/IControl.h>

namespace ui
{
	namespace message
	{
		struct Notify
		{
			IControl* control;
			unsigned int code;
			LPARAM lParam;

			bool IsFor( std::string _name ) const
			{
				return unify::StringIs( control->GetName(), _name );
			}
		};
	}
}