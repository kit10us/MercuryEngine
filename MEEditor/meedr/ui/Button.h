// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <meedr/ui/Control.h>
#include <string>

namespace meedr
{
	namespace ui
	{
		class Button : public Control
		{
		public:		   			
			Button( std::wstring text, DefaultWidth, DefaultHeight );
			Button( std::wstring text, DefaultWidth, FillHeight heightWeight );
			Button( std::wstring text, DefaultWidth, int height );
			
			Button( std::wstring text, FillWidth widthWeight, DefaultHeight );
			Button( std::wstring text, FillWidth widthWeight, FillHeight heigthWidth );
			Button( std::wstring text, FillWidth widthWeight, int height );

			Button( std::wstring text, SizeToContentWidth, DefaultHeight );
			Button( std::wstring text, SizeToContentWidth, FillHeight heigthWidth );
			Button( std::wstring text, SizeToContentWidth, int height );


			Button( std::wstring text, int width, DefaultHeight );
			Button( std::wstring text, int width, FillHeight heightWeight );
			Button( std::wstring text, int width, int height );

			virtual ~Button();

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