// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <melua/ScriptEngine.h>
#include <melua/exports/ExportAction.h>
#include <melua/xml/ExportXMLDocument.h>
#include <melua/xml/ExportXMLElement.h>
#include <melua/Util.h>

using namespace melua;
using namespace me;

char* ActionProxy::Name()
{
	return "Action";
}

int Action_Perform( lua_State * state )
{
	auto action = CheckUserType< ActionProxy >( state, 1 );
	return Push( state, action->action->Perform() );
}

int Action_Constructor( lua_State * state )
{
	int args = lua_gettop( state );

	ScriptEngine * se = ScriptEngine::GetInstance();
	auto gameInstance = se->GetGame();

	std::string type1 = GetTypename( state, 1 );
	if( unify::StringIs( type1, XMLElementProxy::Name() ) )
	{ 
		auto element = CheckUserType< XMLElementProxy >( state, 1 );
		auto action = gameInstance->CreateAction( element->GetElement() );
		return PushUserType< ActionProxy >( state, { action } );
	}
	
	return PushNil( state );
}

int Action_Destructor( lua_State * state )
{
	ActionProxy * proxy = CheckUserType< ActionProxy >( state, 1 );
	delete proxy;
	return 0;
}

void RegisterAction( lua_State * state )
{
	const luaL_Reg functions[] =
	{
		{ "Perform", Action_Perform },
		{ nullptr, nullptr }
	};

	ScriptEngine * se = ScriptEngine::GetInstance();
	Type type = { ActionProxy::Name(), functions, Action_Constructor, Action_Destructor };
	se->AddType( type );
}

