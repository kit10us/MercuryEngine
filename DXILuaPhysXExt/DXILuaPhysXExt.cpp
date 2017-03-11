// DXILuaDLL.cpp : Defines the exported functions for the DLL application.
//

#include "DXILuaPhysXExt.h"
#include <dxilua/ScriptEngine.h>
#include <memory.h>
#include <me/Game.h>

#include <luaphysx/ExportPhysX.h>
#include <luaphysx/ExportPxSceneComponent.h>
#include <luaphysx/ExportPxMaterial.h>
#include <luaphysx/ExportPxShape.h>
#include <luaphysx/ExportPxRigidBody.h>
#include <luaphysx/ExportPxRigidStatic.h>
#include <luaphysx/ExportPxController.h>

#pragma comment( lib, "lua53" )
#pragma comment( lib, "../extensions/DXILua.lib" )
#pragma comment( lib, "../extensions/MEPhysX.lib" )


void Deleter( me::IGameComponent * se )
{
	delete se;
}
				   
extern "C" __declspec(dllexport) bool MELoader( me::IGame * game, const qxml::Document * doc );
__declspec(dllexport) bool MELoader( me::IGame * game, const qxml::Document * document )
{
	auto gcse = game->GetComponent( "LuaScriptEngine", 0 );
	if( !gcse )
	{
		game->ReportError( me::ErrorLevel::Failure, "DXILuaPhysXExt", "Lua Extension (\"LuaScriptEngine\") not found!" );
		return false;
	}
	me::IGameComponent * se = dynamic_cast<me::IGameComponent *>(gcse.get());

	dxilua::ScriptEngine * luaSE = dynamic_cast<dxilua::ScriptEngine *>(se);
	if ( ! luaSE )
	{
		game->ReportError( me::ErrorLevel::Failure, "DXILuaPhysXExt", "Lua Extension found, but wrong version (we can't understand how to use it)!" );
	}

	RegisterPhysX( luaSE, game );
	RegisterPxSceneComponent( luaSE, game );
	RegisterPxMaterial( luaSE, game );
	RegisterPxShape( luaSE, game );
	RegisterPxRigidBody( luaSE, game );
	RegisterPxRigidStatic( luaSE, game );
	RegisterPxController( luaSE, game );

	//game->AddScriptEngine( "lua", std::shared_ptr< dxi::scripting::IScriptEngine >( new dxilua::ScriptEngine( game ), Deleter ) );
	return true;
}

