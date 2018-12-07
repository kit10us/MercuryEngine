// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <wab/create/Create_Control.h>
#include <string>

namespace create
{
	class Treeview : public Control
	{
	public:		   			
		Treeview( DefaultWidth, FillHeight fillHeight );
		Treeview( DefaultWidth, int height );

		Treeview( FillWidth fillWidth, FillHeight fillHeight );
		Treeview( FillWidth fillHeight, int height );

		Treeview( int width, FillHeight fillHeight );
		Treeview( int width, int height );

		virtual ~Treeview();

		DWORD GetWantedStyle() const override;
		std::wstring GetType() const override;

		int GetDefaultWidth() const override;
		int GetDefaultHeight() const override;

	protected:
		void Create( HWND parent ) override;
	};
}