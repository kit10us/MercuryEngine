// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <mephylua/ExportGameComponent.h>
#include <mephylua/collider/ExportCollider.h>
#include <melua/exports/ExportComponent.h>
#include <melua/unify/ExportV3.h>
#include <melua/Util.h>

using namespace melua;

static melua::ScriptEngine * g_luaSE;
static me::game::IGame * g_game;

namespace mephylua
{
	char* GameComponentProxy::Name()
	{
		return "MEPhysics";
	}

	int GameComponent_CreateBoxCollider( lua_State * state )
	{
		int argc = lua_gettop( state );

		// Size
		unify::V3< float > ext( CheckV3( state, 2 )->v3 );

		bool moveable = false;
		if( argc > 2 )
		{
			moveable = melua::Check< bool >( state, 3 );
		}

		float mass = 0.0f;
		if( argc > 3 )
		{
			mass = melua::Check< float >( state, 4 );
		}

		auto physics = g_game->GetComponentT< mephy::phy3d::GameComponent >();
		me::object::component::IObjectComponent::ptr collider( physics->CreateBoxCollider( ext * 0.5f ) );
		return PushUserType< ColliderObjectComponentProxy >( state, { collider } );
	}

	int GameComponent_Constructor( lua_State * state )
	{
		ScriptEngine * se = ScriptEngine::GetInstance();
		auto gameInstance = se->GetGame();

		int args = lua_gettop( state );

		if( args == 0 )
		{ // Get GameComponent (loaded automatically by loading the extension)...
			auto component = gameInstance->GetComponentT< mephy::phy3d::GameComponent >();
			return PushUserType< GameComponentProxy >( state, { component }  );
		}
		else if( args == 1 )
		{ // Get a GameComponent from a me::IComponent...
			auto proxy = CheckUserType< ComponentProxy >( state, 1 );
			auto component = dynamic_cast< mephy::phy3d::GameComponent* >( proxy->component );
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
			{ nullptr, nullptr }
		};

		Type type{ GameComponentProxy::Name(), functions, GameComponent_Constructor, GameComponent_Destructor };
		luaSE->AddType( type );
	}

}