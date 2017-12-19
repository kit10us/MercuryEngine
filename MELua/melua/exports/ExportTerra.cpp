// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <melua/ScriptEngine.h>
#include <melua/exports/ExportTerra.h>
#include <melua/exports/ExportTerraParameters.h>
#include <melua/Util.h>
#include <me/game/Game.h>

using namespace melua;
using namespace me;
using namespace render;

int PushTerra( lua_State * state, Geometry::ptr geometry )
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

	ScriptEngine * se = ScriptEngine::GetInstance();
	auto gameInstance = se->GetGame();

	if ( top == 1 )
	{
		std::string type = GetTypename( state, 1 );

		// If we only have a string parameters, it's expected to be an existing Terra...
		if ( type == "string " )
		{
			std::string name = lua_tostring( state, 1 );
			Geometry::ptr geo = gameInstance->GetManager < Geometry >()->Find( name );
			if ( ! geo )
			{
				lua_pushnil( state );
				return 1;
			}
			else
			{
				TerraProxy ** geometryProxy = (TerraProxy**)(lua_newuserdata( state, sizeof( TerraProxy* ) ));
				*geometryProxy = new TerraProxy;
				(*geometryProxy)->terra = dynamic_cast< Terra* >( geo.get() );
				(*geometryProxy)->geometry = geo;
				luaL_setmetatable( state, "Terra" );
				return 1;
			}
		}

		// If we are a single TerraParameters, then we just want a Terra, don't directly register it in our Geometry Manager...
		else if ( type == "TerraParameters" )
		{
			TerraParameters * parameters = CheckTerraParameters( state, 1 );
			Geometry::ptr geo( new Terra( gameInstance->GetOS()->GetRenderer( 0 ), parameters->parameters ) );

			TerraProxy ** geometryProxy = (TerraProxy**)(lua_newuserdata( state, sizeof( TerraProxy* ) ));
			*geometryProxy = new TerraProxy;
			(*geometryProxy)->terra = dynamic_cast< Terra* >( geo.get() );
			(*geometryProxy)->geometry = geo;
			luaL_setmetatable( state, "Terra" );																			   	
			return 1;
		}	
		else
		{
			lua_pushnil( state );
			return 1;
		}
	}

	// If we have two parameters, then we expect a name and TerraParameters...
	else if ( top == 2 )
	{
		std::string name = lua_tostring( state, 1 );

		Geometry::ptr geo = gameInstance->GetManager < Geometry >()->Find( name );
		if ( !geo )
		{
			TerraParameters * parameters = CheckTerraParameters( state, 2 );
			geo = gameInstance->GetManager< Geometry >()->Add( name, new Terra( gameInstance->GetOS()->GetRenderer( 0 ), parameters->parameters ) );
		}

		TerraProxy ** geometryProxy = (TerraProxy**)(lua_newuserdata( state, sizeof( TerraProxy* ) ));
		*geometryProxy = new TerraProxy;
		(*geometryProxy)->terra = dynamic_cast<Terra*>(geo.get());
		(*geometryProxy)->geometry = geo;
		luaL_setmetatable( state, "Terra" );

		return 1;
	}
	else
	{
		lua_pushnil( state );
		return 1;
	}
}

int Terra_Destructor( lua_State * state )
{
	TerraProxy * TerraProxy = CheckTerra( state, 1 );
	delete TerraProxy;
	return 0;
}

void RegisterTerra( lua_State * state )
{
	ScriptEngine * se = ScriptEngine::GetInstance();
	Type type = { "Terra", TerraFunctions, Terra_Constructor, Terra_Destructor };
	se->AddType( type );
}

