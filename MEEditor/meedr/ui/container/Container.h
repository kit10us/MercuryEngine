// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <meedr/ui/Control.h>

namespace meedr
{
	namespace ui
	{
		namespace container
		{
			class Container : public Control
			{
			public:
				Container( int id );
				virtual ~Container();

				virtual void AddChild( IControl * control ) = 0;
			};
		}
	}
}