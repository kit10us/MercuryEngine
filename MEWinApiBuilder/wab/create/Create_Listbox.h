// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <wab/create/Create_Control.h>
#include <string>

namespace create
{
	class Listbox : public Control
	{
	public:		   			
		Listbox( DefaultWidth, DefaultHeight );
		Listbox( DefaultWidth, FillHeight heightWeight );
		Listbox( DefaultWidth, int height );
			
		Listbox( FillWidth widthWeight, DefaultHeight );
		Listbox( FillWidth widthWeight, FillHeight heigthWidth );
		Listbox( FillWidth widthWeight, int height );

		Listbox( int width, DefaultHeight );
		Listbox( int width, FillHeight heightWeight );
		Listbox( int width, int height );

		virtual ~Listbox();

		DWORD GetWantedStyle() const override;
		std::wstring GetType() const override;

		int GetDefaultWidth() const override;
		int GetDefaultHeight() const override;

		Listbox * SetSorted( bool sorted );

	protected:
		void Create( HWND parent ) override;

		bool m_sorted;
	};
}