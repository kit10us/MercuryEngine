// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <qxml/Element.h>
#include <unify/Path.h>

namespace me
{
	class IGame;

	class Extension
	{
	public:
		Extension( unify::Path source );
		virtual ~Extension();

		bool Load( IGame * game, const qxml::Element * element );
		void Free();

	private:
		unify::Path m_source;

		void* m_moduleHandle;
	};
}