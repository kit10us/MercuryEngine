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
		public:		   			
			Static( std::wstring text, DefaultWidth, DefaultHeight, int id = 0 );
			Static( std::wstring text, DefaultWidth, FillHeight fillHeight, int id = 0 );
			Static( std::wstring text, DefaultWidth, int height, int id = 0 );

			Static( std::wstring text, FillWidth fillWidth, DefaultHeight, int id = 0 );
			Static( std::wstring text, FillWidth fillWidth, FillHeight fillHeight, int id = 0 );
			Static( std::wstring text, FillWidth fillWidth, int height, int id = 0 );

			Static( std::wstring text, SizeToContentWidth, DefaultHeight, int id = 0 );
			Static( std::wstring text, SizeToContentWidth, FillHeight fillHeight, int id = 0 );
			Static( std::wstring text, SizeToContentWidth, int height, int id = 0 );

			Static( std::wstring text, int width, DefaultHeight ight, int id = 0 );
			Static( std::wstring text, int width, FillHeight fillHeight, int id = 0 );
			Static( std::wstring text, int width, int height, int id = 0 );

			virtual ~Static();

			std::wstring GetText();

			int GetDefaultWidth() const override;
			int GetDefaultHeight() const override;

		protected:
			void Create( HWND parent ) override;
			void ComputePass1() override;

			std::wstring m_text;
		};
	}
}