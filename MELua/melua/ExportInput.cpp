// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <melua/ScriptEngine.h>
#include <melua/ExportInput.h>
#include <melua/ExportInputCondition.h>
#include <melua/unify/ExportV3.h>
#include <melua/unify/ExportV2.h>
#include <melua/ExportInputAction.h>
#include <melua/ExportObjectAction.h>
#include <melua/ExportAction.h>

#include <me/input/IInputCondition.h>
#include <me/scene/SceneManager.h>
#include <me/input/action/IA_Action.h>

using namespace melua;
using namespace me;
using namespace input;


char* InputProxy::Name()
{
	return "Input";
}

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

int Input_AddAction( lua_State * state )
{
	int args = lua_gettop( state );

	ScriptEngine * se = ScriptEngine::GetInstance();
	me::Game * game = se->GetGame();

	InputProxy * inputProxy = CheckInput( state, 1 );
	std::string ownerName = luaL_checkstring( state, 2 );

	unify::Owner::ptr owner;
	if( unify::StringIs( ownerName, "game" ) )
	{
		owner = se->GetGame()->GetOwnership();
	}
	else if( unify::StringIs( ownerName, "scene" ) )
	{
		auto sceneManager = se->GetGame()->GetComponentT< me::scene::SceneManager >( "SceneManager" );
		owner = sceneManager->GetCurrentScene()->GetOwnership();
	}
	else
	{
		lua_pushboolean( state, 0 );
		return 1;
	}
	
	// 3rd parameter is condition.
	me::input::IInputCondition::ptr condition = CheckInputCondition( state, 3 )->condition;
	
	std::string type = GetTypename( state, 4 );
	me::input::IInputAction::ptr action;
	if( unify::StringIs( type, ActionProxy::Name() ) )
	{
		auto action = CheckUserType< ActionProxy >( state, 4 );
		inputProxy->input->AddEvent( owner, condition, me::input::IInputAction::ptr( new me::input::action::Action( action->action ) ) );
		return Push( state, true );
	}
	else if( unify::StringIs( type, InputActionProxy::Name() ) )
	{
		auto inputAction = CheckUserType< InputActionProxy >( state, 4 );
		inputProxy->input->AddEvent( owner, condition, inputAction->action );
		return Push( state, true );
	}

	return Push( state, false );
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

int Input_Constructor( lua_State * state )
{
	int top = lua_gettop( state );
	int type = lua_type( state, 1 );

	ScriptEngine * se = ScriptEngine::GetInstance();
	auto game = se->GetGame();

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

	const luaL_Reg ObjectFunctions[] =
	{
		{ "GetName", Input_GetName },
		{ "SubSourceCount", Input_SubSourceCount },
		{ "AddAction", Input_AddAction },
		{ nullptr, nullptr }
	};

	Type type = { InputProxy::Name(), ObjectFunctions, sizeof( ObjectFunctions ) / sizeof( luaL_Reg ), Input_Constructor, Input_Destructor };
	type.named_constructors.push_back( { "StickLow", Input_StickLow } );
	type.named_constructors.push_back( { "StickHeigh", Input_StickHigh } );
	se->AddType( type );
}

