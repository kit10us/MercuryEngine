// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <qxml/Element.h>
#include <unify/Path.h>

namespace me
{
	namespace game
	{
		class IGame;

		class IExtension
		{
		public:
			virtual ~IExtension(){}

			virtual void Create( game::IGame * gameInstance, unify::Path source, const qxml::Element * element ) = 0;
		};
	}
}