// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#include <melua/ScriptFactory.h>
#include <melua/Script.h>
#include <me/exception/FailedToCreate.h>
#include <me/game/Game.h>

using namespace melua;

ScriptFactory::ScriptFactory( ScriptEngine * se )
	: m_se( se )
{
}
	  
me::script::IScript::ptr ScriptFactory::Produce( unify::Path source, void * data )
{
	auto game = dynamic_cast< me::game::Game* >( m_se->GetGame() );

	auto script = new Script( m_se->GetState(), source );

	return me::script::IScript::ptr( script );
}

me::script::IScript::ptr ScriptFactory::Produce( void * data )
{
	throw me::exception::FailedToCreate( "Attempted to create script from raw data." );
}

me::script::IScript::ptr ScriptFactory::Produce( unify::Parameters parameters )
{
	throw me::exception::FailedToCreate( "Attempted to create script from parameters." );
}