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
		public:		   			
			Listbox( DefaultWidth, DefaultHeight );
			Listbox( DefaultWidth, FillHeight heightWeight );
			Listbox( DefaultWidth, int height );
			
			Listbox( FillWidth widthWeight, DefaultHeight );
			Listbox( FillWidth widthWeight, FillHeight heigthWidth );
			Listbox( FillWidth widthWeight, int height );

			Listbox( int width, DefaultHeight );
			Listbox( int width, FillHeight heightWeight );
			Listbox( int width, int height );

			virtual ~Listbox();

			int GetDefaultWidth() const override;
			int GetDefaultHeight() const override;

			Listbox * SetSorted( bool sorted );

		protected:
			void Create( HWND parent ) override;

			bool m_sorted;
		};
	}
}