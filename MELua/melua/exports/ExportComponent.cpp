// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <melua/exports/ExportComponent.h>
#include <melua/exports/ExportObject.h>
#include <melua/ScriptEngine.h>
#include <melua/exports/ExportObjectComponent.h>
#include <melua/Util.h>

using namespace me;
using namespace scene;

namespace melua
{
	char* ComponentProxy::Name()
	{
		return "Component";
	}

	ComponentProxy* CheckComponent( lua_State* state, int index )
	{
		ComponentProxy* ud = *(ComponentProxy**)luaL_checkudata( state, index, "Component" );
		return ud;
	}

	int PushComponent( lua_State * state, me::IComponent* component )
	{
		ComponentProxy ** proxy = (ComponentProxy**)( lua_newuserdata( state, sizeof( ComponentProxy* ) ) );
		*proxy = new ComponentProxy;
		luaL_setmetatable( state, "Component" );
		( *proxy )->component = component;
		return 1;
	}

	int Component_SetEnabled( lua_State * state )
	{
		int args = lua_gettop( state );
		assert( args == 2 );

		ComponentProxy * componentProxy = CheckComponent( state, 1 );
		bool enabled = lua_toboolean( state, 2 ) ? true : false;

		componentProxy->component->SetEnabled( enabled );
		return 0;
	}

	int Component_GetEnabled( lua_State * state )
	{
		int args = lua_gettop( state );
		assert( args == 1 );

		ComponentProxy * componentProxy = CheckComponent( state, 1 );

		lua_pushboolean( state, componentProxy->component->IsEnabled() );

		return 1;
	}

	int Component_GetType( lua_State * state )
	{
		int args = lua_gettop( state );
		assert( args == 1 );

		ComponentProxy * componentProxy = CheckComponent( state, 1 );
		lua_pushstring( state, componentProxy->component->GetTypeName().c_str() );
		return 1;
	}

	int Component_GetWhat( lua_State * state )
	{
		int args = lua_gettop( state );
		assert( args == 1 );

		ComponentProxy * componentProxy = CheckComponent( state, 1 );
		lua_pushstring( state, componentProxy->component->GetWhat().c_str() );
		return 1;
	}

	int Component_GetValueCount( lua_State * state )
	{
		int args = lua_gettop( state );
		assert( args == 1 );

		ComponentProxy * componentProxy = CheckComponent( state, 1 );
		lua_pushnumber( state, componentProxy->component->GetValueCount() );
		return 1;
	}

	int Component_ValueExists( lua_State * state )
	{
		int args = lua_gettop( state );
		assert( args == 2 );

		ComponentProxy * componentProxy = CheckComponent( state, 1 );

		std::string name = lua_tostring( state, 2 );

		lua_pushboolean( state, componentProxy->component->ValueExists( name ) );
		return 1;
	}

	int Component_GetValueName( lua_State * state )
	{
		int args = lua_gettop( state );
		assert( args == 2 );

		ComponentProxy * componentProxy = CheckComponent( state, 1 );

		int index = (int)lua_tonumber( state, 2 );

		lua_pushstring( state, componentProxy->component->GetValueName( index ).c_str() );
		return 1;
	}

	int Component_FindValueIndex( lua_State * state )
	{
		int args = lua_gettop( state );
		assert( args == 2 );

		ComponentProxy * componentProxy = CheckComponent( state, 1 );

		std::string name = lua_tostring( state, 2 );

		lua_pushnumber( state, componentProxy->component->FindValueIndex( name ) );
		return 1;
	}

	int Component_GetValue( lua_State * state )
	{
		int args = lua_gettop( state );
		assert( args == 2 );

		ComponentProxy * componentProxy = CheckComponent( state, 1 );

		int type = lua_type( state, 2 );

		if( type == LUA_TNUMBER )
		{
			int index = (int)lua_tonumber( state, 2 );
			lua_pushstring( state, componentProxy->component->GetValue( index ).c_str() );
			return 1;
		}
		else if( type == LUA_TSTRING )
		{
			std::string name = lua_tostring( state, 2 );
			lua_pushstring( state, componentProxy->component->GetValue( name ).c_str() );
			return 1;
		}
		else
		{
			return 0;
		}
	}

	int Component_SetValue( lua_State * state )
	{
		int args = lua_gettop( state );
		assert( args == 3 );

		ComponentProxy * componentProxy = CheckComponent( state, 1 );

		int type = lua_type( state, 2 );

		int valueType = lua_type( state, 3 );
		std::string value;
		if( valueType == LUA_TSTRING )
		{
			value = lua_tostring( state, 3 );
		}
		else if( valueType == LUA_TNUMBER )
		{
			value = unify::Cast< std::string >( lua_tonumber( state, 3 ) );
		}
		else if( valueType == LUA_TBOOLEAN )
		{
			value = unify::Cast< std::string >( lua_toboolean( state, 3 ) );
		}
		else
		{
			value = "";
		}

		if( type == LUA_TNUMBER )
		{
			int index = (int)lua_tonumber( state, 2 );
			state, componentProxy->component->SetValue( index, value );
			return 0;
		}
		else if( type == LUA_TSTRING )
		{
			std::string name = lua_tostring( state, 2 );
			componentProxy->component->SetValue( name, value );
			return 0;
		}
		else
		{
			return 0;
		}
	}

	int Component_Constructor( lua_State * state )
	{
		ScriptEngine * se = ScriptEngine::GetInstance();
		auto gameInstance = se->GetGame();
		int top = lua_gettop( state );

		std::string typeA = GetTypename( state );
		if( top == 2 )
		{
			if( unify::StringIs( typeA, "Component" ) )
			{
				ComponentProxy* proxy = CheckComponent( state, 1 );
				return PushComponent( state, proxy->component );
			}
			else if( unify::StringIs( typeA, "ObjectComponent" ) )
			{
				ObjectComponentProxy* proxy = CheckUserType< ObjectComponentProxy >( state, 1 );
				return PushUserType< ObjectComponentProxy >( state, { proxy->component } );
			}
		}

		lua_pushnil( state );
		return 1;
	}

	int Component_Destructor( lua_State * state )
	{
		ComponentProxy * proxy = CheckComponent( state, 1 );
		delete proxy;
		return 0;
	}

	void RegisterComponent( lua_State * state )
	{
		const luaL_Reg ComponentMemberFunctions[] =
		{
			{ "SetEnabled", Component_SetEnabled },
			{ "GetEnabled", Component_GetEnabled },
			{ "GetType", Component_GetType },
			{ "GetWhat", Component_GetWhat },
			{ "GetValueCount", Component_GetValueCount },
			{ "ValueExists", Component_ValueExists },
			{ "GetValueName", Component_GetValueName },
			{ "FindValueIndex", Component_FindValueIndex },
			{ "GetValue", Component_GetValue },
			{ "SetValue", Component_SetValue },
			{ nullptr, nullptr }
		};

		ScriptEngine * se = ScriptEngine::GetInstance();
		Type type = { "Component", ComponentMemberFunctions, Component_Constructor, Component_Destructor };
		se->AddType( type );
	}
}