// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <wab/create/Create_Control.h>
#include <string>

namespace create
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

		Edit* SetAutoHScroll( bool autohscroll );
		bool GetAutoHScroll() const;

		Edit* SetAutoVScroll( bool autovscroll );
		bool GetAutoVScroll() const;

		Edit* SetMultiline( bool multiline );
		bool GetMultiline() const;

		Edit* SetReadonly( bool readonly );
		bool GetReadonly() const;

		DWORD GetWantedStyle() const override;
		std::wstring GetType() const override;

		int GetDefaultWidth() const override;
		int GetDefaultHeight() const override;

	protected:
		void Create( HWND parent ) override;
	};
}