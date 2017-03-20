// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <melua/ScriptEngine.h>
#include <melua/ExportTerraParameters.h>

#include <melua/ExportGeometry.h>
#include <melua/ExportEffect.h>  
#include <melua/unify/ExportMatrix.h>
#include <melua/unify/ExportColor.h>
#include <melua/unify/ExportSize2.h>
#include <melua/unify/ExportSize3.h>
#include <melua/unify/ExportV2.h>
#include <melua/unify/ExportV3.h>
#include <melua/unify/ExportTexCoords.h>
#include <melua/unify/ExportTexArea.h>
#include <melua/ExportTexture.h> 
#include <unify/Color.h>

using namespace melua;
using namespace me;

int PushTerraParameters( lua_State * state, TerraParameters & shapeParameters )
{
	TerraParameters ** parameters = (TerraParameters**)(lua_newuserdata( state, sizeof( TerraParameters* ) ));
	*parameters = new TerraParameters( shapeParameters );
	luaL_setmetatable( state, "TerraParameters" );
	return 1;
}

TerraParameters * CheckTerraParameters( lua_State* state, int index )
{
	TerraParameters* ud = *(TerraParameters**)luaL_checkudata( state, index, "TerraParameters" );
	return ud;
}

int TerraParameters_SetFaces( lua_State* state )
{
	int args = lua_gettop( state );
	assert( args == 3 );

	TerraParameters * parameters = CheckTerraParameters( state, 1 );
	unsigned int r = (unsigned int)luaL_checknumber( state, 2 );
	unsigned int c = (unsigned int)luaL_checknumber( state, 3 );

	parameters->parameters.Set< unify::RowColumn< unsigned int > >( "faces", unify::RowColumn< unsigned int >( r, c ) );

	return 0;
}
						   
int TerraParameters_SetPoints( lua_State* state )
{
	int args = lua_gettop( state );
	assert( args == 3 );

	TerraParameters * parameters = CheckTerraParameters( state, 1 );
	unsigned int r = (unsigned int)luaL_checknumber( state, 2 );
	unsigned int c = (unsigned int)luaL_checknumber( state, 3 );

	parameters->parameters.Set< unify::RowColumn< unsigned int > >( "points", unify::RowColumn< unsigned int >( r, c ) );

	return 0;
}

int TerraParameters_SetSize( lua_State* state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	TerraParameters * parameters = CheckTerraParameters( state, 1 );
	unify::Size< float > size = CheckSize2( state, 2 );
	parameters->parameters.Set< unify::Size< float > >( "size", size );

	return 0;
}

int TerraParameters_SetConstant( lua_State* state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	TerraParameters * parameters = CheckTerraParameters( state, 1 );
	float constant = (float)luaL_checknumber( state, 2 );
	parameters->parameters.Set< float >( "constant", constant );
	return 0;
}												

int TerraParameters_SetTexArea( lua_State* state )
{
	int args = lua_gettop( state );
	
	TerraParameters * parameters = CheckTerraParameters( state, 1 );

	if ( args == 3 ) // TexCoords x 
	{
		unify::TexCoords ul( CheckTexCoords( state, 2 ) );
		unify::TexCoords dr( CheckTexCoords( state, 3 ) );
		parameters->parameters.Set< unify::TexArea >( "texarea", unify::TexArea( ul, dr ) );
	}
	else if ( args == 2 ) // TexArea
	{
		unify::TexArea texArea( CheckTexArea( state, 2 ) );
		parameters->parameters.Set< unify::TexArea >( "texarea", texArea );
	}
	return 0;
}

int TerraParameters_SetEffect( lua_State* state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	TerraParameters * parameters = CheckTerraParameters( state, 1 );
	EffectProxy * effectProxy = CheckEffect( state, 2 );

	parameters->parameters.Set< Effect::ptr >( "effect", effectProxy->effect );
	return 0;
}	

int TerraParameters_SetHeightMap( lua_State* state )
{
	int args = lua_gettop( state );
	assert( args == 3 );

	auto game = ScriptEngine::GetGame();

	TerraParameters * parameters = CheckTerraParameters( state, 1 );

	TextureProxy * texture = CheckTexture( state, 2 );

	unify::ColorUnit colorOp( CheckColor( state, 3 ) );
	Terra::TextureOpMap opMap( texture->texture, colorOp );

	parameters->parameters.Set< Terra::TextureOpMap >( "heightmap", opMap );
	return 0;
}

int TerraParameters_SetAlphaMap( lua_State* state )
{
	int args = lua_gettop( state );
	assert( args == 3 );

	auto game = ScriptEngine::GetGame();

	TerraParameters * parameters = CheckTerraParameters( state, 1 );

	TextureProxy * texture = CheckTexture( state, 2 );

	unify::Color colorOp( CheckColor( state, 3 ) );
	Terra::TextureOpMap opMap( texture->texture, colorOp );

	parameters->parameters.Set< Terra::TextureOpMap >( "alphamap", opMap );
	return 0;
}

static const luaL_Reg TerraParametersFunctions[] =
{
	{ "SetFaces", TerraParameters_SetFaces },
	{ "SetPoints", TerraParameters_SetPoints },
	{ "SetSize", TerraParameters_SetSize },
	{ "SetConstant", TerraParameters_SetConstant },
	{ "SetTexArea", TerraParameters_SetTexArea },
	{ "SetEffect", TerraParameters_SetEffect },
	{ "SetHeightMap", TerraParameters_SetHeightMap },
	{ "SetAlphahMap", TerraParameters_SetAlphaMap },

	{ nullptr, nullptr }
};

int TerraParameters_Constructor( lua_State * state )
{
	TerraParameters ** parameters = (TerraParameters**)(lua_newuserdata( state, sizeof( TerraParameters* ) ));
	*parameters = new TerraParameters;
	luaL_setmetatable( state, "TerraParameters" );
	return 1;
}

int TerraParameters_Destructor( lua_State * state )
{
	TerraParameters * parameters = CheckTerraParameters( state, 1 );
	delete parameters;
	return 0;
}

void RegisterTerraParameters( lua_State * state )
{
	ScriptEngine * se = ScriptEngine::GetInstance();
	se->AddType( { "TerraParameters", TerraParametersFunctions, sizeof( TerraParametersFunctions ) / sizeof( luaL_Reg ), TerraParameters_Constructor, TerraParameters_Destructor } );
}

