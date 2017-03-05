// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <meedr/ui/Control.h>
#include <string>

namespace meedr
{
	namespace ui
	{
		class Edit : public Control
		{
			bool m_wantedAutoHScroll;
			bool m_wantedAutoVScroll;
			bool m_wantedMulitline;
			bool m_wantedReadonly;

		public:		   			
			Edit( FillWidth fillWidth, DefaultHeight );
			Edit( FillWidth fillWidth, FillHeight fillHeight );
			Edit( FillWidth fillWidth, int height );

			Edit( int width, DefaultHeight );
			Edit( int width, FillHeight fillHeight );
			Edit( int width, int height );

			virtual ~Edit();

			Edit* SetWantedAutoHScroll( bool autohscroll );
			bool GetWantedAutoHScroll() const;

			Edit* SetWantedAutoVScroll( bool autovscroll );
			bool GetWantedAutoVScroll() const;

			Edit* SetWantedMultiline( bool multiline );
			bool GetWantedMultiline() const;

			Edit* SetWantedReadonly( bool readonly );
			bool GetWantedReadonly() const;

			DWORD GetWantedStyle() const override;
			std::wstring GetType() const override;

			int GetDefaultWidth() const override;
			int GetDefaultHeight() const override;
		};
	}
}