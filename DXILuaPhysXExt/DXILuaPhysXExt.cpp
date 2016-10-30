// DXILuaDLL.cpp : Defines the exported functions for the DLL application.
//

#include "DXILuaPhysXExt.h"
#include <dxilua/DXILua.h>
#include <memory.h>
#include <dxi/win/DXILib.h>
#include <dxi/core/Game.h>

#include <luaphysx/ExportPhysX.h>
#include <luaphysx/ExportPxSceneComponent.h>
#include <luaphysx/ExportPxMaterial.h>
#include <luaphysx/ExportPxShape.h>
#include <luaphysx/ExportPxRigidBody.h>
#include <luaphysx/ExportPxRigidStatic.h>

#pragma comment( lib, "lua53" )
#pragma comment( lib, "../extensions/DXILua.lib" ) // TODO: Ensure these point to the DLL output, once we have one, instead of the tests_data.
#pragma comment( lib, "../extensions/DXIPhysX.lib" )


void Deleter( dxi::scripting::IScriptEngine * se )
{
	delete se;
}
				   
extern "C" __declspec(dllexport) bool DXILoader( dxi::core::IGame * game, const qxml::Document * doc );
__declspec(dllexport) bool DXILoader( dxi::core::IGame * game, const qxml::Document * document )
{
	auto gcse = game->GetComponent( "LUA", 0 );
	if( !gcse )
	{
		game->ReportError( dxi::ErrorLevel::Failure, "DXILuaShapeExt", "Lua extension not found!" );
		return false;
	}
	dxi::scripting::IScriptEngine * se = dynamic_cast<dxi::scripting::IScriptEngine *>(gcse.get());

	dxilua::ScriptEngine * luaSE = dynamic_cast<dxilua::ScriptEngine *>(se);
	if ( ! luaSE )
	{
		game->ReportError( dxi::ErrorLevel::Failure, "DXILuaShapeExt", "Lua extension found, but wrong version (more, we can't understand how to use it)!" );
	}

	RegisterPhysX( luaSE, game );
	RegisterPxSceneComponent( luaSE, game );
	RegisterPxMaterial( luaSE, game );
	RegisterPxShape( luaSE, game );
	RegisterPxRigidBody( luaSE, game );
	RegisterPxRigidStatic( luaSE, game );

	//game->AddScriptEngine( "lua", std::shared_ptr< dxi::scripting::IScriptEngine >( new dxilua::ScriptEngine( game ), Deleter ) );
	return true;
}

