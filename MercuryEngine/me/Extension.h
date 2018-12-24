// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <qxml/Element.h>
#include <unify/Path.h>

namespace me
{
	namespace game
	{
		class IGame;
	}

	class Extension
	{
	public:
		Extension( game::IGame * gameInstance, unify::Path source, const qxml::Element * element );
		virtual ~Extension();

	private:
		unify::Path m_source;
		void* m_moduleHandle;
	};
}