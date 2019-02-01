// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#include <melua/xml/ExportXMLElement.h>
#include <melua/ScriptEngine.h>

namespace melua
{
	char* XMLElementProxy::Name()
	{
		return "XMLElement";
	}

	XMLElementProxy::XMLElementProxy( qxml::Element* element )
		: m_element{ element }
	{
	}

	XMLElementProxy::XMLElementProxy( qxml::Element::shared_ptr element )
		: m_element{ element.get() }
		, m_managed{ element }
	{
	}

	qxml::Element* XMLElementProxy::GetElement()
	{
		return m_element;
	}

	int XMLElement_AddAttribute( lua_State * state )
	{
		auto element = CheckUserType< XMLElementProxy >( state, 1 );

		std::string name = lua_tostring( state, 2 );
		std::string value = lua_tostring( state, 3 );

		element->GetElement()->AddAttribute( name, value );

		return 0;
	}

	int XMLElement_AddText( lua_State * state )
	{
		auto element = CheckUserType< XMLElementProxy >( state, 1 );

		std::string text = lua_tostring( state, 2 );

		element->GetElement()->AddText( text );

		return 0;
	}

	int XMLElement_AddElement( lua_State * state )
	{
		auto element = CheckUserType< XMLElementProxy >( state, 1 );

		std::string name = lua_tostring( state, 2 );

		return PushUserType< XMLElementProxy >( state, { element->GetElement()->AddElement( name, qxml::Element::NodeType::Element ) } );

		return 0;
	}

	int XMLElement_Constructor( lua_State * state )
	{
		int args = lua_gettop( state );

		std::string name = luaL_checkstring( state, 1 );
		return Push( state, XMLElementProxy{ qxml::Element::shared_ptr( new qxml::Element( name ) ) } );
	}

	int XMLElement_Destructor( lua_State* state )
	{
		auto xmlElement = CheckUserType< XMLElementProxy >( state, 0 );
		delete xmlElement;
		return 0;
	}

	void RegisterXMLElement( lua_State * state )
	{
		const luaL_Reg memberFunctions[] =
		{
			{ "AddAttribute", XMLElement_AddAttribute },
			{ "AddElement", XMLElement_AddElement },
			{ "AddText", XMLElement_AddText },
			{ nullptr, nullptr }
		};

		ScriptEngine * se = ScriptEngine::GetInstance();
		Type type = { XMLElementProxy::Name(), memberFunctions, XMLElement_Constructor, XMLElement_Destructor };
		se->AddType( type );
	}
}