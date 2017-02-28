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
			enum class Stack
			{
				Horizontal, Vertical
			};

			class StackPanel : public Container
			{
				void Create( int x, int y, int parentWidth, int parentHeight, HWND parent ) override;
			public:
				StackPanel( Stack direction, int width, int height, int padding = 3 );
				StackPanel( Stack direction, FillWidth, FillHeight, int padding = 3 );
				StackPanel( Stack direction, FillWidth, int height, int padding = 3 );
				StackPanel( Stack direction, int width, FillHeight, int padding = 3 );

				virtual ~StackPanel();

				Stack GetDirection();

				int GetWidth() override;
				int GetHeight() override;

				void AddChild( IControl * child );

				int GetPadding();
				void SetPadding( int padding );

			private:
				Stack m_direction;
				int m_width;
				int m_height;
				int m_padding;
				std::list< IControl * > m_children;
			};
		}
	}
}