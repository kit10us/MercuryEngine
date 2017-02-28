// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <meedr/ui/container/Container.h>
#include <list>

namespace meedr
{
	namespace ui
	{
		namespace container
		{
			class Canvas : public Container
			{
				void Create( int x, int y, int parentWidth, int parentHeight, HWND parent ) override;
			public:
				Canvas( int width, int height );
				virtual ~Canvas();

				int GetWidth() override;
				int GetHeight() override;

				void AddChild( IControl * child );

			private:
				int m_width;
				int m_height;
				std::list< IControl * > m_children;
			};
		}
	}
}