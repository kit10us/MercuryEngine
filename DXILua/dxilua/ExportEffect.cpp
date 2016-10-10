// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxilua/DXILua.h>
#include <dxilua/ExportEffect.h>

using namespace dxilua;
using namespace dxi;

int PushEffect( lua_State * state, dxi::Effect::ptr effect )
{
	EffectProxy ** newProxy = (EffectProxy**)(lua_newuserdata( state, sizeof( EffectProxy* ) ));
	*newProxy = new EffectProxy();
	(*newProxy)->effect = effect;
	luaL_setmetatable( state, "EffectProxy" );
	return 1;
}

EffectProxy* CheckEffect( lua_State* state, int index )
{
	EffectProxy* ud = *(EffectProxy**)luaL_checkudata( state, index, "Effect" );
	return ud;
}
			   
static const luaL_Reg EffectFunctions[] =
{
	{ nullptr, nullptr }
};

int Effect_Constructor( lua_State * state )
{
	int top = lua_gettop( state );
	int type = lua_type( state, 1 );

	Effect::ptr effect;

	auto game = ScriptEngine::GetGame();
	
	// Allow pulling existing from manager...
	if ( top == 1 )
	{
		std::string name = lua_tostring( state, 1 );
		effect = game->GetManager< Effect >()->Find( name );
	}
	else
	{
		std::string name = lua_tostring( state, 1 );
		unify::Path source = lua_tostring( state, 2 );
		effect = game->GetManager< Effect >()->Add( name, source );
	}

	EffectProxy ** effectProxy = (EffectProxy**)(lua_newuserdata( state, sizeof( EffectProxy* ) ));
	*effectProxy = new EffectProxy;
	(*effectProxy)->effect = effect;
	luaL_setmetatable( state, "Effect" );
	
	return 1;
}

int Effect_Destructor( lua_State * state )
{
	EffectProxy * EffectProxy = CheckEffect( state, 1 );
	delete EffectProxy;
	return 0;
}

void RegisterEffect( lua_State * state )
{
	lua_register( state, "Effect", Effect_Constructor );
	luaL_newmetatable( state, "Effect" );
	lua_pushcfunction( state, Effect_Destructor ); lua_setfield( state, -2, "__gc" );
	lua_pushvalue( state, -1 ); lua_setfield( state, -2, "__index" );
	luaL_setfuncs( state, EffectFunctions, 0 );
	lua_pop( state, 1 );
}

