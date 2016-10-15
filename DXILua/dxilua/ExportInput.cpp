// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxilua/DXILua.h>
#include <dxilua/ExportInput.h>

using namespace dxilua;
using namespace dxi;

InputProxy* CheckInput( lua_State* state, int index )
{
	InputProxy* ud = *(InputProxy**)luaL_checkudata( state, index, "Input" );
	return ud;
}
			   
int Input_Name( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	InputProxy * inputProxy = CheckInput( state, 1 );

	lua_pushstring( state, inputProxy->input->Name().c_str() );

	return 1;
}

int Input_SubSourceCount( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	InputProxy * inputProxy = CheckInput( state, 1 );

	lua_pushnumber( state, inputProxy->input->SubSourceCount() );

	return 1;
}

int Input_GetState( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 4 );

	InputProxy * inputProxy = CheckInput( state, 1 );
	
	size_t subSource = (size_t)lua_tonumber( state, 2 );
	std::string name = lua_tostring( state, 3 );
	std::string condition = lua_tostring( state, 4 );

	input::State inputState = inputProxy->input->GetState( subSource, name, condition );
	int result = -1;
	if ( inputState == input::State::True )
	{
		result = 1;
	}
	else if ( inputState == input::State::False )
	{
		result = 0;
	}
	lua_pushnumber( state, result );

	return 1;
}

int Input_HasValue( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 3 );

	InputProxy * inputProxy = CheckInput( state, 1 );

	size_t subSource = (size_t)lua_tonumber( state, 2 );
	std::string name = lua_tostring( state, 3 );

	bool hasValue = inputProxy->input->HasValue( subSource, name );
	lua_pushboolean( state, hasValue ? 1 : 0 );

	return 1;
}

int Input_GetValue( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 3 );

	InputProxy * inputProxy = CheckInput( state, 1 );

	size_t subSource = (size_t)lua_tonumber( state, 2 );
	std::string name = lua_tostring( state, 3 );

	float value = inputProxy->input->GetValue( subSource, name );
	lua_pushnumber( state, value );

	return 1;
}

static const luaL_Reg ObjectFunctions[] =
{
	{ "Name", Input_Name },
	{ "SubSourceCount", Input_SubSourceCount },
	{ "GetState", Input_GetState },
	{ "HasValue", Input_HasValue },
	{ "GetValue", Input_GetValue },
	{ nullptr, nullptr }
};

int Input_Constructor( lua_State * state )
{
	int top = lua_gettop( state );
	int type = lua_type( state, 1 );

	auto game = ScriptEngine::GetGame();

	std::string name = luaL_checkstring( state, 1 );

	input::IInputSource::ptr input = game->GetInputManager()->Find( name );
	if ( ! input )
	{
		lua_pushnil( state );
		return 1;
	}

	InputProxy ** inputProxy = (InputProxy**)(lua_newuserdata( state, sizeof( InputProxy* ) ));
	*inputProxy = new InputProxy;
	(*inputProxy)->input = input;
	luaL_setmetatable( state, "Input" );

	return 1;
}

int Input_Destructor( lua_State * state )
{
	InputProxy * InputProxy = CheckInput( state, 1 );
	delete InputProxy;
	return 0;
}


void RegisterInput( lua_State * state )
{
	lua_register( state, "Input", Input_Constructor );
	luaL_newmetatable( state, "Input" );
	lua_pushcfunction( state, Input_Destructor ); lua_setfield( state, -2, "__gc" );
	lua_pushvalue( state, -1 ); lua_setfield( state, -2, "__index" );
	luaL_setfuncs( state, ObjectFunctions, 0 );
	lua_pop( state, 1 );
}
