// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include "MEPhysicsLuaExt.h"
#include <melua/ScriptEngine.h>
#include <memory.h>
#include <me/Game.h>

/*
#include <luaphysx/ExportPhysX.h>
#include <luaphysx/ExportPxSceneComponent.h>
#include <luaphysx/ExportPxMaterial.h>
#include <luaphysx/ExportPxShape.h>
#include <luaphysx/ExportPxRigidBody.h>
#include <luaphysx/ExportPxRigidStatic.h>
#include <luaphysx/ExportPxController.h>
  */

#pragma comment( lib, "lua53" )

#ifdef _DEBUG
	#pragma comment( lib, "../extensions/Debug/MELua.lib" )
	#pragma comment( lib, "../extensions/Debug/MEPhysics.lib" )
#else
	#pragma comment( lib, "../extensions/Release/MELua.lib" )
	#pragma comment( lib, "../extensions/Relesase/MEPhysics.lib" )
#endif


void Deleter( me::IGameComponent * se )
{
	delete se;
}
				   
extern "C" __declspec(dllexport) bool MELoader( me::IGame * game, const qxml::Document * doc );
__declspec(dllexport) bool MELoader( me::IGame * game, const qxml::Document * document )
{
	auto gcse = game->GetComponent( "Lua" );
	if( !gcse )
	{
		game->ReportError( me::ErrorLevel::Failure, "MEPhysicsLuaExt", "Lua Extension (\"Lua\") not found!" );
		return false;
	}
	me::IGameComponent * se = dynamic_cast<me::IGameComponent *>(gcse.get());

	melua::ScriptEngine * luaSE = dynamic_cast<melua::ScriptEngine *>(se);
	if ( ! luaSE )
	{
		game->ReportError( me::ErrorLevel::Failure, "MEPhysicsLuaExt", "Lua Extension found, but wrong version (we can't understand how to use it)!" );
	}

	/*
	RegisterPhysX( luaSE, game );
	RegisterPxSceneComponent( luaSE, game );
	RegisterPxMaterial( luaSE, game );
	RegisterPxShape( luaSE, game );
	RegisterPxRigidBody( luaSE, game );
	RegisterPxRigidStatic( luaSE, game );
	RegisterPxController( luaSE, game );
	*/

	return true;
}

