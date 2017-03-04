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
			Richtext( FillWidth fillWidth, FillHeight fillHeight );
			Richtext( FillWidth fillHeight, int height );

			Richtext( int width, FillHeight fillHeight );
			Richtext( int width, int height );

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