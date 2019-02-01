// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/game/IExtension.h>
#include <qxml/Element.h>
#include <unify/Path.h>

namespace me
{
	class Extension : public me::game::IExtension
	{
	public:
		Extension();
		
		void Create( game::IGame * gameInstance, unify::Path source, const qxml::Element * element ) override;
		virtual ~Extension();

	private:
		unify::Path m_source;
		void* m_moduleHandle;
	};
}