// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxilua/ExportCamera.h>
#include <dxilua/DXILua.h>

using namespace dxilua;
using namespace dxi;

///////////////////////
// Camera...

extern "C"
int Camera_SetProjection( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	std::string objectPathName = lua_tostring( state, 1 );

	luaL_checktype( state, 2, LUA_TTABLE );

	unify::Matrix mat;
	int index = 1;
	for( int c = 0; c < 4; ++c )
	{
		for( int r = 0; r < 4; ++r )
		{
			lua_rawgeti( state, 2, index++ );
			float value = (float)lua_tonumber( state, -1 );
			mat.Set( r, c, value );
		}
	}

	auto game = ScriptEngine::GetGame();

	std::string sceneName;
	std::string objectName;
	auto i = objectPathName.find_first_of( ':', 0 );
	if( i == std::string::npos )
	{
		sceneName = objectPathName;
		// No object specified.
	}
	else
	{
		sceneName = std::string( objectPathName.begin(), objectPathName.begin() + i );
		objectName = std::string( objectPathName.begin() + i + 1, objectPathName.end() );
	}

	scene::Scene * scene = game->GetSceneManager()->Find( sceneName ).get();
	assert( scene );

	scene->GetCamera().SetProjection( mat );

	return 0;
}

static const luaL_Reg cameraFuncs[] =
{
	{ "SetProjection", Camera_SetProjection },
	{ nullptr, nullptr }
};

int ExportCamera( lua_State * state )
{
	luaL_newlib( state, cameraFuncs );
	lua_setglobal( state, "Camera" );
	return 1;
}