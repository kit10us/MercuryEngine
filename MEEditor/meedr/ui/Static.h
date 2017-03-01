// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <meedr/ui/Control.h>
#include <string>

namespace meedr
{
	namespace ui
	{
		class Static : public Control
		{
			void Create( int x, int y, int width, int height, HWND parent ) override;
		public:		   			
			Static( std::wstring text, DefaultWidth, DefaultHeight, int id = 0 );
			Static( std::wstring text, DefaultWidth, FillHeight, int id = 0 );
			Static( std::wstring text, DefaultWidth, int height, int id = 0 );

			Static( std::wstring text, FillWidth, DefaultHeight, int id = 0 );
			Static( std::wstring text, FillWidth, FillHeight, int id = 0 );
			Static( std::wstring text, FillWidth, int height, int id = 0 );

			Static( std::wstring text, SizeToContentWidth, DefaultHeight, int id = 0 );
			Static( std::wstring text, SizeToContentWidth, FillHeight, int id = 0 );
			Static( std::wstring text, SizeToContentWidth, int height, int id = 0 );

			Static( std::wstring text, int width, DefaultHeight height, int id = 0 );
			Static( std::wstring text, int width, FillHeight height, int id = 0 );
			Static( std::wstring text, int width, int height, int id = 0 );

			virtual ~Static();

			int GetWidth() override;
			int GetHeight() override;

			std::wstring GetText();

			int GetDefaultWidth() override;
			int GetDefaultHeight() override;

		private:
			int m_width;
			int m_height;
			std::wstring m_text;
		};
	}
}