// Copyright (c) 2002 - 2018, Kit10 Studios LLC
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
	  
me::script::IScript::ptr SetupScriptFactory::Produce( unify::Path source, unify::Parameters parameters )
{
	auto script = new SetupScript( m_game, source );

	return me::script::IScript::ptr( script );
}

me::script::IScript::ptr SetupScriptFactory::Produce( unify::Parameters parameters )
{
	throw me::exception::FailedToCreate( "Attempted to create a setup script from parameters." );
}
