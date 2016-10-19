// DXILuaDLL.cpp : Defines the exported functions for the DLL application.
//

#include "DXILuaPhysXExt.h"
#include "luaphysx/ExportTest.h"
#include <dxilua/DXILua.h>
#include <memory.h>
#include <dxi/win/DXILib.h>

#include <luaphysx/ExportPxMaterial.h>
#include <luaphysx/ExportShape.h>

#pragma comment( lib, "lua53" )
#pragma comment( lib, "../DXI/tests_data/DXILua.lib" )
#pragma comment( lib, "DXILuaLib" )


extern "C"
int Debug_Bark( lua_State * state )
{
	int args = lua_gettop( state );

	lua_pushnumber( state, 12.0f );


	return 1;
}

extern "C" static const luaL_Reg debugFuncs[] =
{
	{ "Bark", Debug_Bark },
	{ nullptr, nullptr }
};

void Deleter( dxi::scripting::IScriptEngine * se )
{
	delete se;
}
				   
__declspec(dllexport) bool DXILoader( dxi::core::Game * game, const qxml::Document * document )
{
	dxi::scripting::IScriptEngine * se = game->GetScriptEngine( "lua" );
	if ( ! se )
	{
		game->ReportError( dxi::ErrorLevel::Failure, "DXILuaShapeExt", "Lua extension not found!" );
		return false;
	}

	dxilua::ScriptEngine * luaSE = dynamic_cast<dxilua::ScriptEngine *>(se);
	if ( ! luaSE )
	{
		game->ReportError( dxi::ErrorLevel::Failure, "DXILuaShapeExt", "Lua extension found, but wrong version (more, we can't understand how to use it)!" );
	}

	RegisterPxMaterial( luaSE );
	RegisterPxShape( luaSE );
						  	
	//game->AddScriptEngine( "lua", std::shared_ptr< dxi::scripting::IScriptEngine >( new dxilua::ScriptEngine( game ), Deleter ) );
	return true;
}

