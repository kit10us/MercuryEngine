// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <mephylua/ExportEntity.h>
#include <melua/ExportObjectComponent.h>
#include <melua/ExportObjectAction.h>
#include <melua/Util.h>
#include <MEPhysicsLuaExt.h>

using namespace melua;

static melua::ScriptEngine * g_luaSE;
static me::IGame * g_game;

namespace mephylua
{
	char* EntityObjectComponentProxy::Name()
	{
		return "MEPhysicEntity";
	}

	EntityObjectComponentProxy::EntityObjectComponentProxy( me::object::IObjectComponent::ptr component )
		: component{ component }
		, entity{ dynamic_cast<mephy::Entity *>( component.get() ) }
	{
	}

	EntityObjectComponentProxy::~EntityObjectComponentProxy()
	{
		if( !component )
		{
			delete entity;
		}
		entity = 0;
	}

	int EntityObjectComponent_AsObjectComponent( lua_State* state )
	{
		auto proxy = CheckUserType< EntityObjectComponentProxy >( state, 1 );
		return PushUserType< ObjectComponentProxy >( state, { proxy->component } );
	}

	int EntityObjectComponent_Constructor( lua_State * state )
	{
		ScriptEngine * se = ScriptEngine::GetInstance();
		auto game = se->GetGame();

		int args = lua_gettop( state );

		if( args == 0 )
		{ // Can't create directly.
			return PushNil( state );
		}
		else if( args == 1 )
		{ // From an IObjectComponent::ptr.

			auto type = GetTypename( state, 1 );
			if( unify::StringIs( type, ObjectComponentProxy::Name() ) )
			{
				auto proxy = CheckUserType< ObjectComponentProxy >( state, 1 );
				return PushUserType< EntityObjectComponentProxy >( state, { proxy->component }  );
			}
			else
			{
				return PushNil( state );
			}
		}
		else
		{
			return PushNil( state );
		}
	}

	int EntityObjectComponent_Destructor( lua_State * state )
	{
		EntityObjectComponentProxy * proxy = CheckUserType< EntityObjectComponentProxy >( state, 1 );
		delete proxy;
		return 0;
	}


	void RegisterEntityObjectComponent( melua::ScriptEngine * luaSE )
	{
		g_game = luaSE->GetGame();
		g_luaSE = luaSE;

		static const luaL_Reg functions[] =
		{
			{ "AsObjectComponent", EntityObjectComponent_AsObjectComponent },
			{ nullptr, nullptr }
		};

		Type type{ EntityObjectComponentProxy::Name(), functions, sizeof( functions ) / sizeof( luaL_Reg ), EntityObjectComponent_Constructor, EntityObjectComponent_Destructor };
		g_luaSE->AddType( type );
	}
}