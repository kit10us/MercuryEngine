// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <wab/create/Create_Control.h>
#include <string>

namespace create
{
	class ListView : public Control
	{
	public:		   			
		ListView( DefaultWidth, DefaultHeight );
		ListView( DefaultWidth, FillHeight heightWeight );
		ListView( DefaultWidth, int height );
			
		ListView( FillWidth widthWeight, DefaultHeight );
		ListView( FillWidth widthWeight, FillHeight heigthWidth );
		ListView( FillWidth widthWeight, int height );

		ListView( int width, DefaultHeight );
		ListView( int width, FillHeight heightWeight );
		ListView( int width, int height );

		virtual ~ListView();

		DWORD GetWantedStyle() const override;
		std::wstring GetType() const override;

		int GetDefaultWidth() const override;
		int GetDefaultHeight() const override;

		ListView * SetSorted( bool sorted );
		ListView * SetFullRowSelect( bool fullRowSelect );
		ListView * SetOneClickActivate( bool oneClickActivate );

	protected:
		void Create( HWND parent ) override;

		bool m_sorted;
		bool m_fullRowSelect;
		bool m_oneClickActivate;
	};
}