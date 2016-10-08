// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <memory>

namespace dxi
{
	namespace scripting
	{
		class IModule
		{
		public:
			typedef std::shared_ptr< IModule > ptr;

			virtual ~IModule() {}

			virtual void OnStart() = 0;
			virtual void OnUpdate() = 0;
		};
	}
}