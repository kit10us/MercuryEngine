// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxilua/ExportObject.h>
#include <dxilua/DXILua.h>
#include <dxilua/ExportV2.h>
#include <dxilua/ExportV3.h>

using namespace dxilua;
using namespace dxi;

extern "C"
int Object_SetEnabled( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	std::string objectPathName = lua_tostring( state, 1 );
	bool enabled = lua_toboolean( state, 2 ) ? true : false;

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

	object->SetEnabled( enabled );
	return 0;
}

extern "C"
int Object_GetEnabled( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	std::string objectPathName = lua_tostring( state, 1 );

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

	lua_pushboolean( state, object->GetEnabled() );

	return 1;
}

extern "C"
int Object_SetVisible( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	std::string objectPathName = lua_tostring( state, 1 );
	bool visible = lua_toboolean( state, 2 ) ? true : false;

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

	object->SetVisible( visible );
	return 0;
}

extern "C"
int Object_GetVisible( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	std::string objectPathName = lua_tostring( state, 1 );

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

	lua_pushboolean( state, object->GetVisible() );

	return 1;
}

extern "C"
int Object_SetSelectable( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	std::string objectPathName = lua_tostring( state, 1 );
	bool selectable = lua_toboolean( state, 2 ) ? true : false;

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

	object->SetSelectable( selectable );
	return 0;
}

extern "C"
int Object_GetSelectable( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	std::string objectPathName = lua_tostring( state, 1 );

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

	lua_pushboolean( state, object->GetSelectable() );

	return 1;
}

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
	assert( args == 2 );

	std::string objectPathName = lua_tostring( state, 1 );

	unify::V3< float > position( lua_ToV3( state, 2 ) );

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

	object->GetFrame().SetPosition( position );

	return 0;
}

extern "C"
int Object_LookAt( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	std::string objectPathName = lua_tostring( state, 1 );

	unify::V3< float > position( lua_ToV3( state, 2 ) );

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

	object->GetFrame().LookAt( position );

	return 0;
}

extern "C"
int Object_Orbit( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 4 );

	std::string objectPathName = lua_tostring( state, 1 );

	unify::V3< float > origin( lua_ToV3( state, 2 ) );

	unify::V2< float > axis( lua_ToV2( state, 3 ) );

	float distance = (float)lua_tonumber( state, 4 );

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

	object->GetFrame().Orbit( origin, axis, unify::Angle::AngleInRadians( distance ) );

	return 0;
}

static const luaL_Reg objectFuncs[] =
{
	{ "setenabled", Object_SetEnabled },
	{ "getenabled", Object_GetEnabled },
	{ "setvisible", Object_SetVisible },
	{ "getvisible", Object_GetVisible },
	{ "setselectable", Object_SetSelectable },
	{ "getselectable", Object_GetSelectable },
	{ "setgeometry", Object_SetGeometry },
	{ "setposition", Object_SetPosition },
	{ "lookat", Object_LookAt },
	{ "orbit", Object_Orbit },
	{ nullptr, nullptr }
};

int ExportObject( lua_State * state )
{
	luaL_newlib( state, objectFuncs );
	lua_setglobal( state, "object" );
	return 1;
}


