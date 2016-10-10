// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxilua/DXILua.h>
#include <dxilua/ExportGeometry.h>
#include <dxilua/shape/ExportShapeParameters.h>
#include <dxi/shapes/ShapeCreators.h>

using namespace dxilua;
using namespace dxi;

int PushGeometry( lua_State * state, dxi::Geometry::ptr geometry )
{
	GeometryProxy ** newProxy = (GeometryProxy**)(lua_newuserdata( state, sizeof( GeometryProxy* ) ));
	*newProxy = new GeometryProxy();
	(*newProxy)->geometry = geometry;
	luaL_setmetatable( state, "GeometryProxy" );
	return 1;
}

GeometryProxy* CheckGeometry( lua_State* state, int index )
{
	GeometryProxy* ud = *(GeometryProxy**)luaL_checkudata( state, index, "Geometry" );
	return ud;
}
			   
static const luaL_Reg GeometryFunctions[] =
{
	{ nullptr, nullptr }
};

int Geometry_Constructor( lua_State * state )
{
	int top = lua_gettop( state );
	int type = lua_type( state, 1 );

	Geometry::ptr geometry;

	auto game = ScriptEngine::GetGame();
	
	// Allow pulling existing from manager...
	if ( top == 1 && type == LUA_TSTRING )
	{
		std::string name = lua_tostring( state, 1 );
		geometry = game->GetManager< Geometry >()->Find( name );
	}
	else if ( top == 2)
	{
		std::string name = lua_tostring( state, 1 );
		unify::Path source = lua_tostring( state, 2 );
		geometry = game->GetManager< Geometry >()->Add( name, source );
	}
	else
	{
		ShapeParameters * parameters = CheckShapeParameters( state, 1 );
		geometry = game->CreateShape( parameters->parameters );
	}

	GeometryProxy ** geometryProxy = (GeometryProxy**)(lua_newuserdata( state, sizeof( GeometryProxy* ) ));
	*geometryProxy = new GeometryProxy;
	(*geometryProxy)->geometry = geometry;
	luaL_setmetatable( state, "Geometry" );
	
	return 1;
}

int Geometry_Destructor( lua_State * state )
{
	GeometryProxy * GeometryProxy = CheckGeometry( state, 1 );
	delete GeometryProxy;
	return 0;
}

void RegisterGeometry( lua_State * state )
{
	lua_register( state, "Geometry", Geometry_Constructor );
	luaL_newmetatable( state, "Geometry" );
	lua_pushcfunction( state, Geometry_Destructor ); lua_setfield( state, -2, "__gc" );
	lua_pushvalue( state, -1 ); lua_setfield( state, -2, "__index" );
	luaL_setfuncs( state, GeometryFunctions, 0 );
	lua_pop( state, 1 );
}

