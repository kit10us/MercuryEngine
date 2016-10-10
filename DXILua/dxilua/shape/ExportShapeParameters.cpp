// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxilua/DXILua.h>
#include <dxilua/shape/ExportShapeParameters.h>
#include <dxilua/ExportMatrix.h>
#include <dxilua/unify/ExportColor.h>
#include <dxilua/unify/ExportV3.h>
#include <dxilua/unify/ExportSize2.h>
#include <dxilua/unify/ExportSize3.h>
#include <dxilua/ExportGeometry.h>
#include <dxi/shapes/ShapeCreators.h>

using namespace dxilua;
using namespace dxi;

int PushShapeParameters( lua_State * state, ShapeParameters & shapeParameters )
{
	ShapeParameters ** parameters = (ShapeParameters**)(lua_newuserdata( state, sizeof( ShapeParameters* ) ));
	*parameters = new ShapeParameters( shapeParameters );
	luaL_setmetatable( state, "ShapeParameters" );
	return 1;
}

ShapeParameters * CheckShapeParameters( lua_State* state, int index )
{
	ShapeParameters* ud = *(ShapeParameters**)luaL_checkudata( state, index, "ShapeParameters" );
	return ud;
}

// ShapeParameters...

int ShapeParameters_SetDiffuse( lua_State* state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	ShapeParameters * parameters = CheckShapeParameters( state, 1 );
	unify::ColorUnit diffuse = CheckColor( state, 2 );   
	parameters->parameters.Set< unify::Color>( "diffuse", diffuse );

	return 0;
}

int ShapeParameters_SetSpecular( lua_State* state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	ShapeParameters * parameters = CheckShapeParameters( state, 1 );
	unify::ColorUnit specular = CheckColor( state, 2 );
	parameters->parameters.Set< unify::Color>( "specular", specular );

	return 0;
}

int ShapeParameters_SetCenter( lua_State* state )
{
	ShapeParameters * parameters = CheckShapeParameters( state, 1 );
	unify::V3< float > center = CheckV3( state, 2 );
	parameters->parameters.Set< unify::V3< float > >( "center", center );
	return 0;
}

int ShapeParameters_SetTextureMode( lua_State* state )
{
	ShapeParameters * parameters = CheckShapeParameters( state, 1 );
	std::string textureMode = lua_tostring( state, 2 );
	parameters->parameters.Set< int >( "texturemode", dxi::TextureMode::FromString( textureMode ) );
	return 0;
}

int ShapeParameters_SetEffect( lua_State* state )
{
	ShapeParameters * parameters = CheckShapeParameters( state, 1 );
	std::string name = lua_tostring( state, 2 );
	auto game = ScriptEngine::GetGame();
	Effect::ptr effect = game->GetManager< Effect >()->Find( name );
	if ( ! effect )
	{
		game->ReportError( ErrorLevel::Failure, "LUA", "Effect \"" + name + "\" not found!" );
		return 0;
	}

	parameters->parameters.Set< Effect::ptr >( "effect", effect );
	return 0;
}		

// CubeParameters...

int CubeParameters_SetInf( lua_State* state )
{
	ShapeParameters * parameters = CheckShapeParameters( state, 1 );
	unify::V3< float > inf( CheckV3( state, 2 ) );
	parameters->parameters.Set< unify::V3< float > >( "inf", inf );
	return 0;
}

int CubeParameters_SetSup( lua_State* state )
{
	ShapeParameters * parameters = CheckShapeParameters( state, 1 );
	unify::V3< float > sup( CheckV3( state, 2 ) );
	parameters->parameters.Set< unify::V3< float > >( "sup", sup );
	return 0;
}

int CubeParameters_SetSize( lua_State* state )
{
	ShapeParameters * parameters = CheckShapeParameters( state, 1 );
	float size = (float)lua_tonumber( state, 2 );
	parameters->parameters.Set< float >( "size", size );
	return 0;
}

int CubeParameters_SetSize2( lua_State* state )
{
	ShapeParameters * parameters = CheckShapeParameters( state, 1 );
	unify::Size< float > size( CheckSize2( state, 2 ) );
	parameters->parameters.Set< unify::Size< float > >( "size2", size );
	return 0;
}

int CubeParameters_SetSize3( lua_State* state )
{
	ShapeParameters * parameters = CheckShapeParameters( state, 1 );
	unify::Size3< float > size( CheckSize3( state, 2 ) );
	parameters->parameters.Set< unify::Size3< float > >( "size3", size );
	return 0;
}

int CubeParameters_SetDiffuseFaces( lua_State* state )
{
	ShapeParameters * parameters = CheckShapeParameters( state, 1 );

	unify::Color colors[6];
	colors[0] = CheckColor( state, 2 );
	colors[1] = CheckColor( state, 3 );
	colors[2] = CheckColor( state, 4 );
	colors[3] = CheckColor( state, 5 );
	colors[4] = CheckColor( state, 6 );
	colors[5] = CheckColor( state, 7 );

	std::vector< unify::Color > diffuses( &colors[0], &colors[0] + 6 );
	parameters->parameters.Set< std::vector< unify::Color > >( "diffuses", diffuses );
	return 0;
}

