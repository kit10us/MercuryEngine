// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxilua/ScriptEngine.h>
#include <dxilua/ExportTerra.h>
#include <dxilua/ExportTerraParameters.h>
#include <me/Game.h>

using namespace dxilua;
using namespace me;

int PushTerra( lua_State * state, me::Geometry::ptr geometry )
{
	TerraProxy ** newProxy = (TerraProxy**)(lua_newuserdata( state, sizeof( TerraProxy* ) ));
	*newProxy = new TerraProxy();
	(*newProxy)->geometry = geometry;
	(*newProxy)->terra = dynamic_cast< Terra * >(geometry.get());
	luaL_setmetatable( state, "TerraProxy" );
	return 1;
}

TerraProxy* CheckTerra( lua_State* state, int index )
{
	TerraProxy* ud = *(TerraProxy**)luaL_checkudata( state, index, "Terra" );
	return ud;
}
			   
static const luaL_Reg TerraFunctions[] =
{
	{ nullptr, nullptr }
};

int Terra_Constructor( lua_State * state )
{	
	int top = lua_gettop( state );
	assert( top == 2 );

	auto game = dynamic_cast< me::Game * >( ScriptEngine::GetGame() );

	std::string name = lua_tostring( state, 1 );

	Geometry::ptr geo = game->GetManager < Geometry >()->Find( name );
	if ( !geo )
	{
		TerraParameters * parameters = CheckTerraParameters( state, 2 );
		geo = game->GetManager< Geometry >()->Add( name, new Terra( game->GetOS()->GetRenderer( 0 ), parameters->parameters ) );
	}

	TerraProxy ** geometryProxy = (TerraProxy**)(lua_newuserdata( state, sizeof( TerraProxy* ) ));
	*geometryProxy = new TerraProxy;
	(*geometryProxy)->terra = dynamic_cast< Terra* >( geo.get() );
	(*geometryProxy)->geometry = geo;
	luaL_setmetatable( state, "Terra" );
	
	return 1;
}

int Terra_Destructor( lua_State * state )
{
	TerraProxy * TerraProxy = CheckTerra( state, 1 );
	delete TerraProxy;
	return 0;
}

void RegisterTerra( lua_State * state )
{
	lua_register( state, "Terra", Terra_Constructor );
	luaL_newmetatable( state, "Terra" );
	lua_pushcfunction( state, Terra_Destructor ); lua_setfield( state, -2, "__gc" );
	lua_pushvalue( state, -1 ); lua_setfield( state, -2, "__index" );
	luaL_setfuncs( state, TerraFunctions, 0 );
	lua_pop( state, 1 );
}

