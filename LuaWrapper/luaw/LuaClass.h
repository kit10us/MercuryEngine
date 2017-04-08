// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <luaw/LuaWrapper.h>
#include <string>

namespace luaw
{
	class LuaClass
	{
	public:	 
		LuaClass( std::string name );

	private:
		std::string m_name;
	};
}
