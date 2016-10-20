// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxilua/DXILua.h>
#include <dxilua/ExportTerra.h>
#include <dxilua/ExportTerraParameters.h>

using namespace dxilua;
using namespace dxi;

int PushTerra( lua_State * state, dxi::Geometry::ptr geometry )
{
	TerraProxy ** newProxy = (TerraProxy**)(lua_newuserdata( state, sizeof( TerraProxy* ) ));
	*newProxy = new TerraProxy();
	(*newProxy)->geometry = geometry;
	(*newProxy)->terra = dynamic_cast<dxi::Terra *>(geometry.get());
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
	int type = lua_type( state, 1 );

	auto game = ScriptEngine::GetGame();
	
	TerraParameters * parameters = CheckTerraParameters( state, 1 );
	Terra * terra( new Terra( game->GetOS()->GetRenderer(0), parameters->parameters ) );

	TerraProxy ** geometryProxy = (TerraProxy**)(lua_newuserdata( state, sizeof( TerraProxy* ) ));
	*geometryProxy = new TerraProxy;
	(*geometryProxy)->terra = terra;
	(*geometryProxy)->geometry = Geometry::ptr( terra );
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

