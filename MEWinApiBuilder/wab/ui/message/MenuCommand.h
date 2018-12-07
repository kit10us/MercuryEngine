// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <wab/ui/POD.h>
#include <wab/ui/MenuItem.h>

namespace ui
{
	namespace message
	{
		struct MenuCommand
		{
			ui::MenuItem * item;

			bool IsFor( std::string _name ) const
			{
				return unify::StringIs( item->GetName(), _name );
			}
		};
	}
}