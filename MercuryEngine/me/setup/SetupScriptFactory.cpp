// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <me/setup/SetupScriptFactory.h>
#include <me/setup/SetupScript.h>
#include <me/exception/FailedToCreate.h>

using namespace me;
using namespace setup;

SetupScriptFactory::SetupScriptFactory( game::Game * game )
	: m_game{ game }
{
}
	  
me::script::IScript::ptr SetupScriptFactory::Produce( unify::Path source, void * data )
{
	auto script = new SetupScript( m_game, source );

	return me::script::IScript::ptr( script );
}