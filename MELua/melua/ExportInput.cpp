// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <melua/ScriptEngine.h>
#include <melua/ExportInput.h>
#include <me/input/IInputCondition.h>

using namespace melua;
using namespace me;
using namespace input;

int PushInput( lua_State * state, me::input::IInputSource::ptr input )
{
	InputProxy ** proxy = (InputProxy**)(lua_newuserdata( state, sizeof( InputProxy* ) ));
	*proxy = new InputProxy;
	luaL_setmetatable( state, "Input" );
	(*proxy)->input = input;
	return 1;
}

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

static const luaL_Reg ObjectFunctions[] =
{
	{ "Name", Input_Name },
	{ "SubSourceCount", Input_SubSourceCount },
	{ nullptr, nullptr }
};

int Input_Constructor( lua_State * state )
{
	int top = lua_gettop( state );
	int type = lua_type( state, 1 );

	auto game = ScriptEngine::GetGame();

	std::string name = luaL_checkstring( state, 1 );

	IInputSource::ptr input = game->GetInputManager()->FindSource( name );
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
	ScriptEngine * se = ScriptEngine::GetInstance();
	se->AddType( "Input", ObjectFunctions, sizeof( ObjectFunctions ) / sizeof( luaL_Reg ), Input_Constructor, Input_Destructor );
}

