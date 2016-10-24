// DXILuaDLL.cpp : Defines the exported functions for the DLL application.
//

#include "DXILuaDLL.h"
#include <dxilua/DXILua.h>
#include <memory.h>
#include <dxi/win/DXILib.h>

void Deleter( dxi::scripting::IScriptEngine * se )
{
	delete se;
}

extern "C" DXILUADLL_API bool DXILoader( dxi::core::IGame * game, const qxml::Document * doc );

DXILUADLL_API bool DXILoader( dxi::core::IGame * game, const qxml::Document * document )
{
	game->AddScriptEngine( "lua", std::shared_ptr< dxi::scripting::IScriptEngine >( new dxilua::ScriptEngine( game ), Deleter ) );
	return true;
}

