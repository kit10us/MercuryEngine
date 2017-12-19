// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <melua/ScriptEngine.h>
#include <melua/exports/ExportGeometry.h>
#include <melua/shape/ExportShapeParameters.h>
#include <melua/Util.h>
#include <sg/ShapeCreators.h>
#include <me/game/Game.h>

using namespace melua;
using namespace me;
using namespace render;

char* GeometryProxy::Name()
{
	return "Geometry";
}
			   
static const luaL_Reg GeometryFunctions[] =
{
	{ nullptr, nullptr }
};

int Geometry_Constructor( lua_State * state )
{
	int args = lua_gettop( state );

	Geometry::ptr geometry;

	ScriptEngine * se = ScriptEngine::GetInstance();
	auto gameInstance = se->GetGame();

	try
	{
		if( args == 1 )
		{
			std::string type1 = GetTypename( state, 1 );
			if( unify::StringIs( type1, "string" ) )
			{ // Find an existing geometry...
				std::string name = lua_tostring( state, 1 );
				geometry = gameInstance->GetManager< Geometry >()->Find( name );
				if( !geometry )
				{
					return PushNil( state );
				}
				else
				{
					PushUserType< GeometryProxy >( state, { geometry } );
				}
			}
			else if( unify::StringIs( type1, "ShapeParameters" ) )
			{ // 
				ShapeParameters * parameters = CheckShapeParameters( state, 1 );
				geometry = sg::CreateShape( gameInstance->GetOS()->GetRenderer( 0 ), parameters->parameters );
				return PushUserType< GeometryProxy >( state, { geometry } );
			}
			else
			{
				return PushNil( state );
			}
		}
		else if( args == 2 )
		{
			std::string type1 = GetTypename( state, 1 );
			std::string type2 = GetTypename( state, 2 );
			if( unify::StringIs( type1, "string" ) && unify::StringIs( type2, "string" ) )
			{ // Create a named geometry from a file...
				std::string name = lua_tostring( state, 1 );
				unify::Path source( lua_tostring( state, 2 ) );
				geometry = gameInstance->GetManager< Geometry >()->Add( name, source );
				return PushUserType< GeometryProxy >( state, { geometry } );
			}
			else if( unify::StringIs( type1, "string" ) && unify::StringIs( type2, "ShapeParameters" ) )
			{
				std::string name = lua_tostring( state, 1 );
				ShapeParameters * parameters = CheckShapeParameters( state, 2 );
				geometry = sg::CreateShape( gameInstance->GetOS()->GetRenderer( 0 ), parameters->parameters );
				return PushUserType< GeometryProxy >( state, { geometry } );
			}
			else
			{
				return PushNil( state );
			}
		}
	}
	catch( std::exception & ex )
	{
		Error( state, ex.what() );
	}
	
	return PushNil( state );
}

int Geometry_Destructor( lua_State * state )
{
	GeometryProxy * proxy = CheckUserType< GeometryProxy >( state, 1 );
	delete proxy;
	return 0;
}

void RegisterGeometry( lua_State * state )
{
	ScriptEngine * se = ScriptEngine::GetInstance();
	Type type = { GeometryProxy::Name(), GeometryFunctions, Geometry_Constructor, Geometry_Destructor };
	se->AddType( type );
}

