// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <ui/POD.h>
#include <ui/IControl.h>

namespace ui
{
	namespace message
	{
		struct ControlCommand
		{
			IControl* control;
			int code;

			bool IsFor( std::string _name ) const
			{
				return unify::StringIs( control->GetName(), _name );
			}
		};
	}
}