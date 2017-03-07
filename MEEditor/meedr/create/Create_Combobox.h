// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <meedr/create/Create_Control.h>
#include <string>

namespace meedr
{
	namespace create
	{
		class Combobox : public Control
		{
		public:		   			
			Combobox( DefaultWidth, DefaultHeight );
			Combobox( DefaultWidth, int height );
			
			Combobox( FillWidth widthWeight, DefaultHeight );
			Combobox( FillWidth widthWeight, int height );

			Combobox( int width, DefaultHeight );
			Combobox( int width, int height );

			virtual ~Combobox();

			int GetWantedHeight() const override;
			DWORD GetWantedStyle() const override;
			std::wstring GetType() const override;
			
			int GetDefaultWidth() const override;
			int GetDefaultHeight() const override;

		protected:
			void Create( HWND parent ) override;
		};
	}
}