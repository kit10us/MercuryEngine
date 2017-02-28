// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <meedr/ui/Control.h>
#include <string>

namespace meedr
{
	namespace ui
	{
		class Combobox : public Control
		{
			void Create( int x, int y, int width, int height, HWND parent ) override;
		public:		   			
			Combobox( DefaultWidth, DefaultHeight, int id );
			Combobox( DefaultWidth, int height, int id );
			
			Combobox( FillWidth widthWeight, DefaultHeight, int id );
			Combobox( FillWidth widthWeight, int height, int id );

			Combobox( int width, DefaultHeight, int id );
			Combobox( int width, int height, int id );

			virtual ~Combobox();

			int GetWidth() override;
			int GetHeight() override;

			int GetDefaultWidth() override;
			int GetDefaultHeight() override;

		private:
			int m_width;
			int m_height;
		};
	}
}