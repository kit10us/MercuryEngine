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
			Static( std::wstring text, DefaultWidth, DefaultHeight );
			Static( std::wstring text, DefaultWidth, FillHeight fillHeight );
			Static( std::wstring text, DefaultWidth, int height );

			Static( std::wstring text, FillWidth fillWidth, DefaultHeight );
			Static( std::wstring text, FillWidth fillWidth, FillHeight fillHeight );
			Static( std::wstring text, FillWidth fillWidth, int height );

			Static( std::wstring text, SizeToContentWidth, DefaultHeight );
			Static( std::wstring text, SizeToContentWidth, FillHeight fillHeight );
			Static( std::wstring text, SizeToContentWidth, int height );

			Static( std::wstring text, int width, DefaultHeight ight );
			Static( std::wstring text, int width, FillHeight fillHeight );
			Static( std::wstring text, int width, int height );

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