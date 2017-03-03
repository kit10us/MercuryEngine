// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <meedr/ui/Control.h>
#include <string>

namespace meedr
{
	namespace ui
	{
		class Richtext : public Control
		{
		public:		   			
			Richtext( FillWidth, FillHeight, int id = 0 );
			Richtext( FillWidth, int height, int id = 0 );

			Richtext( int width, FillHeight height, int id = 0 );
			Richtext( int width, int height, int id = 0 );

			virtual ~Richtext();

			std::wstring GetText();

			int GetDefaultWidth() const override;
			int GetDefaultHeight() const override;

		protected:
			void Create( HWND parent ) override;

			std::wstring m_text;
		};
	}
}