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
			Button( std::wstring text, DefaultWidth, DefaultHeight, int id );
			Button( std::wstring text, DefaultWidth, FillHeight heightWeight, int id );
			Button( std::wstring text, DefaultWidth, int height, int id );
			
			Button( std::wstring text, FillWidth widthWeight, DefaultHeight, int id );
			Button( std::wstring text, FillWidth widthWeight, FillHeight heigthWidth, int id );
			Button( std::wstring text, FillWidth widthWeight, int height, int id );

			Button( std::wstring text, int width, DefaultHeight, int id );
			Button( std::wstring text, int width, FillHeight heightWeight, int id );
			Button( std::wstring text, int width, int height, int id );

			virtual ~Button();

			std::wstring GetText();

			int GetDefaultWidth() const override;
			int GetDefaultHeight() const override;

		protected:
			void Create( HWND parent ) override;

			std::wstring m_text;
		};
	}
}