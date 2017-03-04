// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <meedr/ui/Control.h>
#include <string>

namespace meedr
{
	namespace ui
	{
		class Treeview : public Control
		{
		public:		   			
			Treeview( FillWidth fillWidth, FillHeight fillHeight );
			Treeview( FillWidth fillHeight, int height );

			Treeview( int width, FillHeight fillHeight );
			Treeview( int width, int height );

			virtual ~Treeview();

			int GetDefaultWidth() const override;
			int GetDefaultHeight() const override;

		protected:
			void Create( HWND parent ) override;
		};
	}
}