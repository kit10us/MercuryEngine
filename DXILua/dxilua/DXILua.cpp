// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxilua/DXILua.h>

#pragma comment( lib, "lua53" )

using namespace dxilua;
using namespace dxi;

ScriptEngine * ScriptEngine::s_se;

//TODO: Replace asserts with runtime errors.

///////////////////////
// game->..

extern "C"
int Game_GetWidth( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 0 );

	auto game = ScriptEngine::GetGame();

	lua_pushnumber( state, game->GetOS().GetResolution().width );

	return 1;
}

extern "C"
int Game_GetHeight( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 0 );

	auto game = ScriptEngine::GetGame();

	lua_pushnumber( state, game->GetOS().GetResolution().height );

	return 1;
}

extern "C"
int Game_GetAspectRatioHW( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 0 );

	auto game = ScriptEngine::GetGame();

	lua_pushnumber( state, game->GetOS().GetResolution().AspectRatioHW() );

	return 1;
}

static const luaL_Reg gameFuncs[] =
{
	{ "getwidth", Game_GetWidth },
	{ "getheight", Game_GetHeight },
	{ "getaspectratiohw", Game_GetAspectRatioHW },
	{ nullptr, nullptr }
};

int luaopen_game( lua_State * state )
{
	luaL_newlib( state, gameFuncs );
	lua_setglobal( state, "game" );
	return 1;
}

///////////////////////
// Resources...

extern "C"
int Resources_AddResource( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 3 );

	std::string type = lua_tostring( state, 1 );
	std::string name = lua_tostring( state, 2 );
	std::string path = lua_tostring( state, 3 );

	auto game = ScriptEngine::GetGame();

	game->GetResourceHub().Load( type, name, path );

	return 0;
}

static const luaL_Reg resourcesFuncs[] =
{
	{ "add", Resources_AddResource },
	{ nullptr, nullptr }
};

int luaopen_resources( lua_State * state )
{
	luaL_newlib( state, resourcesFuncs );
	lua_setglobal( state, "resources" );
	return 1;
}

///////////////////////
// Scenes...

extern "C"
int Scenes_LoadScene( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	std::string name = lua_tostring( state, 1 );
	std::string path = lua_tostring( state, 2 );

	auto game = ScriptEngine::GetGame();

	game->GetSceneManager()->Load( name, path );

	return 0;
}

extern "C"
int Scenes_AddScene( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	std::string name = lua_tostring( state, 1 );

	auto game = ScriptEngine::GetGame();

	game->GetSceneManager()->Add( name );

	return 0;
}	

static const luaL_Reg scenesFuncs[] = 
{
	{ "load", Scenes_LoadScene },
	{ "add", Scenes_AddScene },
	{ nullptr, nullptr }
};

int luaopen_scenes( lua_State * state )
{
	luaL_newlib( state, scenesFuncs );
	lua_setglobal( state, "scenes" );
	return 1;
}

///////////////////////
// Scene...

extern "C"
int Scene_AddObject( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	std::string sceneName = lua_tostring( state, 1 );
	std::string name = lua_tostring( state, 2 );

	auto game = ScriptEngine::GetGame();

	scene::Scene * scene = game->GetSceneManager()->Find( sceneName ).get();
	scene->Add( name );

	return 0;
}

extern "C"
int Scene_SetCamera( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	std::string sceneName = lua_tostring( state, 1 );
	std::string camera = lua_tostring( state, 2 );

	auto game = ScriptEngine::GetGame();

	scene::Scene * scene = game->GetSceneManager()->Find( sceneName ).get();
	scene->SetCamera( camera );

	return 0;
}

extern "C"
int Scene_SetSize( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 3 );

	std::string sceneName = lua_tostring( state, 1 );
	float width = (float)lua_tonumber( state, 2 );
	float height = (float)lua_tonumber( state, 3 );

	auto game = ScriptEngine::GetGame();

	scene::Scene * scene = game->GetSceneManager()->Find( sceneName ).get();
	scene->SetSize( unify::Size< float >( width, height ) );

	return 0;
}

static const luaL_Reg sceneFuncs[] =
{
	{ "addobject", Scene_AddObject },
	{ "setcamera", Scene_SetCamera },
	{ "setsize", Scene_SetSize },
	{ nullptr, nullptr }
};

int luaopen_scene( lua_State * state )
{
	luaL_newlib( state, sceneFuncs );
	lua_setglobal( state, "scene" );
	return 1;
}

///////////////////////
// Object...

extern "C"
int Object_SetGeometry( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	std::string objectPathName = lua_tostring( state, 1 );
	std::string geometryName = lua_tostring( state, 2 );

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

	auto game = ScriptEngine::GetGame();

	scene::Scene * scene = game->GetSceneManager()->Find( sceneName ).get();
	assert( scene );

	scene::Object * object = scene->FindObject( objectName ).get();
	assert( object );

	auto geometry = game->GetManager< Geometry >()->Find( geometryName );

	object->SetGeometry( geometry );

	return 0;
}

extern "C"
int Object_SetPosition( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 4 );

	std::string objectPathName = lua_tostring( state, 1 );
	float x = (float)lua_tonumber( state, 2 );
	float y = (float)lua_tonumber( state, 3 );
	float z = (float)lua_tonumber( state, 4 );

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

	auto game = ScriptEngine::GetGame();

	scene::Scene * scene = game->GetSceneManager()->Find( sceneName ).get();
	assert( scene );

	scene::Object * object = scene->FindObject( objectName ).get();
	assert( object );

	object->GetFrame().SetPosition( unify::V3< float >( x, y, z ) );

	return 0;
}

