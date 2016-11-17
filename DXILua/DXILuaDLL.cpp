// DXILuaDLL.cpp : Defines the exported functions for the DLL application.
//

#include "DXILuaDLL.h"
#include <dxilua/ScriptEngine.h>
#include <memory.h>

void Deleter( me::IScriptEngine * se )
{
	delete se;
}

extern "C" DXILUADLL_API bool DXILoader( me::IGame * game, const qxml::Document * doc );

DXILUADLL_API bool DXILoader( me::IGame * game, const qxml::Document * document )
{
	//game->AddScriptEngine( "lua", std::shared_ptr< dxi::scripting::IScriptEngine >( new dxilua::ScriptEngine( game ), Deleter ) );
	game->AddComponent( me::IGameComponent::ptr( new dxilua::ScriptEngine( game ), Deleter ) );
	return true;
}

