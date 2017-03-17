// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <melua/ScriptEngine.h>
#include <melua/ExportGeometry.h>
#include <melua/shape/ExportShapeParameters.h>
#include <sg/ShapeCreators.h>
#include <me/Game.h>

using namespace melua;
using namespace me;

int PushGeometry( lua_State * state, me::Geometry::ptr geometry )
{
	GeometryProxy ** newProxy = (GeometryProxy**)(lua_newuserdata( state, sizeof( GeometryProxy* ) ));
	*newProxy = new GeometryProxy();
	(*newProxy)->geometry = geometry;
	luaL_setmetatable( state, "GeometryProxy" );
	return 1;
}

GeometryProxy* CheckGeometry( lua_State* state, int index )
{			
	GeometryProxy** ud = (GeometryProxy**)luaL_testudata( state, index, "Geometry" );
	return ud ? *ud : nullptr;
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

	auto game = dynamic_cast< Game * >( ScriptEngine::GetGame() );

	try
	{
	
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
		geometry = sg::CreateShape( game->GetOS()->GetRenderer(0), parameters->parameters );
	}

	GeometryProxy ** geometryProxy = (GeometryProxy**)(lua_newuserdata( state, sizeof( GeometryProxy* ) ));
	*geometryProxy = new GeometryProxy;
	(*geometryProxy)->geometry = geometry;
	luaL_setmetatable( state, "Geometry" );
	}
	catch (std::exception ex )
	{
		game->ReportError( ErrorLevel::Failure, "LUA", ex.what() );
	}
	
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
	ScriptEngine * se = ScriptEngine::GetInstance();
	se->AddType( "Geometry", GeometryFunctions, sizeof( GeometryFunctions ) / sizeof( luaL_Reg ), Geometry_Constructor, Geometry_Destructor );
}

