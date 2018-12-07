// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <wab/create/Create_Menu.h>
#include <wab/ui/Menu.h>
#include <wab/ui/MenuItem.h>
#include <Commctrl.h>

#pragma comment( lib, "Comctl32" )

#include <functional>

using namespace create;

Menu::Menu( std::vector< MenuItem > menuItems )
	: m_createItems{ menuItems }
{
}

Menu::~Menu()
{
}	  					

HMENU Menu::Create()
{
	m_rootMenu.reset( new ui::Menu( CreateMenu(), this ) );

	std::function< bool( ui::Menu *, std::vector< MenuItem > & )> buildMenu
		= [ & ]( ui::Menu * menu, std::vector< MenuItem > & createItems ) -> bool
	{
		bool result{};

		for ( auto createItem : createItems )
		{
			// Has a sub-menu.
			if ( createItem.m_items.size() != 0 )
			{
				ui::Menu* newMenu( new ui::Menu( CreateMenu(), this ) );
				m_menuMap[ createItem.m_name ] = std::shared_ptr< ui::Menu >( newMenu );

				buildMenu( newMenu, createItem.m_items );
				result = AppendMenuW( menu->GetHandle(), MF_STRING | MF_POPUP, (UINT_PTR)newMenu->GetHandle(), createItem.m_text.c_str() ) ? true : false;
			}
			else if ( createItem.m_name.empty() )
			{
				result = AppendMenuW( menu->GetHandle(), MF_SEPARATOR, 0, 0 ) ? true : false;		
			}
			else
			{	
				int id = (int)m_itemList.size();
				auto menuItem = ui::MenuItem::ptr( new ui::MenuItem( createItem.m_name, id, createItem ) );
				m_itemList.push_back( menuItem );
				m_itemMap[ menuItem->GetName() ] = menuItem;
				result = AppendMenuW( menu->GetHandle(), MF_STRING, (UINT_PTR)id, createItem.m_text.c_str() ) ? true : false;
			}

			if ( !result ) return result;
		}
		return result;
	};
		
	buildMenu( m_rootMenu.get(), m_createItems );

	return m_rootMenu->GetHandle();
}

ui::Menu* Menu::GetMenu( HMENU handle ) const
{
	if ( !m_rootMenu )
	{
		return nullptr;
	}

	auto itr = m_menuList.find( handle );
	if ( itr == m_menuList.end() )
	{
		return nullptr;
	}
	else
	{
		return itr->second.get();
	}
}

ui::MenuItem* Menu::GetMenuItem( int id ) const
{
	if ( id >= (int)m_itemList.size() )
	{
		return nullptr;
	}

	return m_itemList[ id ].get();
}

ui::MenuItem* Menu::GetMenuItem( std::string name ) const
{
	auto itr = m_itemMap.find( name );
	if ( itr == m_itemMap.end() )
	{
		return nullptr;
	}

	return itr->second.get();
}
