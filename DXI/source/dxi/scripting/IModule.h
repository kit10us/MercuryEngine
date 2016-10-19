// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/scene/Object.h>
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

			virtual void OnInit() = 0;
			virtual void OnStart() = 0;
			virtual void OnUpdate() = 0;
			virtual void OnSuspend() = 0;
			virtual void OnResume() = 0;
		};
	}
}