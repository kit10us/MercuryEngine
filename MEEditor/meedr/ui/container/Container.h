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
			protected:
				Container( int width, int height );

			public:
				virtual ~Container();

				virtual void AddChild( IControl * control ) = 0;
			};
		}
	}
}