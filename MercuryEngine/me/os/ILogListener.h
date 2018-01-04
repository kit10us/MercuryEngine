// Copyright (c) 2003 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <string>

namespace me
{
	class ILogListener
	{
	public:
		~ILogListener() {}

		virtual void Log( std::string text ) = 0;
	};
}