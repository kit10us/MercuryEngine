// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <wab/create/Create_Menu.h>

namespace ui
{
	class MenuItem
	{
	public:
		typedef std::shared_ptr< MenuItem > ptr;

		MenuItem( std::string name, int id, create::MenuItem createMenuItem );

		std::string GetName() const;

		int GetId() const;

	private:
		std::string m_name;
		int m_id;
		create::MenuItem m_createMenuItem;
	};
}