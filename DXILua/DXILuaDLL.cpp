// DXILuaDLL.cpp : Defines the exported functions for the DLL application.
//

#include "DXILuaDLL.h"
#include <dxilua/DXILua.h>
#include <memory.h>
#include <dxi/win/DXILib.h>
#include <dxi/win/DXDevice.h>
#include <dxi/win/DXRenderer.h>

void Deleter( dxi::scripting::IScriptEngine * se )
{
	delete se;
}

DXILUADLL_API bool DXILoader( dxi::core::Game * game, const qxml::Document * document )
{
	dxi::win::DXRenderer * dxRenderer = (dxi::win::DXRenderer *)(void*)game->GetOS().GetRenderer( 0 );
	dxi::win::DX::SetDxDevice( dxRenderer->GetDxDevice() );

	game->AddScriptEngine( "lua", std::shared_ptr< dxi::scripting::IScriptEngine >( new dxilua::ScriptEngine( game ), Deleter ) );
	return true;
}

