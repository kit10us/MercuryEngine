// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#include <MEPhysicsLuaExt.h>
#include <mephylua/ExportGameComponent.h>
#include <mephylua/collider/ExportCollider.h>
#include <melua/ScriptEngine.h>
#include <memory.h>
#include <me/game/Game.h>

#pragma comment( lib, "lua53" )

#ifdef _DEBUG
	#pragma comment( lib, "../../extensions/Debug/MELua.lib" )
	#pragma comment( lib, "../../extensions/Debug/MEPhysics.lib" )
#else
	#pragma comment( lib, "../../extensions/Release/MELua.lib" )
	#pragma comment( lib, "../../extensions/Release/MEPhysics.lib" )
#endif

void Deleter( me::game::component::IGameComponent * se )
{
	delete se;
}
				   
extern "C" __declspec(dllexport) bool MELoader( me::game::IGame * gameInstance, const qxml::Document * doc );
__declspec(dllexport) bool MELoader( me::game::IGame * gameInstance, const qxml::Document * document )
{
	auto gcse = gameInstance->GetComponent( "Lua" );
	if( !gcse )
	{
		gameInstance->Debug()->ReportError( me::ErrorLevel::Failure, "MEPhysicsLuaExt", "Lua Extension (\"Lua\") not found!" );
		return false;
	}
	me::game::component::IGameComponent * se = dynamic_cast<me::game::component::IGameComponent *>(gcse.get());

	melua::ScriptEngine * luaSE = dynamic_cast<melua::ScriptEngine *>(se);
	if ( ! luaSE )
	{
		gameInstance->Debug()->ReportError( me::ErrorLevel::Failure, "MEPhysicsLuaExt", "Lua Extension found, but wrong version (we can't understand how to use it)!" );
	}

	mephylua::RegisterGameComponent( luaSE );
	mephylua::RegisterColliderObjectComponent( luaSE );	

	return true;
}

