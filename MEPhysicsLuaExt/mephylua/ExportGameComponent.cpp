// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <mephylua/ExportGameComponent.h>
#include <mephylua/collider/ExportCollider.h>
#include <mephylua/ExportEntity.h>
#include <melua/ExportComponent.h>
#include <melua/unify/ExportV3.h>
#include <melua/Util.h>

using namespace melua;

static melua::ScriptEngine * g_luaSE;
static me::IGame * g_game;

namespace mephylua
{
	char* GameComponentProxy::Name()
	{
		return "MEPhysics";
	}

	int GameComponent_CreateBoxCollider( lua_State * state )
	{
		int argc = lua_gettop( state );

		unify::V3< float > halfExt( CheckV3( state, 2 )->v3 );

		auto physics = g_game->GetComponentT< mephy::GameComponent >();
		me::object::IObjectComponent::ptr collider( physics->CreateBoxCollider( halfExt ) );
		return PushUserType< ColliderObjectComponentProxy >( state, { collider } );
	}

	int GameComponent_CreateEntity( lua_State * state )
	{
		int argc = lua_gettop( state );

		auto physics = g_game->GetComponentT< mephy::GameComponent >();
		me::object::IObjectComponent::ptr entity( physics->CreateEntity() );
		return PushUserType< EntityObjectComponentProxy >( state, { entity } );
	}

	int GameComponent_Constructor( lua_State * state )
	{
		ScriptEngine * se = ScriptEngine::GetInstance();
		auto game = se->GetGame();

		int args = lua_gettop( state );

		if( args == 0 )
		{ // Get GameComponent (loaded automatically by loading the extension)...
			auto component = game->GetComponentT< mephy::GameComponent >();
			return PushUserType< GameComponentProxy >( state, { component }  );
		}
		else if( args == 1 )
		{ // Get a GameComponent from a me::IComponent...
			auto proxy = CheckUserType< ComponentProxy >( state, 1 );
			auto component = dynamic_cast< mephy::GameComponent* >( proxy->component );
			return PushUserType< GameComponentProxy >( state, { component } );
		}
		else
		{
			lua_pushnil( state );
			return 1;
		}
	}

	int GameComponent_Destructor( lua_State * state )
	{
		GameComponentProxy * proxy = CheckUserType< GameComponentProxy >( state, 1 );
		delete proxy;
		return 0;
	}

	void RegisterGameComponent( melua::ScriptEngine * luaSE )
	{
		g_game = luaSE->GetGame();
		g_luaSE = luaSE;

		static const luaL_Reg functions[] =
		{
			{ "CreateBoxCollider", GameComponent_CreateBoxCollider },
			{ "CreateEntity", GameComponent_CreateEntity },
			{ nullptr, nullptr }
		};

		Type type{ GameComponentProxy::Name(), functions, sizeof( functions ) / sizeof( luaL_Reg ), GameComponent_Constructor, GameComponent_Destructor };

		luaSE->AddType( type );
	}

}