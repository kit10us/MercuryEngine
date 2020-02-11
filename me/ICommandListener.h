// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <string>
#include <memory>

namespace me
{
	class ICommandListener
	{
	public:
		typedef std::shared_ptr< ICommandListener > ptr;

		virtual ~ICommandListener() {}

		virtual std::string SendCommand( size_t id, std::string extra ) = 0;
	};
}

