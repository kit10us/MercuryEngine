/*
 * QXML Interop Library
 * https://github.com/kit10us/qxmlinterop
 * Copyright (c) 2002, Kit10 Studios LLC
 *
 * This file is part of QXML Interop Library (a.k.a. QXMLIO)
 *
 * QXMLIO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * QXMLIO is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QXMLIO.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <me/game/Game.h>
#include <me/debug/ErrorLevel.h>
#include <xmlio/DocumentSourceFactory.h>
#include <io/IDocument.h>

using namespace xmlio;
using namespace me;

extern "C" __declspec(dllexport) bool MELoader( me::game::IGame * gameInstance, const qxml::Element * element );

__declspec(dllexport) bool MELoader( me::game::IGame * game, const qxml::Element * element )
{
	using namespace me;

	auto gameDerived = dynamic_cast<me::game::Game*>(game);

	auto debug = game->Debug();
	auto block{ debug->GetLogger()->CreateBlock( "MELoader \"QXMLIO\"" ) };

	// Setup XML source factory
	DocumentSourceFactory * factory = new DocumentSourceFactory( game );

	auto manager = gameDerived->GetManager<io::IDocument>();
	manager->AddFactory( "xml", DocumentSourceFactory::ptr( factory ) );
	return true;
}

