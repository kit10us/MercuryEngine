// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <wab/ui/POD.h>

#include <map>
#include <stack>
#include <memory>
#include <string>

namespace ui
{
	class Window;
	class Menu;
	class MenuItem;
}

namespace create
{
	class MenuItem
	{
		friend class Menu;

	public:
		MenuItem( std::wstring text, std::string name )
			: m_text{ text }
			, m_name{ name }
		{
		}

		MenuItem( std::string textIsName )
			: m_text{ unify::Cast< std::wstring >( textIsName ) }
			, m_name{ textIsName }
		{
		}

		MenuItem( std::wstring text, std::initializer_list< MenuItem > items )
			: m_text{ text }
			, m_items{ items }
		{	
		}

		MenuItem( std::string text, std::initializer_list< MenuItem > items )
			: m_text{ unify::Cast< std::wstring >( text ) }
			, m_items{ items }
		{	
		}

	private:
		std::wstring m_text;
		std::string m_name;
		std::vector< MenuItem > m_items;
	};			

	class Seperator : public MenuItem
	{
	public:
		Seperator()
			: MenuItem( std::wstring(), std::string() )
		{
		}
	};



	class Menu
	{
		friend ui::Window;

	public:		   	
		typedef std::shared_ptr< Menu > ptr;

		Menu( std::vector< MenuItem > menuItems );

		virtual ~Menu();

		ui::Menu* GetMenu( HMENU handle ) const;
		ui::MenuItem* GetMenuItem( int id ) const;
		ui::MenuItem* GetMenuItem( std::string name ) const;

	protected:
		HMENU Create();

	private:		 
		std::vector< MenuItem > m_createItems;
		std::vector< std::shared_ptr< ui::MenuItem > > m_itemList;
		std::map< std::string, std::shared_ptr< ui::MenuItem >, unify::CaseInsensitiveLessThanTest > m_itemMap;


		std::shared_ptr< ui::Menu > m_rootMenu;

		std::map< std::string /* name */, std::shared_ptr< ui::Menu >, unify:: CaseInsensitiveLessThanTest> m_menuMap;
		std::map< HMENU, std::shared_ptr< ui::Menu > > m_menuList;
		
		//std::map< std::string /*name*/, int, unify::CaseInsensitiveLessThanTest > m_itemNameToId;
	};
}