int CubeParameters_SetDiffusePoints( lua_State* state )
{
	ShapeParameters * parameters = CheckShapeParameters( state, 1 );

	unify::Color colors[8];
	colors[0] = CheckColor( state, 2 );
	colors[1] = CheckColor( state, 3 );
	colors[2] = CheckColor( state, 4 );
	colors[3] = CheckColor( state, 5 );
	colors[4] = CheckColor( state, 6 );
	colors[5] = CheckColor( state, 7 );
	colors[6] = CheckColor( state, 8 );
	colors[7] = CheckColor( state, 9 );

	std::vector< unify::Color > diffuses( &colors[0], &colors[0] + 8 );
	parameters->parameters.Set< std::vector< unify::Color > >( "diffuses", diffuses );
	return 0;
}			

// PointfieldParameters...
int PointfieldParameters_SetMajorRadius( lua_State * state )
{
	ShapeParameters * parameters = CheckShapeParameters( state, 1 );
	float radius = (float)lua_tonumber( state, 2 );
	parameters->parameters.Set< float >( "majorradius", radius );
	return 0;
}

int PointfieldParameters_SetMinorRadius( lua_State * state )
{
	ShapeParameters * parameters = CheckShapeParameters( state, 1 );
	float radius = (float)lua_tonumber( state, 2 );
	parameters->parameters.Set< float >( "minorradius", radius );
	return 0;
}

int PointfieldParameters_SetCount( lua_State * state )
{
	ShapeParameters * parameters = CheckShapeParameters( state, 1 );
	unsigned int count = (unsigned int)lua_tonumber( state, 2 );
	parameters->parameters.Set< unsigned int >( "count", count );
	return 0;
}


// ConeParameters...

int ConeParameters_SetSegments( lua_State* state )
{
	ShapeParameters * parameters = CheckShapeParameters( state, 1 );
	unsigned int segments = (unsigned int)lua_tonumber( state, 2 );
	parameters->parameters.Set< unsigned int >( "segments", segments );
	return 0;
}

int ConeParameters_SetRadius( lua_State* state )
{
	ShapeParameters * parameters = CheckShapeParameters( state, 1 );
	float radius = (float)lua_tonumber( state, 2 );
	parameters->parameters.Set< float >( "radius", radius );
	return 0;
}

int ConeParameters_SetHeight( lua_State* state )
{
	ShapeParameters * parameters = CheckShapeParameters( state, 1 );
	float height = (float)lua_tonumber( state, 2 );
	parameters->parameters.Set< float >( "height", height );
	return 0;
}

int ConeParameters_SetCaps( lua_State* state )
{
	ShapeParameters * parameters = CheckShapeParameters( state, 1 );
	bool caps = lua_toboolean( state, 2 ) ? true : false;
	parameters->parameters.Set< bool >( "caps", caps );
	return 0;
}

static const luaL_Reg ShapeParametersFunctions[] =
{
	// ShapeBaseParameters...
	{ "SetDiffuse", ShapeParameters_SetDiffuse },
	{ "SetSpecular", ShapeParameters_SetSpecular },
	{ "SetCenter", ShapeParameters_SetCenter },
	{ "SetTextureMode", ShapeParameters_SetTextureMode },
	{ "SetEffect", ShapeParameters_SetEffect },

	// CubeParameters...
	{ "SetInf", CubeParameters_SetInf },
	{ "SetSup", CubeParameters_SetSup },
	{ "SetSize", CubeParameters_SetSize },
	{ "SetSize2", CubeParameters_SetSize2 },
	{ "SetSize3", CubeParameters_SetSize3 },
	{ "SetDiffuseFaces", CubeParameters_SetDiffuseFaces },
	{ "SetDiffusePoints", CubeParameters_SetDiffusePoints },

	// PointfieldParameters...
	{ "SetMajorRadius", PointfieldParameters_SetMajorRadius },
	{ "SetMinorRadius", PointfieldParameters_SetMinorRadius },
	{ "SetCount", PointfieldParameters_SetCount },

	// ConeParameters...
	{ "SetSegments", ConeParameters_SetSegments },
	{ "SetRadius", ConeParameters_SetRadius },
	{ "SetHeight", ConeParameters_SetHeight },
	{ "SetCaps", ConeParameters_SetCaps },


	{ nullptr, nullptr }
};

int ShapeParameters_Constructor( lua_State * state )
{
	std::string type = lua_tostring( state, 1 );

	ShapeParameters ** parameters = (ShapeParameters**)(lua_newuserdata( state, sizeof( ShapeParameters* ) ));
	*parameters = new ShapeParameters;
	(*parameters)->parameters.Set< shapes::Shape::TYPE >( "type",shapes::Shape::FromString( type ) );
	luaL_setmetatable( state, "ShapeParameters" );
	return 1;
}

int ShapeParameters_Destructor( lua_State * state )
{
	ShapeParameters * parameters = CheckShapeParameters( state, 1 );
	delete parameters;
	return 0;
}

void RegisterShapeParameters( lua_State * state )
{
	lua_register( state, "ShapeParameters", ShapeParameters_Constructor );
	luaL_newmetatable( state, "ShapeParameters" );
	lua_pushcfunction( state, ShapeParameters_Destructor ); lua_setfield( state, -2, "__gc" );
	lua_pushvalue( state, -1 ); lua_setfield( state, -2, "__index" );
	luaL_setfuncs( state, ShapeParametersFunctions, 0 );
	lua_pop( state, 1 );
}

