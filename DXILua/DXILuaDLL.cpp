// DXILuaDLL.cpp : Defines the exported functions for the DLL application.
//

#include "DXILuaDLL.h"
#include <dxilua/ScriptEngine.h>
#include <dxilua/component/GameComponent.h>
#include <memory.h>

void ScriptEngineDeleter( me::IScriptEngine * se )
{
	delete se;
}

extern "C" DXILUADLL_API bool MELoader( me::IGame * game, const qxml::Element * element);

DXILUADLL_API bool MELoader( me::IGame * game, const qxml::Element * element )
{
	// Add Script Engine...
	dxilua::ScriptEngine * scriptEngine = new dxilua::ScriptEngine( game );
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
		game->AddComponent( scriptEngine->LoadGameScript( source ) );
	}
	else
	{
		game->LogLine( "Unable to find startup script \"" + source.ToString() + "\"." );
	}

	return true;
}

