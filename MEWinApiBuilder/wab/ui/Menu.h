// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <wab/create/Create_Menu.h>

namespace ui
{
	class Menu
	{
		friend create::Menu;

	public:		   			
		Menu( HMENU handle, create::Menu * menu );

		~Menu();

		HMENU GetHandle() const;
		 
		ui::Menu* GetMenu( HMENU handle );
		ui::MenuItem* GetMenuItem( int id ) const;
		ui::MenuItem* GetMenuItem( std::string name ) const;

	private:
		HMENU m_handle;
		create::Menu * m_createMenu;
	};
}