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
#pragma comment( lib, "../extensions/DXIPhysX.lib" )


void Deleter( me::IScriptEngine * se )
{
	delete se;
}
				   
extern "C" __declspec(dllexport) bool DXILoader( me::IGame * game, const qxml::Document * doc );
__declspec(dllexport) bool DXILoader( me::IGame * game, const qxml::Document * document )
{
	auto gcse = game->GetComponent( "LUA", 0 );
	if( !gcse )
	{
		game->ReportError( me::ErrorLevel::Failure, "DXILuaShapeExt", "Lua extension not found!" );
		return false;
	}
	me::IScriptEngine * se = dynamic_cast<me::IScriptEngine *>(gcse.get());

	dxilua::ScriptEngine * luaSE = dynamic_cast<dxilua::ScriptEngine *>(se);
	if ( ! luaSE )
	{
		game->ReportError( me::ErrorLevel::Failure, "DXILuaShapeExt", "Lua extension found, but wrong version (more, we can't understand how to use it)!" );
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

