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

DXILUADLL_API bool DXILoader( dxi::core::Game * game, const qxml::Document * document )
{
	game->AddScriptEngine( "lua", std::shared_ptr< dxi::scripting::IScriptEngine >( new dxilua::ScriptEngine( game ), Deleter ) );
	return true;
}

