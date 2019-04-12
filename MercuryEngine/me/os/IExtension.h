// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <qxml/Element.h>
#include <unify/Path.h>
#include <memory>

namespace me
{
	namespace game
	{
		class IGame;
	}

	namespace os
	{
		class IExtension
		{
		public:
			typedef std::shared_ptr< IExtension > ptr;
			virtual ~IExtension(){}
		};
	}
}