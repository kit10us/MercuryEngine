// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#include <melua/exports/ExportObjectAction.h>
#include <melua/xml/ExportXMLDocument.h>
#include <melua/xml/ExportXMLElement.h>
#include <melua/ScriptEngine.h>

namespace melua
{
	char* ObjectActionProxy::Name()
	{
		return "ObjectAction";
	}

	ObjectActionProxy::ObjectActionProxy( me::object::action::IObjectAction::ptr action )
		: m_action{ action }
	{
	}

	me::object::action::IObjectAction::ptr ObjectActionProxy::GetAction()
	{
		return m_action;
	}

	ObjectActionProxy::~ObjectActionProxy()
	{
		m_action.reset();
	}

	int ObjectAction_Constructor( lua_State * state )
	{
		ScriptEngine * se = ScriptEngine::GetInstance();
		auto gameInstance = se->GetGame();

		int args = lua_gettop( state );

		if( args == 1 )
		{ 
			std::string type = GetTypename( state, 1 );
			if( unify::string::StringIs( type, XMLElementProxy::Name() ) )
			{
				auto elementProxy = CheckUserType< XMLElementProxy >( state, 1 );
				auto actionObject = gameInstance->CreateObjectAction( elementProxy->GetElement() );
				return PushUserType< ObjectActionProxy >( state, { actionObject } );
			}
		}

		return PushNil( state );
	}

	int ObjectAction_Destructor( lua_State * state )
	{
		ObjectActionProxy* proxy = CheckUserType< ObjectActionProxy >( state, 1 );
		delete proxy;
		return -1;
	}

	void RegisterObjectAction( lua_State * state )
	{
		const luaL_Reg memberFunctions[] =
		{
			{ nullptr, nullptr }
		};

		ScriptEngine * se = ScriptEngine::GetInstance();
		Type type = { ObjectActionProxy::Name(), memberFunctions, ObjectAction_Constructor };
		se->AddType( type );
	}
}