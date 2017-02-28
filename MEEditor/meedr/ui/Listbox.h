// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <meedr/ui/Control.h>
#include <string>

namespace meedr
{
	namespace ui
	{
		class Listbox : public Control
		{
			void Create( int x, int y, int width, int height, HWND parent ) override;
		public:		   			
			Listbox( DefaultWidth, DefaultHeight, int id );
			Listbox( DefaultWidth, FillHeight heightWeight, int id );
			Listbox( DefaultWidth, int height, int id );
			
			Listbox( FillWidth widthWeight, DefaultHeight, int id );
			Listbox( FillWidth widthWeight, FillHeight heigthWidth, int id );
			Listbox( FillWidth widthWeight, int height, int id );

			Listbox( int width, DefaultHeight, int id );
			Listbox( int width, FillHeight heightWeight, int id );
			Listbox( int width, int height, int id );

			virtual ~Listbox();

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