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
			bool m_wantedAutoHScroll;
			bool m_wantedAutoVScroll;
			bool m_wantedMulitline;
			bool m_wantedReadonly;

		public:		   			
			Richtext( FillWidth fillWidth, FillHeight fillHeight );
			Richtext( FillWidth fillHeight, int height );

			Richtext( int width, FillHeight fillHeight );
			Richtext( int width, int height );

			virtual ~Richtext();

			Richtext* SetWantedAutoHScroll( bool autohscroll );
			bool GetWantedAutoHScroll() const;

			Richtext* SetWantedAutoVScroll( bool autovscroll );
			bool GetWantedAutoVScroll() const;

			Richtext* SetWantedMultiline( bool multiline );
			bool GetWantedMultiline() const;

			Richtext* SetWantedReadonly( bool readonly );
			bool GetWantedReadonly() const;		  

			DWORD GetWantedStyle() const override;
			std::wstring GetType() const override;

			int GetDefaultWidth() const override;
			int GetDefaultHeight() const override;

		protected:
			void Create( HWND parent ) override;
		};
	}
}