extern "C"
int Object_LookAt( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 4 );

	std::string objectPathName = lua_tostring( state, 1 );
	float x = (float)lua_tonumber( state, 2 );
	float y = (float)lua_tonumber( state, 3 );
	float z = (float)lua_tonumber( state, 4 );

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

	auto game = ScriptEngine::GetGame();

	scene::Scene * scene = game->GetSceneManager()->Find( sceneName ).get();
	assert( scene );

	scene::Object * object = scene->FindObject( objectName ).get();
	assert( object );

	object->GetFrame().LookAt( unify::V3< float >( x, y, z ) );

	return 0;
}

static const luaL_Reg objectFuncs[] =
{
	{ "setgeometry", Object_SetGeometry },
	{ "setposition", Object_SetPosition },
	{ "lookat", Object_LookAt },
	{ nullptr, nullptr }
};

int luaopen_object( lua_State * state )
{
	luaL_newlib( state, objectFuncs );
	lua_setglobal( state, "object" );
	return 1;
}
	 
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

extern "C"
int Camera_SetPosition( lua_State * state )
{
	return Object_SetPosition( state );
}

extern "C"
int Camera_LookAt( lua_State * state )
{
	return Object_LookAt( state );
}

static const luaL_Reg cameraFuncs[] =
{
	{ "setprojection", Camera_SetProjection },
	{ "setposition", Camera_SetPosition },
	{ "lookat", Camera_LookAt },
	{ nullptr, nullptr }
};

int luaopen_camera( lua_State * state )
{
	luaL_newlib( state, cameraFuncs );
	lua_setglobal( state, "camera" );
	return 1;
}

///////////////////////
// Matrix...

extern "C"
int Matrix_MakeIdentity( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 0 );

	lua_newtable( state );

	unify::Matrix mat = unify::Matrix::MatrixIdentity();

	int index = 1;
	for( int c = 0; c < 4; ++c )
	{
		for( int r = 0; r < 4; ++r )
		{
			double value = mat( r, c );
			lua_pushnumber( state, value );
			lua_rawseti( state, -2, index++ );
		}
	}

	return 1;
}

extern "C"
int Matrix_MakePerspectiveFovLH( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 4 );

	float w = (float)lua_tonumber( state, 1 );
	float h = (float)lua_tonumber( state, 2 );
	float zn = (float)lua_tonumber( state, 3 );
	float zf = (float)lua_tonumber( state, 4 );

	lua_newtable( state );

	unify::Matrix mat = unify::Matrix::MatrixPerspectiveFovLH( w, h, zn, zf );
	
	int index = 1;
	for( int c = 0; c < 4; ++c )
	{
		for( int r = 0; r < 4; ++r )
		{
			double value = mat( r, c );
			lua_pushnumber( state, value );
			lua_rawseti( state, -2, index++ );
		}
	}

	return 1;
}

static const luaL_Reg matrixFuncs[] =
{
	{ "makeidentity", Matrix_MakeIdentity },
	{ "makeperspectivefovlh", Matrix_MakePerspectiveFovLH },
	{ nullptr, nullptr }
};

int luaopen_matrix( lua_State * state )
{
	luaL_newlib( state, matrixFuncs );
	lua_setglobal( state, "matrix" );
	return 1;
}

///////////////////////
// Debug...

extern "C"
int Debug_Write( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	std::string log = lua_tostring( state, 1 );

	OutputDebugStringA( log.c_str() );
	
	return 0;
}

extern "C"
int Debug_WriteLine( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	std::string log = lua_tostring( state, 1 );

	OutputDebugStringA( ( log + "\n" ).c_str() );

	return 0;
}

static const luaL_Reg debugFuncs[] =
{
	{ "write", Debug_Write },
	{ "writeline", Debug_WriteLine },
	{ nullptr, nullptr }
};

int luaopen_debug( lua_State * state )
{
	luaL_newlib( state, debugFuncs );
	lua_setglobal( state, "debug" );
	return 1;
}

///////////////////////
///////////////////////

ScriptEngine::ScriptEngine( dxi::core::Game * game )
	: m_game( game ),
	m_state{ luaL_newstate() }
{
	s_se = this;

	luaL_openlibs( m_state );

	// Add custom functions...
	luaopen_game( m_state );
	luaopen_resources( m_state );
	luaopen_scenes( m_state );
	luaopen_scene( m_state );
	luaopen_object( m_state );
	luaopen_camera( m_state );
	luaopen_matrix( m_state );
	luaopen_debug( m_state );
}

ScriptEngine::~ScriptEngine()
{
	if ( m_state != 0 )
	{
		lua_close( m_state );
	}
}


scripting::ExecuteResult ScriptEngine::ExecuteString( std::string line )
{
	int result = luaL_loadstring( m_state, line.c_str() );
	if( result != LUA_OK )
	{
		//print_error( state );
		return scripting::ExecuteResult::Fail;
	}

	result = lua_pcall( m_state, 0, LUA_MULTRET, 0 );
	if( result != LUA_OK )
	{
		//print_error( state );
		return scripting::ExecuteResult::Fail;
	}

	return scripting::ExecuteResult::Pass;
}

scripting::ExecuteResult ScriptEngine::ExecuteFile( unify::Path path )
{
	int result = luaL_loadfile( m_state, path.ToString().c_str() );
	if( result != LUA_OK )
	{
		//print_error( state );
		return scripting::ExecuteResult::Fail;
	}

	result = lua_pcall( m_state, 0, LUA_MULTRET, 0 );
	if( result != LUA_OK )
	{
		//print_error( state );
		return scripting::ExecuteResult::Fail;
	}

	return scripting::ExecuteResult::Pass;
}


dxi::core::Game * ScriptEngine::GetGame()
{
	return ScriptEngine::s_se->m_game;
}