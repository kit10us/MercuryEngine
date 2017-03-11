// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxilua/ScriptEngine.h>
#include <dxilua/ExportEffect.h>
#include <dxilua/ExportTexture.h>
#include <me/Game.h>

using namespace dxilua;
using namespace me;

int PushEffect( lua_State * state, me::Effect::ptr effect )
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

int Effect_SetTexture( lua_State* state )
{
	int top = lua_gettop( state );

	EffectProxy* proxy = CheckEffect( state, 1 );
	int stage = (int)lua_tonumber( state, 2 );
	TextureProxy* texture = CheckTexture( state, 3 );

	proxy->effect->SetTexture( stage, texture->texture );

	return 0;
}

int Effect_Constructor( lua_State * state )
{
	auto game = dynamic_cast< Game * >( ScriptEngine::GetGame() );
	int top = lua_gettop( state );

	if ( top < 1 )
	{
		game->ReportError( me::ErrorLevel::Failure, "LUA", "Effect requires at least the name of the effect as a parameter!" );
	}

	int type = lua_type( state, 1 );

	Effect::ptr effect;

	std::string name = lua_tostring( state, 1 );

	if ( top == 1 )
	{
		effect = game->GetManager< Effect >()->Find( name );
	}
	else
	{
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
	const luaL_Reg EffectFunctions[] =
	{
		{ "SetTexture", Effect_SetTexture },
		{ nullptr, nullptr }
	};

	lua_register( state, "Effect", Effect_Constructor );
	luaL_newmetatable( state, "Effect" );
	lua_pushcfunction( state, Effect_Destructor ); lua_setfield( state, -2, "__gc" );
	lua_pushvalue( state, -1 ); lua_setfield( state, -2, "__index" );
	luaL_setfuncs( state, EffectFunctions, 0 );
	lua_pop( state, 1 );
}

