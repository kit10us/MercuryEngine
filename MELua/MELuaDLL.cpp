// MELuaDLL.cpp : Defines the exported functions for the DLL application.
//

#include "MELuaDLL.h"
#include <melua/ScriptEngine.h>
#include <melua/component/GameComponent.h>
#include <memory.h>

void ScriptEngineDeleter( me::IGameComponent * se )
{
	delete se;
}

extern "C" MELUADLL_API bool MELoader( me::IGame * game, const qxml::Element * element);

MELUADLL_API bool MELoader( me::IGame * game, const qxml::Element * element )
{
	// Add Script Engine...
	melua::ScriptEngine * scriptEngine = new melua::ScriptEngine( game->GetOS() );
	game->AddComponent( me::IGameComponent::ptr( scriptEngine, ScriptEngineDeleter ) );
				  
	// Automatically add "game.lua" GameComponent...
	unify::Path source( "game.lua" );
	if ( element->HasElements( "startup" ) )
	{
		source = element->GetElement( "startup" )->GetText();
	}

	source = unify::Path( game->GetOS()->GetAssetPaths().FindAsset( source ) );

	if ( source.Exists() )
	{
		game->LogLine( "Loading startup script \"" + source.ToString() + "\"." );
		auto component = scriptEngine->LoadGameScript( source );
		game->AddComponent( component );
	}
	else
	{
		game->LogLine( "Unable to find startup script \"" + source.ToString() + "\"." );
	}

	return true;
}

