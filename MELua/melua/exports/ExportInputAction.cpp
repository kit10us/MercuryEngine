// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <melua/exports/ExportInputAction.h>
#include <melua/exports/ExportAction.h>
#include <melua/exports/ExportObjectAction.h>
#include <melua/xml/ExportXMLDocument.h>
#include <melua/xml/ExportXMLElement.h>
#include <melua/Util.h>

#include <melua/exports/ExportObject.h>
#include <me/input/action/IA_Action.h>
#include <me/input/action/IA_ObjectAction.h>

using namespace melua;
using namespace me;

char* InputActionProxy::Name()
{
	return "InputAction";
}

int InputAction_Constructor( lua_State * state )
{
	int args = lua_gettop( state );

	ScriptEngine * se = ScriptEngine::GetInstance();
	auto gameInstance = se->GetGame();

	std::string type1 = GetTypename( state, 1 );
	std::string type2 = GetTypename( state, 2 );
	if( unify::StringIs( type1, XMLElementProxy::Name() ) )
	{ 
		auto element = CheckUserType< XMLElementProxy >( state, 1 );
		auto action = gameInstance->CreateInputAction( element->GetElement() );
		return PushUserType< InputActionProxy >( state, { action } );
	}
	else if( unify::StringIs( type1, ObjectProxy::Name() ) && unify::StringIs( type2, ObjectActionProxy::Name() ) )
	{
		auto object = CheckUserType< ObjectProxy >( state, 1 )->object;
		auto objectAction = CheckUserType< ObjectActionProxy >( state, 2 )->GetAction();
		auto inputAction = me::input::IInputAction::ptr( new me::input::action::ObjectAction( object, objectAction ) );
		return PushUserType< InputActionProxy >( state, { inputAction } );
	}
	
	return PushNil( state );
}

int InputAction_Destructor( lua_State * state )
{
	InputActionProxy * proxy = CheckUserType< InputActionProxy >( state, 1 );
	delete proxy;
	return 0;
}

void RegisterInputAction( lua_State * state )
{
	const luaL_Reg GeometryFunctions[] =
	{
		{ nullptr, nullptr }
	};

	ScriptEngine * se = ScriptEngine::GetInstance();
	Type type = { InputActionProxy::Name(), GeometryFunctions, InputAction_Constructor, InputAction_Destructor };
	se->AddType( type );
}

