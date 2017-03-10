// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <ui/MenuItem.h>
#include <create/Create_Menu.h>

using namespace ui;

MenuItem::MenuItem( std::string name, int id, create::MenuItem createMenuItem )
	: m_name{ name }
	, m_id{ id }
	, m_createMenuItem{ createMenuItem }
{
}

std::string MenuItem::GetName() const
{
	return m_name;
}

int MenuItem::GetId() const
{
	return m_id;
}
