// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxilua/ScriptEngine.h>
#include <dxilua/ExportTerraParameters.h>

#include <dxilua/ExportGeometry.h>
#include <dxilua/ExportEffect.h>  
#include <dxilua/unify/ExportMatrix.h>
#include <dxilua/unify/ExportColor.h>
#include <dxilua/unify/ExportSize2.h>
#include <dxilua/unify/ExportSize3.h>
#include <dxilua/unify/ExportV2.h>
#include <dxilua/unify/ExportV3.h>
#include <dxilua/unify/ExportTexCoords.h>
#include <dxilua/ExportTexture.h> 
#include <unify/Color.h>

using namespace dxilua;
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
	assert( args == 3 );

	TerraParameters * parameters = CheckTerraParameters( state, 1 );
	unify::TexCoords ul( CheckTexCoords( state, 2 ) );
	unify::TexCoords dr( CheckTexCoords( state, 3 ) );
	parameters->parameters.Set< unify::TexArea >( "texarea", unify::TexArea( ul, dr )  );
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
	lua_register( state, "TerraParameters", TerraParameters_Constructor );
	luaL_newmetatable( state, "TerraParameters" );
	lua_pushcfunction( state, TerraParameters_Destructor ); lua_setfield( state, -2, "__gc" );
	lua_pushvalue( state, -1 ); lua_setfield( state, -2, "__index" );
	luaL_setfuncs( state, TerraParametersFunctions, 0 );
	lua_pop( state, 1 );
}

