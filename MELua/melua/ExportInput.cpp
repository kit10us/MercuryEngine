// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <melua/ScriptEngine.h>
#include <melua/ExportInput.h>
#include <melua/ExportInputCondition.h>
#include <melua/unify/ExportV3.h>
#include <melua/unify/ExportV2.h>
#include <me/input/IInputCondition.h>
#include <me/scene/SceneManager.h>

using namespace melua;
using namespace me;
using namespace input;

int PushInput( lua_State * state, me::input::IInputDevice::ptr input )
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
			   
int Input_GetName( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 1 );

	InputProxy * inputProxy = CheckInput( state, 1 );

	lua_pushstring( state, inputProxy->input->GetName().c_str() );

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

int Input_AddEvent( lua_State * state )
{
	int args = lua_gettop( state );

	ScriptEngine * se = ScriptEngine::GetInstance();

	InputProxy * inputProxy = CheckInput( state, 1 );
	std::string owner = luaL_checkstring( state, 2 );

	unify::Owner::ptr ownership;
	if( unify::StringIs( owner, "game" ) )
	{
		ownership = se->GetGame()->GetOwnership();
	}
	else if( unify::StringIs( owner, "scene" ) )
	{
		auto sceneManager = se->GetGame()->GetComponentT< me::scene::SceneManager >( "SceneManager" );
		ownership = sceneManager->GetCurrentScene()->GetOwnership();
	}
	else
	{
		lua_pushboolean( state, 0 );
		return 1;
	}

	me::input::IInputCondition::ptr condition = CheckInputCondition( state, 3 )->condition;

	me::input::IInputAction::ptr action;

	assert( 0 ); // Need to work out actions first.

	auto input = inputProxy->input;
	input->AddEvent( ownership, condition, action );


	lua_pushstring( state, inputProxy->input->GetName().c_str() );

	return 1;
}

int Input_StickLow( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	unify::V3< float > v3( -0.70711f, -0.70711f, 0.0f );
	PushV3( state, v3 );
	return 1;
}

int Input_StickHigh( lua_State * state )
{
	int args = lua_gettop( state );
	assert( args == 2 );

	unify::V3< float > v3( 0.70711f, 0.70711f, 0.0f );
	PushV3( state, v3 );
	return 1;
}

static const luaL_Reg ObjectFunctions[] =
{
	{ "GetName", Input_GetName },
	{ "SubSourceCount", Input_SubSourceCount },
	{ "AddEvent", Input_AddEvent },
	{ nullptr, nullptr }
};

int Input_Constructor( lua_State * state )
{
	int top = lua_gettop( state );
	int type = lua_type( state, 1 );

	auto game = ScriptEngine::GetGame();

	std::string name = luaL_checkstring( state, 1 );

	IInputDevice::ptr input = game->GetInputManager()->FindSource( name );
	if ( ! input )
	{
		lua_pushnil( state );
		return 1;
	}

	return PushInput( state, input );
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
	Type type = { "Input", ObjectFunctions, sizeof( ObjectFunctions ) / sizeof( luaL_Reg ), Input_Constructor, Input_Destructor };
	type.named_constructors.push_back( { "StickLow", Input_StickLow } );
	type.named_constructors.push_back( { "StickHeigh", Input_StickHigh } );
	se->AddType( type );
}

