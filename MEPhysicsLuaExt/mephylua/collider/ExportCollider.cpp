// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <mephylua/collider/ExportCollider.h>
#include <melua/exports/ExportObjectComponent.h>
#include <melua/exports/ExportObjectAction.h>
#include <melua/Util.h>
#include <MEPhysicsLuaExt.h>

using namespace melua;

static melua::ScriptEngine * g_luaSE;
static me::game::IGame * g_game;

namespace mephylua
{
	char* ColliderObjectComponentProxy::Name()
	{
		return "MEPhysicCollider";
	}

	ColliderObjectComponentProxy::ColliderObjectComponentProxy( me::object::component::IObjectComponent::ptr component )
		: component{ component }
		, collider{ dynamic_cast<mephy::phy3d::collider::ColliderBase *>( component.get() ) }
	{
	}

	ColliderObjectComponentProxy::~ColliderObjectComponentProxy()
	{
		if( !component )
		{
			delete collider;
		}
		collider = 0;
	}

	int ColliderObjectComponent_AsObjectComponent( lua_State* state )
	{
		auto proxy = CheckUserType< ColliderObjectComponentProxy >( state, 1 );
		return PushUserType< ObjectComponentProxy >( state, { proxy->component } );
	}

	int ColliderObjectComponent_SetOnEnter( lua_State* state )
	{
		auto proxy = CheckUserType< ColliderObjectComponentProxy >( state, 1 );

		auto objectActionProxy = CheckUserType< melua::ObjectActionProxy >( state, 2 );
		
		proxy->collider->SetOnEnterAction( objectActionProxy->GetAction() );

		return 0;
	}

	int ColliderObjectComponent_SetOnExit( lua_State* state )
	{
		auto proxy = CheckUserType< ColliderObjectComponentProxy >( state, 1 );

		auto objectActionProxy = CheckUserType< melua::ObjectActionProxy >( state, 2 );

		proxy->collider->SetOnExitAction( objectActionProxy->GetAction() );

		return 0;
	}

	int ColliderObjectComponent_Constructor( lua_State * state )
	{
		ScriptEngine * se = ScriptEngine::GetInstance();
		auto gameInstance = se->GetGame();

		int args = lua_gettop( state );

		if( args == 0 )
		{ // Can't create a collider directly; it's abstract.

			return PushNil( state );
		}
		else if( args == 1 )
		{ // From an IObjectComponent::ptr.

			auto type = GetTypename( state, 1 );
			if( unify::StringIs( type, ObjectComponentProxy::Name() ) )
			{
				auto proxy = CheckUserType< ObjectComponentProxy >( state, 1 );
				return PushUserType< ColliderObjectComponentProxy >( state, { proxy->component }  );
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

	int ColliderObjectComponent_Destructor( lua_State * state )
	{
		ColliderObjectComponentProxy * proxy = CheckUserType< ColliderObjectComponentProxy >( state, 1 );
		delete proxy;
		return 0;
	}


	void RegisterColliderObjectComponent( melua::ScriptEngine * luaSE )
	{
		g_game = luaSE->GetGame();
		g_luaSE = luaSE;

		static const luaL_Reg functions[] =
		{
			{ "AsObjectComponent", ColliderObjectComponent_AsObjectComponent },
			{ "SetOnEnter", ColliderObjectComponent_SetOnEnter },
			{ "SetOnExit", ColliderObjectComponent_SetOnExit },
			{ nullptr, nullptr }
		};

		Type type{ ColliderObjectComponentProxy::Name(), functions, ColliderObjectComponent_Constructor, ColliderObjectComponent_Destructor };
		g_luaSE->AddType( type );
	}
}