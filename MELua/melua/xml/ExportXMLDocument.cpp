// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <melua/xml/ExportXMLDocument.h>
#include <melua/xml/ExportXMLElement.h>
#include <melua/ScriptEngine.h>

namespace melua
{
	char* XMLDocumentProxy::Name()
	{
		return "XMLDocument";
	}

	XMLDocumentProxy::XMLDocumentProxy( qxml::Document* document )
		: m_document{ document }
	{
	}

	XMLDocumentProxy::XMLDocumentProxy( std::shared_ptr< qxml::Document > document )
		: m_document{ document.get() }
		, m_managed{ document }
	{
	}

	qxml::Document* XMLDocumentProxy::GetDocument()
	{
		return m_document;
	}

	XMLDocumentProxy::~XMLDocumentProxy()
	{
		m_document = 0;
		m_managed.reset();
	}

	int XMLDocument_AddElement ( lua_State * state )
	{
		auto document = CheckUserType< XMLDocumentProxy >( state, 1 );

		std::string name = lua_tostring( state, 2 );

		auto element = new qxml::Element( name, qxml::Element::NodeType::Element, document->GetDocument() );
		return PushUserType< XMLElementProxy >( state, { element } );
	}

	int XMLDocument_GetRootElement( lua_State * state )
	{
		auto document = CheckUserType< XMLDocumentProxy >( state, 1 );

		return PushUserType< XMLElementProxy >( state, { document->GetDocument()->GetRoot() } );
	}

	int XMLDocument_FindElement( lua_State * state )
	{
		auto document = CheckUserType< XMLDocumentProxy >( state, 1 );

		std::string name = lua_tostring( state, 2 );

		return PushUserType< XMLElementProxy >( state, { document->GetDocument()->FindElement( name ) } );
	}

	int XMLDocument_Constructor( lua_State * state )
	{
		ScriptEngine * se = ScriptEngine::GetInstance();
		auto game = se->GetGame();

		int args = lua_gettop( state );

		if( args == 0 )
		{ // New empty XML document...
			return PushUserType< XMLDocumentProxy >( state, { std::shared_ptr< qxml::Document >( new qxml::Document() ) } );
		}
		else
		{ // Load an XML document...
			unify::Path path( lua_tostring( state, 1 ) );
			return PushUserType< XMLDocumentProxy >( state, { std::shared_ptr< qxml::Document >( new qxml::Document( path ) ) } );
		}
	}

	void RegisterXMLDocument( lua_State * state )
	{
		const luaL_Reg memberFunctions[] =
		{
			{ "AddElement", XMLDocument_AddElement },
			{ "GetRootElement", XMLDocument_GetRootElement },
			{ "FindElement", XMLDocument_FindElement },
			{ nullptr, nullptr }
		};

		ScriptEngine * se = ScriptEngine::GetInstance();
		se->AddType( { XMLDocumentProxy::Name(), memberFunctions, sizeof( memberFunctions ) / sizeof( luaL_Reg ), XMLDocument_Constructor } );
	}
}