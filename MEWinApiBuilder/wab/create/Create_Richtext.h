// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <wab/create/Create_Control.h>
#include <string>

namespace create
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

		Richtext* SetAutoHScroll( bool autohscroll );
		bool GetAutoHScroll() const;

		Richtext* SetAutoVScroll( bool autovscroll );
		bool GetAutoVScroll() const;

		Richtext* SetMultiline( bool multiline );
		bool GetMultiline() const;

		Richtext* SetReadonly( bool readonly );
		bool GetReadonly() const;		  

		DWORD GetWantedStyle() const override;
		std::wstring GetType() const override;

		int GetDefaultWidth() const override;
		int GetDefaultHeight() const override;

	protected:
		void Create( HWND parent ) override;
	};
}