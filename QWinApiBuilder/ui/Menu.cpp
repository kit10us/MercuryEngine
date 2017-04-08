// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <ui/Menu.h>

#pragma comment( lib, "Comctl32" )

using namespace ui;

Menu::Menu( HMENU handle, create::Menu * menu )
	: m_handle( handle )
	, m_createMenu( menu )
{
}

Menu::~Menu()
{
}

HMENU Menu::GetHandle() const
{
	return m_handle;
}

Menu* Menu::GetMenu( HMENU handle )
{
	if ( handle == m_handle )
	{
		return this;
	}

	if ( ! m_createMenu )
	{
		return nullptr;
	}

	return m_createMenu->GetMenu( handle );
}

ui::MenuItem* Menu::GetMenuItem( int id ) const
{
	if ( ! m_createMenu )
	{
		return nullptr;
	}

	return m_createMenu->GetMenuItem( id );
}

ui::MenuItem* Menu::GetMenuItem( std::string name ) const
{
	if ( ! m_createMenu )
	{
		return nullptr;
	}

	return m_createMenu->GetMenuItem( name );
}
