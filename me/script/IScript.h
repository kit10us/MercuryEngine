// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <rm/IResource.h>
#include <unify/Path.h>
#include <memory>

namespace me
{
	namespace script
	{
		class IScript : public rm::IResource
		{
		public:
			typedef std::shared_ptr< IScript > ptr;

			IScript();

			virtual ~IScript();
		};
	}
}